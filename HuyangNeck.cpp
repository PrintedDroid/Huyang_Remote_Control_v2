#include "HuyangNeck.h"
#include <Adafruit_PWMServoDriver.h>

HuyangNeck::HuyangNeck(Adafruit_PWMServoDriver *pwm)
{
	_pwm = pwm;
}

void HuyangNeck::setup()
{
	rotateHead(0);
	tiltNeckForward(0);
}

void HuyangNeck::loop()
{
	_currentMillis = millis();
	if (_currentMillis - _previousMillis >= 20)
	{
		_previousMillis = _currentMillis;
		updateNeckPosition();
		updateCurrentRotate();
	}

	if (automatic == true)
	{
		doRandomRotate();
		doRandomTiltForward();
	}
}

// --- Servo & Easing ---

void HuyangNeck::rotateServo(uint8_t servo, double degree)
{
	uint16_t pulselength = map(degree, 0, 180, HuyangNeck_SERVOMIN, HuyangNeck_SERVOMAX);
	_pwm->setPWM(servo, 0, pulselength);
}

double HuyangNeck::easeInOutQuad(double t)
{
	return t < 0.5 ? 2 * t * t : t * (4 - 2 * t) - 1;
}

double HuyangNeck::easeInAndOut(double start, double current, double target, double percentage)
{
	double result = target;

	if (percentage > 1.0)
	{
		percentage = 1.0;
	}

	if (current != target)
	{
		double easeInOut = easeInOutQuad(percentage);

		if (current < target)
		{
			double subTarget = (target - start);
			result = start + (subTarget * easeInOut);
			if (result > target)
			{
				result = target;
			}
		}
		else if (current > target)
		{
			double subTarget = (start - target);
			result = start - (subTarget * easeInOut);
			if (result < target)
			{
				result = target;
			}
		}
	}

	return result;
}

// --- Neck Position ---

void HuyangNeck::updateNeckPosition()
{
	if (_currentTiltForward != targetTiltForward)
	{
		double tiltForwardPercentage = (_tiltForwardDuration > 0) ? (_currentMillis - _tiltForwardStartMillis) / _tiltForwardDuration : 1.0;
		_currentTiltForward = easeInAndOut(_startTiltForward, _currentTiltForward, targetTiltForward, tiltForwardPercentage);
	}

	if (_tiltSidewaysPercentage < 1.0)
	{
		_currentTiltSideways = easeInAndOut(_startTiltSideways, _currentTiltSideways, targetTiltSideways, _tiltSidewaysPercentage);
		_tiltSidewaysPercentage += 0.06;
	}

	uint16_t leftDegree = map(_currentTiltForward + _currentTiltSideways, 0, 200, 65, 10);
	uint16_t rightDegree = map(_currentTiltForward - _currentTiltSideways, 0, 200, 35, 90);
	uint16_t neckDegree = map(_currentTiltForward, 0, 200, 100, 0);

	leftDegree = constrain(leftDegree, uint16_t(10), uint16_t(65));
	rightDegree = constrain(rightDegree, uint16_t(35), uint16_t(90));
	neckDegree = constrain(neckDegree, uint16_t(0), uint16_t(100));

	rotateServo(pwm_pin_head_left, leftDegree);
	rotateServo(pwm_pin_head_right, rightDegree);
	rotateServo(pwm_pin_head_neck, neckDegree);
}

// --- Tilt ---

void HuyangNeck::tiltNeckSideways(double degree)
{
	degree = constrain(degree, _minTiltSideways, _maxTiltSideways);
	degree = map(degree, _minTiltSideways, _maxTiltSideways, -50, 50);

	if (targetTiltSideways != degree)
	{
		_startTiltSideways = _currentTiltSideways;
		targetTiltSideways = degree;
		_tiltSidewaysPercentage = 0.0;
	}
}

void HuyangNeck::tiltNeckForward(double degree, double duration)
{
	degree = degree + 100;
	degree = constrain(degree, _minTiltForward, _maxTiltForward);

	if (targetTiltForward != degree)
	{
		if (duration == 0)
		{
			double way = 0;
			if (degree > _currentTiltForward)
			{
				way = degree - _currentTiltForward;
			}
			else
			{
				way = _currentTiltForward - degree;
			}
			duration = way * 16;
		}

		_startTiltForward = _currentTiltForward;
		targetTiltForward = degree;
		_tiltForwardDuration = (_speedMultiplier > 0.0f) ? duration / _speedMultiplier : duration;
		_tiltForwardStartMillis = millis(); // Use fresh millis(), not stale _currentMillis
	}
}

void HuyangNeck::setSpeedMultiplier(float m)
{
	if (m < 0.1f) m = 0.1f;
	if (m > 5.0f) m = 5.0f;
	_speedMultiplier = m;
}
float HuyangNeck::getSpeedMultiplier() { return _speedMultiplier; }

// --- Rotation ---

void HuyangNeck::rotateHead(double degree, double duration)
{
	degree = constrain(degree, _minRotation, _maxRotation);

	if (targetRotate != degree)
	{
		if (duration == 0)
		{
			double way = 0;
			if (degree > _currentRotate)
			{
				way = degree - _currentRotate;
			}
			else
			{
				way = _currentRotate - degree;
			}
			duration = way * 16;
		}

		_startRotate = _currentRotate;
		targetRotate = degree;
		_rotationDuration = (_speedMultiplier > 0.0f) ? duration / _speedMultiplier : duration;
		_rotationStartMillis = millis(); // Use fresh millis(), not stale _currentMillis
	}
}

void HuyangNeck::updateCurrentRotate()
{
	if (_currentRotate != targetRotate)
	{
		double percentage = (_rotationDuration > 0) ? (_currentMillis - _rotationStartMillis) / _rotationDuration : 1.0;
		_currentRotate = easeInAndOut(_startRotate, _currentRotate, targetRotate, percentage);

		uint16_t rotateDegree = map(_currentRotate, _minRotation, _maxRotation, 0, 110);
		rotateServo(pwm_pin_head_rotate, rotateDegree);
	}
}

// --- Random Animations ---

void HuyangNeck::doRandomRotate()
{
	if (_currentMillis - _randomRotateStart >= _randomRotateInterval)
	{
		_randomRotateStart = _currentMillis;
		_randomRotateInterval = 2000 + (random(6, 13) * 1000);

		if (_currentRotate > 0)
		{
			rotateHead(-(random(10, 81)), random(2, 7) * 1000);
		}
		else
		{
			rotateHead(random(10, 81), random(2, 7) * 1000);
		}
	}
}

void HuyangNeck::doRandomTiltForward()
{
	if (_currentMillis - _randomTiltStart >= _randomTiltInterval)
	{
		_randomTiltStart = _currentMillis;
		_randomTiltInterval = 2500 + (random(6, 13) * 1050);

		double workArea = 60;
		double center = -15;

		tiltNeckForward(random(center - workArea, center + workArea + 1), random(3, 7) * 1000);
	}
}
