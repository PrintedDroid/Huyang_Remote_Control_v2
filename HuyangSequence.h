#ifndef HuyangSequence_h
#define HuyangSequence_h

#include <Arduino.h>
#include "HuyangFace.h"
#include "HuyangNeck.h"
#include "HuyangBody.h"
#include "HuyangAudio.h"

// Eine Sequenz ist eine Liste zeitgesteuerter Aktionen.
// Jeder Step hat einen Zeitpunkt (Offset in ms ab Sequenz-Start) und eine Aktion.
// Das ermoeglicht koordinierte Performances (Augen + Bewegung + Audio synchron).

struct __attribute__((packed)) SequenceStep
{
	uint16_t atMs;

	enum Action : uint8_t
	{
		ActionNone,
		ActionEyeState,         // param1 = EyeState
		ActionNeckRotate,       // param1 = degree (-100..100), param2 = duration ms
		ActionNeckTiltForward,  // param1 = degree, param2 = duration ms
		ActionNeckTiltSideways, // param1 = degree
		ActionBodyRotate,       // param1 = degree
		ActionBodyTiltForward,  // param1 = degree
		ActionBodyTiltSideways, // param1 = degree
		ActionAudioPlay,        // param1 = track number
		ActionAudioStop,
		ActionSetAutomatic      // param1 = 0/1 - schaltet automatic-Mode auf Face/Neck/Body
	};

	Action action;
	int16_t param1;
	int16_t param2;
};

static_assert(sizeof(SequenceStep) == 7, "SequenceStep should pack to 7 bytes (for EEPROM-Blob)");

class HuyangSequence
{
public:
	HuyangSequence(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio);

	// Eigene Sequenz abspielen (Step-Array muss persistent bleiben waehrend Wiedergabe)
	void play(const SequenceStep *steps, size_t numSteps, const char *name = "");
	void stop();
	bool isPlaying();
	void loop();

	// Status fuer Web-UI
	float getProgress();        // 0..1
	const char *getName();      // aktuelle Sequenz-Name oder ""

	// Vordefinierte Sequenzen
	void playGreeting();    // Begruessung: Augen auf -> Kopf-Nick -> Audio
	void playSurprised();   // Erschrocken: weite Augen -> Audio
	void playSad();         // Traurig: Augen Sad -> Kopf nach unten -> Audio
	void playAngry();       // Wuetend: Augen Angry -> Body-Shake -> Audio
	void playSelfTest();    // Hardware-Test: Servos sweepen, Eyes durchgehen, Audio anspielen

	// Custom-Sequence: User-definierte Steps abspielen (Buffer wird intern kopiert)
	static const size_t CUSTOM_MAX_STEPS = 32;
	void playCustomSteps(const SequenceStep *steps, size_t count, const char *name = "custom");

	// Trigger-Helper, optional
	bool triggerByName(const String &name); // "greeting", "surprised", "sad", "angry", "selftest", "custom"

private:
	HuyangFace *_face;
	HuyangNeck *_neck;
	HuyangBody *_body;
	HuyangAudio *_audio;

	const SequenceStep *_steps = nullptr;
	size_t _numSteps = 0;
	size_t _currentStep = 0;
	unsigned long _startMs = 0;
	unsigned long _totalDurationMs = 0;
	bool _playing = false;
	char _currentName[24] = "";
	SequenceStep _customBuffer[CUSTOM_MAX_STEPS];

	void executeStep(const SequenceStep &step);
};

#endif
