#include "HuyangFace.h"
#include <Arduino_GFX_Library.h>

HuyangFace::HuyangFace(Arduino_GFX *left, Arduino_GFX *right)
{
	_leftEye = left;
	_rightEye = right;
}

void HuyangFace::setup()
{
	_previousMillis = millis();

	_leftEye->begin();
	_rightEye->begin();

	_leftEye->fillScreen(_huyangEyeColor);
	_rightEye->fillScreen(_huyangEyeColor);

	delay(500);
}

void HuyangFace::loop()
{
	unsigned long now = millis();

	closeEyesLoop(now);

	if (now - _previousMillis > 100)
	{
		openEyesLoop();
		focusEyesLoop();
		sadEyesLoop();
		angryEyesLoop();
		updatePupilIdle();
	}

	if (automatic == true && now - _previousRandomMillis >= _randomDuration)
	{
		_previousRandomMillis = now;

		_randomDuration = random(3, 7) * 1000;

		if (_leftEyeTargetState == Closed || _rightEyeTargetState == Closed) {
			_leftEyeTargetState = Open;
			_rightEyeTargetState = Open;
			return;
		}

		if (_leftEyeTargetState != Open || _rightEyeTargetState != Open) {
			_leftEyeTargetState = Closed;
			_rightEyeTargetState = Closed;

			_randomDuration = 250;
			return;
		}

		// Balanced mood distribution: 0=Blink, 1=Focus, 2=Sad, 3=Angry
		uint8_t moodType = random(0, 4);

		switch (moodType)
		{
		case 1:
			_leftEyeTargetState = Focus;
			_rightEyeTargetState = Focus;
			break;
		case 2:
			_leftEyeTargetState = Sad;
			_rightEyeTargetState = Sad;
			break;
		case 3:
			_leftEyeTargetState = Angry;
			_rightEyeTargetState = Angry;
			break;
		default:
			_randomDuration = random(6, 11) * 1000;
			_leftEyeTargetState = Blink;
			_rightEyeTargetState = Blink;
			break;
		}
	}
}

// --- State Management ---

HuyangFace::EyeState HuyangFace::getStateFrom(uint8_t state)
{
	if (state == 1)
	{
		return HuyangFace::EyeState::Open;
	}
	else if (state == 2)
	{
		return HuyangFace::EyeState::Closed;
	}
	else if (state == 3)
	{
		return HuyangFace::EyeState::Blink;
	}
	else if (state == 4)
	{
		return HuyangFace::EyeState::Focus;
	}
	else if (state == 5)
	{
		return HuyangFace::EyeState::Sad;
	}
	else if (state == 6)
	{
		return HuyangFace::EyeState::Angry;
	}

	return HuyangFace::EyeState::None;
}

// --- Eye Color (User-konfigurierbar via Web/CLI) ---

void HuyangFace::setEyeColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
	_eyeR = r; _eyeG = g; _eyeB = b;
	// Display verwendet invertierte Farben (siehe _huyangEyeColor-Init)
	_huyangEyeColor = tftColor(255 - r, 255 - g, 255 - b);

	redrawOpenEyes();
}

void HuyangFace::setEyeColorHex(uint32_t rgb)
{
	setEyeColorRGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

uint32_t HuyangFace::getEyeColorHex()
{
	return ((uint32_t)_eyeR << 16) | ((uint32_t)_eyeG << 8) | _eyeB;
}

// --- Pupille ---

void HuyangFace::setPupilEnabled(bool on)
{
	if (_pupilEnabled == on) return;
	_pupilEnabled = on;
	redrawOpenEyes();
}

bool HuyangFace::getPupilEnabled() { return _pupilEnabled; }

void HuyangFace::setPupilColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
	_pupilR = r; _pupilG = g; _pupilB = b;
	// Display invertiert genauso wie Eye-Farbe
	_pupilColor565 = tftColor(255 - r, 255 - g, 255 - b);
	if (_pupilEnabled) redrawOpenEyes();
}

void HuyangFace::setPupilColorHex(uint32_t rgb)
{
	setPupilColorRGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}

uint32_t HuyangFace::getPupilColorHex()
{
	return ((uint32_t)_pupilR << 16) | ((uint32_t)_pupilG << 8) | _pupilB;
}

void HuyangFace::setPupilRadius(uint8_t px)
{
	if (px < 6) px = 6;
	if (px > 80) px = 80;
	_pupilRadius = px;
	if (_pupilEnabled) redrawOpenEyes();
}
uint8_t HuyangFace::getPupilRadius() { return _pupilRadius; }

void HuyangFace::setClosedEyeColorRGB(uint8_t r, uint8_t g, uint8_t b)
{
	_closedR = r; _closedG = g; _closedB = b;
	_huyangClosedEyeColor = tftColor(255 - r, 255 - g, 255 - b);
	// Re-render geschlossene Augen
	if (_leftEyeState == Closed) _leftEye->fillScreen(_huyangClosedEyeColor);
	if (_rightEyeState == Closed) _rightEye->fillScreen(_huyangClosedEyeColor);
}
void HuyangFace::setClosedEyeColorHex(uint32_t rgb)
{
	setClosedEyeColorRGB((rgb >> 16) & 0xFF, (rgb >> 8) & 0xFF, rgb & 0xFF);
}
uint32_t HuyangFace::getClosedEyeColorHex()
{
	return ((uint32_t)_closedR << 16) | ((uint32_t)_closedG << 8) | _closedB;
}

void HuyangFace::drawPupil(Arduino_GFX *eye)
{
	if (!_pupilEnabled) return;
	int16_t cx = _tftDisplayWidth / 2 + _pupilCurrX;
	int16_t cy = _tftDisplayHeight / 2 + _pupilCurrY;
	eye->fillCircle(cx, cy, _pupilRadius, _pupilColor565);
}

void HuyangFace::updatePupilIdle()
{
	if (!_pupilEnabled || !automatic) return;
	if (_leftEyeState != Open && _rightEyeState != Open) return;
	unsigned long now = millis();
	if (now - _pupilLastMove < _pupilNextInterval) return;

	int16_t maxOffsetX = (_tftDisplayHeight / 2) - _pupilRadius - 20;
	int16_t maxOffsetY = maxOffsetX / 2; // vertikal weniger Bewegung
	int16_t oldX = _pupilCurrX, oldY = _pupilCurrY;
	_pupilCurrX = random(-maxOffsetX, maxOffsetX + 1);
	_pupilCurrY = random(-maxOffsetY, maxOffsetY + 1);
	_pupilLastMove = now;
	_pupilNextInterval = random(2000, 5000);

	int16_t cx = _tftDisplayWidth / 2;
	int16_t cy = _tftDisplayHeight / 2;
	// alte Pupille mit Augenfarbe ueberschreiben, neue zeichnen
	if (_leftEyeState == Open) {
		_leftEye->fillCircle(cx + oldX, cy + oldY, _pupilRadius + 1, _huyangEyeColor);
		_leftEye->fillCircle(cx + _pupilCurrX, cy + _pupilCurrY, _pupilRadius, _pupilColor565);
	}
	if (_rightEyeState == Open) {
		_rightEye->fillCircle(cx + oldX, cy + oldY, _pupilRadius + 1, _huyangEyeColor);
		_rightEye->fillCircle(cx + _pupilCurrX, cy + _pupilCurrY, _pupilRadius, _pupilColor565);
	}
}

void HuyangFace::redrawOpenEyes()
{
	// Wenn ein Auge offen ist, neu fuellen und ggf. Pupille zeichnen
	if (_leftEyeState == Open || _leftEyeState == Blink) {
		_leftEye->fillScreen(_huyangEyeColor);
		drawPupil(_leftEye);
	}
	if (_rightEyeState == Open || _rightEyeState == Blink) {
		_rightEye->fillScreen(_huyangEyeColor);
		drawPupil(_rightEye);
	}
}

void HuyangFace::setEyesTo(EyeState newState)
{
	if (_leftEyeTargetState != newState || _rightEyeTargetState != newState)
	{
		_leftEyeLastSelectedState = newState;
		_rightEyeLastSelectedState = newState;
		_leftEyeTargetState = newState;
		_rightEyeTargetState = newState;
		automatic = false;
		_randomDuration = 0;
	}
}
void HuyangFace::setLeftEyeTo(EyeState newState)
{
	if (_leftEyeTargetState != newState)
	{
		_leftEyeLastSelectedState = newState;
		_leftEyeTargetState = newState;
		automatic = false;
		_randomDuration = 0;
	}
}
void HuyangFace::setRightEyeTo(EyeState newState)
{
	if (_rightEyeTargetState != newState)
	{
		_rightEyeLastSelectedState = newState;
		_rightEyeTargetState = newState;
		automatic = false;
		_randomDuration = 0;
	}
}

// --- Open Eyes ---

void HuyangFace::openEyesLoop()
{
	if ((_leftEyeTargetState == Open || _leftEyeTargetState == Blink) || (_rightEyeTargetState == Open || _rightEyeTargetState == Blink))
	{
		bool shouldDoLeftEye = (_leftEyeTargetState == Open || _leftEyeTargetState == Blink);
		bool shouldDoRightEye = (_rightEyeTargetState == Open || _rightEyeTargetState == Blink);

		if (shouldDoLeftEye && _leftEyeState != Open && shouldDoRightEye && _rightEyeState != Open)
		{
			openEyes(_huyangEyeColor);
			_leftEyeState = Open;
			_rightEyeState = Open;
		}
		else if ((_leftEyeTargetState == Open || _leftEyeTargetState == Blink) && _leftEyeState != Open)
		{
			openEye(_leftEye, _huyangEyeColor);
			_leftEyeState = Open;
		}
		else if ((_rightEyeTargetState == Open || _rightEyeTargetState == Blink) && _rightEyeState != Open)
		{
			openEye(_rightEye, _huyangEyeColor);
			_rightEyeState = Open;
		}

		if (_leftEyeTargetState == Blink)
		{
			_leftEyeTargetState = Open;
		}
		if (_rightEyeTargetState == Blink)
		{
			_rightEyeTargetState = Open;
		}
	}
}

void HuyangFace::openEyes(uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= _tftDisplayHeight / 2; step++)
	{
		uint16_t position = (_tftDisplayHeight / 2) - step;

		_leftEye->drawFastHLine(0, position, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, position, _tftDisplayWidth, color);
		_leftEye->drawFastHLine(0, _tftDisplayHeight - position, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, _tftDisplayHeight - position, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	drawPupil(_leftEye);
	drawPupil(_rightEye);
	_previousMillis = millis();
}

void HuyangFace::openEye(Arduino_GFX *eye, uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= _tftDisplayHeight / 2; step++)
	{
		uint16_t position = (_tftDisplayHeight / 2) - step;

		eye->drawFastHLine(0, position, _tftDisplayWidth, color);
		eye->drawFastHLine(0, _tftDisplayHeight - position, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	drawPupil(eye);
	_previousMillis = millis();
}

// --- Close Eyes ---

void HuyangFace::closeEyesLoop(unsigned long now)
{
	if (now - _previousMillis > _randomDuration)
	{

		if ((_leftEyeTargetState == Closed || _leftEyeTargetState == Blink) || (_rightEyeTargetState == Closed || _rightEyeTargetState == Blink))
		{

			if ((_leftEyeTargetState == Closed || _leftEyeTargetState == Blink) && _leftEyeState != Closed && (_rightEyeTargetState == Closed || _rightEyeTargetState == Blink) && _rightEyeState != Closed)
			{
				closeEyes(_huyangClosedEyeColor);
				_leftEyeState = Closed;
				_rightEyeState = Closed;
			}
			else if ((_leftEyeTargetState == Closed || _leftEyeTargetState == Blink) && _leftEyeState != Closed)
			{
				closeEye(_leftEye, _huyangClosedEyeColor);
				_leftEyeState = Closed;
			}
			else if ((_rightEyeTargetState == Closed || _rightEyeTargetState == Blink) && _rightEyeState != Closed)
			{
				closeEye(_rightEye, _huyangClosedEyeColor);
				_rightEyeState = Closed;
			}
		}
	}
}

void HuyangFace::closeEyes(uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= _tftDisplayHeight / 2; step++)
	{

		_leftEye->drawFastHLine(0, step, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, step, _tftDisplayWidth, color);
		_leftEye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}

void HuyangFace::closeEye(Arduino_GFX *eye, uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= _tftDisplayHeight / 2; step++)
	{
		eye->drawFastHLine(0, step, _tftDisplayWidth, color);
		eye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}

// --- Focus Eyes ---

void HuyangFace::focusEyesLoop()
{
	if (_leftEyeTargetState == Focus || _rightEyeTargetState == Focus)
	{

		bool shouldDoLeftEye = _leftEyeTargetState == Focus && _leftEyeState != Focus;
		bool shouldDoRightEye = _rightEyeTargetState == Focus && _rightEyeState != Focus;

		if (shouldDoLeftEye && shouldDoRightEye)
		{
			focusEyes(_huyangClosedEyeColor);
			_leftEyeState = Focus;
			_rightEyeState = Focus;
		}
		else if (_leftEyeTargetState == Focus && _leftEyeState != Focus)
		{
			focusEye(_leftEye, _huyangClosedEyeColor);
			_leftEyeState = Focus;
		}
		else if (_rightEyeTargetState == Focus && _rightEyeState != Focus)
		{
			focusEye(_rightEye, _huyangClosedEyeColor);
			_rightEyeState = Focus;
		}
	}
}

void HuyangFace::focusEyes(uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= ((_tftDisplayHeight / 2) / 6 * 4); step++)
	{

		_leftEye->drawFastHLine(0, step, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, step, _tftDisplayWidth, color);
		_leftEye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);
		_rightEye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}
void HuyangFace::focusEye(Arduino_GFX *eye, uint16_t color)
{
	unsigned long prev = millis();

	for (uint16_t step = 0; step <= ((_tftDisplayHeight / 2) / 6 * 4); step++)
	{
		eye->drawFastHLine(0, step, _tftDisplayWidth, color);
		eye->drawFastHLine(0, _tftDisplayHeight - step, _tftDisplayWidth, color);

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}

// --- Sad Eyes ---

void HuyangFace::sadEyesLoop()
{
	if (_leftEyeTargetState == Sad || _rightEyeTargetState == Sad)
	{

		bool shouldDoLeftEye = (_leftEyeTargetState == Sad && _leftEyeState != Sad);
		bool shouldDoRightEye = (_rightEyeTargetState == Sad && _rightEyeState != Sad);

		if (shouldDoLeftEye && shouldDoRightEye)
		{
			sadEyes(_huyangClosedEyeColor);
			_leftEyeState = Sad;
			_rightEyeState = Sad;
		}
		else if (_leftEyeTargetState == Sad && _leftEyeState != Sad)
		{
			sadEye(_leftEye, true, _huyangClosedEyeColor);
			_leftEyeState = Sad;
		}
		else if (_rightEyeTargetState == Sad && _rightEyeState != Sad)
		{
			sadEye(_rightEye, false, _huyangClosedEyeColor);
			_rightEyeState = Sad;
		}
	}
}

void HuyangFace::sadEyes(uint16_t color)
{
	unsigned long prev = millis();

	uint16_t length = _tftDisplayHeight;

	for (uint16_t step = 0; step <= _tftDisplayHeight; step++)
	{
		_leftEye->drawFastHLine(_tftDisplayHeight - length, _tftDisplayHeight - step, length, color);
		_rightEye->drawFastHLine(0, _tftDisplayHeight - step, length, color);
		if (length >= 2) length -= 2; else length = 0;

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}
void HuyangFace::sadEye(Arduino_GFX *eye, bool inner, uint16_t color)
{
	unsigned long prev = millis();
	uint16_t length = _tftDisplayHeight;
	uint16_t left = 0;

	for (uint16_t step = 0; step <= _tftDisplayHeight; step++)
	{

		if (inner == true)
		{
			left = _tftDisplayHeight - length;
		}

		eye->drawFastHLine(left, _tftDisplayHeight - step, length, color);

		if (length >= 2) length -= 2; else length = 0;

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}

// --- Angry Eyes ---

void HuyangFace::angryEyesLoop()
{
	if (_leftEyeTargetState == Angry || _rightEyeTargetState == Angry)
	{
		bool shouldDoLeftEye = (_leftEyeTargetState == Angry && _leftEyeState != Angry);
		bool shouldDoRightEye = (_rightEyeTargetState == Angry && _rightEyeState != Angry);

		if (shouldDoLeftEye && shouldDoRightEye)
		{
			angryEyes(_huyangClosedEyeColor);
			_leftEyeState = Angry;
			_rightEyeState = Angry;
		}
		else if (_leftEyeTargetState == Angry && _leftEyeState != Angry)
		{
			sadEye(_leftEye, false, _huyangClosedEyeColor);
			_leftEyeState = Angry;
		}
		else if (_rightEyeTargetState == Angry && _rightEyeState != Angry)
		{
			sadEye(_rightEye, true, _huyangClosedEyeColor);
			_rightEyeState = Angry;
		}
	}
}

void HuyangFace::angryEyes(uint16_t color)
{
	unsigned long prev = millis();

	uint16_t length = _tftDisplayHeight;

	for (uint16_t step = 0; step <= _tftDisplayHeight; step++)
	{
		_leftEye->drawFastHLine(0, _tftDisplayHeight - step, length, color);
		_rightEye->drawFastHLine(_tftDisplayHeight - length, _tftDisplayHeight - step, length, color);
		if (length >= 2) length -= 2; else length = 0;

		while (millis() - prev < _blinkDelay)
		{
			yield();
		}
		prev = millis();
	}
	_previousMillis = millis();
}
