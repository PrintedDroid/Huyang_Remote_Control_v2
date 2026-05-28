#ifndef HuyangSequence_h
#define HuyangSequence_h

#include <Arduino.h>
#include "HuyangFace.h"
#include "HuyangNeck.h"
#include "HuyangBody.h"
#include "HuyangAudio.h"

// A sequence is a list of time-scheduled actions.
// Each step has a timestamp (offset in ms from sequence start) and an action.
// This enables coordinated performances (eyes + motion + audio in sync).

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
		ActionSetAutomatic      // param1 = 0/1 - toggles automatic mode on Face/Neck/Body
	};

	Action action;
	int16_t param1;
	int16_t param2;
};

static_assert(sizeof(SequenceStep) == 7, "SequenceStep should pack to 7 bytes (for EEPROM blob)");

class HuyangSequence
{
public:
	HuyangSequence(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio);

	// Play a sequence (the step array must remain valid during playback)
	void play(const SequenceStep *steps, size_t numSteps, const char *name = "");
	void stop();
	bool isPlaying();
	void loop();

	// Status for the web UI
	float getProgress();        // 0..1
	const char *getName();      // current sequence name or ""

	// Pre-defined sequences
	void playGreeting();    // Greeting: eyes open -> head nod -> audio
	void playSurprised();   // Surprised: wide eyes -> audio
	void playSad();         // Sad: eyes Sad -> head down -> audio
	void playAngry();       // Angry: eyes Angry -> body shake -> audio
	void playSelfTest();    // Hardware test: sweep servos, cycle eye states, play audio

	// Custom sequence: play user-defined steps (buffer is copied internally)
	static const size_t CUSTOM_MAX_STEPS = 32;
	void playCustomSteps(const SequenceStep *steps, size_t count, const char *name = "custom");

	// Trigger helper, optional
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
