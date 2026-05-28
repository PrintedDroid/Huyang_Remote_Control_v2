#include "HuyangBody.h"
#include "pins.h"
#include <Adafruit_PWMServoDriver.h>

HuyangBody::HuyangBody(Adafruit_PWMServoDriver *pwm)
{
	_pwm = pwm;

	_neoPixelLights = new Adafruit_NeoPixel(2, PIN_NEOPIXEL, pixelFormat);
	_neoPixelLights->setBrightness(20);
	_neoPixelLights->begin();

	// Default-Farbe (dunkelblau, wie im Original)
	_chestColor = _neoPixelLights->Color(0, 0, 128);
}

void HuyangBody::setRawServo(uint8_t channel, uint16_t pwm)
{
	if (channel > 15) return;
	if (pwm > 4095) pwm = 4095;
	_pwm->setPWM(channel, 0, pwm);
}

void HuyangBody::rotateServo(uint8_t servo, uint16_t degree)
{
	degree = constrain(degree, (uint16_t)0, (uint16_t)180);
	uint16_t pulselength = map(degree, 0, 180, HuyangBody_SERVOMIN, HuyangBody_SERVOMAX);
	_pwm->setPWM(servo, 0, pulselength);
}

void HuyangBody::tiltBodySideways(int16_t degree)
{
	int16_t calibrationShift = -22;  // max 0 to -60
	int16_t calibratedDegree = map(degree, -100, 100, -60 + calibrationShift, 60 + calibrationShift);
	uint16_t rotateDegree = map(calibratedDegree, -100, 100, 0, 170);

	rotateServo(pwm_pin_sideway_left, rotateDegree + 0);
	rotateServo(pwm_pin_sideway_right, rotateDegree + 15);
}
void HuyangBody::tiltBodyForward(int16_t degree)
{
	int16_t calibrationShift = 40; // max 0 to 40
	uint16_t rotateDegree = map(degree, -100, 100, 30 + calibrationShift, 140 + calibrationShift);
	rotateDegree = constrain(rotateDegree, (uint16_t)0, (uint16_t)180);

	rotateServo(pwm_pin_forward_left, rotateDegree);
	rotateServo(pwm_pin_forward_right, 180 - rotateDegree);
}
void HuyangBody::rotateBody(int16_t degree)
{
	int16_t calibrationShift = 0; // max 0 to 20
	uint16_t rotateDegree = map(degree, -100, 100, 0 + calibrationShift, 70 + calibrationShift);
	rotateServo(pwm_pin_body_rotate, rotateDegree);
}

void HuyangBody::centerAll()
{
	tiltBodySideways(0);
	delay(200);

	tiltBodyForward(0);
	delay(200);

	rotateBody(0);
	delay(200);
}

void HuyangBody::updateChestLights()
{
	uint32_t color = _chestColor;

	// Blink-Modus: jede Sekunde umschalten zwischen Farbe und Aus
	if (_blinkActive)
	{
		unsigned long now = millis();
		if (now - _blinkPrevious > 500)
		{
			_blinkPrevious = now;
			_blinkState = !_blinkState;
		}
		if (!_blinkState) color = 0; // aus
	}

	_neoPixelLights->setPixelColor(0, color);
	_neoPixelLights->setPixelColor(1, color);
	_neoPixelLights->show();
}

void HuyangBody::setChestColor(uint8_t r, uint8_t g, uint8_t b)
{
	_chestColor = _neoPixelLights->Color(r, g, b);
	_blinkActive = false;
}

void HuyangBody::setStatus(Status s)
{
	_statusRevertAt = 0;
	_blinkActive = false;

	switch (s)
	{
	case StatusBoot:
		setChestColor(128, 0, 0); // rot
		break;
	case StatusConnectingSTA:
		setChestColor(128, 96, 0); // gelb
		break;
	case StatusConnectedSTA:
		setChestColor(0, 128, 0); // gruen
		_statusRevertAt = millis() + 2000; // nach 2s zurueck zu Default
		break;
	case StatusAPMode:
		setChestColor(96, 0, 96); // magenta
		break;
	case StatusError:
		setChestColor(192, 0, 0); // rot blinkend
		_blinkActive = true;
		_blinkPrevious = millis();
		_blinkState = true;
		break;
	case StatusDefault:
	default:
		setChestColor(0, 0, 128); // dunkelblau
		break;
	}
}

void HuyangBody::setup()
{
    centerAll();
}

void HuyangBody::loop()
{
	unsigned long now = millis();

	// Auto-Revert nach StatusConnectedSTA
	if (_statusRevertAt != 0 && now >= _statusRevertAt)
	{
		_statusRevertAt = 0;
		setStatus(StatusDefault);
	}

	if (now - _previousMillis > 100)
	{
		_previousMillis = now;
		updateChestLights();
	}
}
