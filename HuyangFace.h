#ifndef HuyangFace_h
#define HuyangFace_h

#include "Arduino.h"
#include <Arduino_GFX_Library.h>

#define tftColor(r, g, b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | ((b) >> 3))

class HuyangFace
{
public:
	enum EyeState
	{
		None = 0,
		Open = 1,
		Closed = 2,
		Blink = 3,
		Focus = 4,
		Sad = 5,
		Angry = 6
	};

	HuyangFace(Arduino_GFX *left, Arduino_GFX *right);

	void setup();
	void loop();

	bool automatic = true;

	void setEyesTo(EyeState newState);
	void setLeftEyeTo(EyeState newState);
	void setRightEyeTo(EyeState newState);

	EyeState getStateFrom(uint8_t state);

	// User-configurable eye color (RGB888 from web/CLI)
	// Stored in the same "inverted" format as the default value (see _huyangEyeColor init)
	void setEyeColorRGB(uint8_t r, uint8_t g, uint8_t b);
	void setEyeColorHex(uint32_t rgb);          // 0xRRGGBB
	uint32_t getEyeColorHex();                  // returns currently set color as 0xRRGGBB

	// Pupil - optional, default color black, user-configurable
	void setPupilEnabled(bool on);
	bool getPupilEnabled();
	void setPupilColorRGB(uint8_t r, uint8_t g, uint8_t b);
	void setPupilColorHex(uint32_t rgb);
	uint32_t getPupilColorHex();
	void setPupilRadius(uint8_t px);    // 6 - 80 px
	uint8_t getPupilRadius();

	// Closed-eye color (also used for Focus/Sad/Angry as they share the "closed" color)
	void setClosedEyeColorRGB(uint8_t r, uint8_t g, uint8_t b);
	void setClosedEyeColorHex(uint32_t rgb);
	uint32_t getClosedEyeColorHex();

private:
	Arduino_GFX *_leftEye;
	Arduino_GFX *_rightEye;

	unsigned long _previousMillis = 0;
	unsigned long _previousRandomMillis = 0;

	uint16_t _tftDisplayWidth = 240;
	uint16_t _tftDisplayHeight = 240;

	uint16_t _huyangEyeColor = tftColor(255 - 255, 255 - 221, 255 - 34); // 0xFD20
	uint16_t _huyangClosedEyeColor = tftColor(255, 255, 255);

	// Stored user RGB (before display inversion) - for echo back to the web UI
	uint8_t _eyeR = 255, _eyeG = 221, _eyeB = 34;

	// Pupil - default off, black
	bool _pupilEnabled = false;
	uint8_t _pupilR = 0, _pupilG = 0, _pupilB = 0;
	uint16_t _pupilColor565 = 0x0000;
	uint8_t _pupilRadius = 30; // px, medium default

	// Pupil idle movement (in automatic mode the pupil drifts slightly)
	int16_t _pupilCurrX = 0, _pupilCurrY = 0;
	unsigned long _pupilLastMove = 0;
	unsigned long _pupilNextInterval = 2500;
	void updatePupilIdle();

	// Closed-eye user RGB (before display inversion)
	uint8_t _closedR = 255, _closedG = 255, _closedB = 255;
	void drawPupil(Arduino_GFX *eye);
	void redrawOpenEyes(); // helper: re-fill + draw pupil on both eyes if currently open

	EyeState _leftEyeLastSelectedState = Blink;
	EyeState _rightEyeLastSelectedState = Blink;

	EyeState _leftEyeTargetState = Blink;
	EyeState _rightEyeTargetState = Blink;

	EyeState _leftEyeState = Closed;
	EyeState _rightEyeState = Closed;

	uint32_t _randomDuration = 2000;
	uint16_t _blinkDelay = 1;

	void openEyesLoop();
	void openEyes(uint16_t color);
	void openEye(Arduino_GFX *eye, uint16_t color);

	void closeEyesLoop(unsigned long now);
	void closeEyes(uint16_t color);
	void closeEye(Arduino_GFX *eye, uint16_t color);

	void focusEyesLoop();
	void focusEyes(uint16_t color);
	void focusEye(Arduino_GFX *eye, uint16_t color);

	void sadEyesLoop();
	void sadEyes(uint16_t color);
	void sadEye(Arduino_GFX *eye, bool inner, uint16_t color);

	void angryEyesLoop();
	void angryEyes(uint16_t color);
};

#endif
