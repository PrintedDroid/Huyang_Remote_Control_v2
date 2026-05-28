#include "HuyangSequence.h"

// =========================================================================
// Vordefinierte Sequenzen - als static const Arrays im Flash (PROGMEM-fae
// hig, aber hier einfach im RODATA - Sequenzen sind klein)
// =========================================================================

static const SequenceStep SEQ_GREETING[] = {
	{   0, SequenceStep::ActionSetAutomatic, 0, 0 },
	{   0, SequenceStep::ActionEyeState,     HuyangFace::Open, 0 },
	{ 200, SequenceStep::ActionNeckRotate,   0, 800 },        // Kopf zur Mitte
	{ 400, SequenceStep::ActionAudioPlay,    1, 0 },          // Track 1
	{ 600, SequenceStep::ActionNeckTiltForward, 30, 400 },    // leichtes Nicken hoch
	{ 1000, SequenceStep::ActionNeckTiltForward, -20, 400 },  // dann runter
	{ 1400, SequenceStep::ActionNeckTiltForward, 0, 400 },    // Mitte
	{ 1800, SequenceStep::ActionSetAutomatic, 1, 0 },         // zurueck zu Auto-Mode
};

static const SequenceStep SEQ_SURPRISED[] = {
	{   0, SequenceStep::ActionSetAutomatic, 0, 0 },
	{   0, SequenceStep::ActionEyeState,     HuyangFace::Open, 0 },
	{   0, SequenceStep::ActionNeckTiltForward, 40, 200 },    // Kopf hoch (erschrocken)
	{ 100, SequenceStep::ActionAudioPlay,    2, 0 },
	{ 800, SequenceStep::ActionEyeState,     HuyangFace::Blink, 0 },
	{ 1200, SequenceStep::ActionNeckTiltForward, 0, 600 },
	{ 1800, SequenceStep::ActionSetAutomatic, 1, 0 },
};

static const SequenceStep SEQ_SAD[] = {
	{   0, SequenceStep::ActionSetAutomatic, 0, 0 },
	{   0, SequenceStep::ActionEyeState,     HuyangFace::Sad, 0 },
	{ 200, SequenceStep::ActionNeckTiltForward, -50, 1000 },  // Kopf nach unten
	{ 500, SequenceStep::ActionAudioPlay,    3, 0 },
	{ 3000, SequenceStep::ActionNeckTiltForward, 0, 800 },
	{ 3800, SequenceStep::ActionEyeState,    HuyangFace::Open, 0 },
	{ 4000, SequenceStep::ActionSetAutomatic, 1, 0 },
};

static const SequenceStep SEQ_SELFTEST[] = {
	{    0, SequenceStep::ActionSetAutomatic,    0, 0 },
	{    0, SequenceStep::ActionEyeState,        HuyangFace::Open, 0 },
	{    0, SequenceStep::ActionAudioPlay,       1, 0 },
	// Neck Test
	{  500, SequenceStep::ActionNeckRotate,      -100, 1500 },
	{ 2200, SequenceStep::ActionNeckRotate,      100, 1500 },
	{ 4000, SequenceStep::ActionNeckRotate,      0, 1000 },
	{ 5000, SequenceStep::ActionNeckTiltForward, 100, 1000 },
	{ 6000, SequenceStep::ActionNeckTiltForward, -100, 1000 },
	{ 7000, SequenceStep::ActionNeckTiltForward, 0, 800 },
	{ 7800, SequenceStep::ActionNeckTiltSideways, 80, 0 },
	{ 8500, SequenceStep::ActionNeckTiltSideways, -80, 0 },
	{ 9200, SequenceStep::ActionNeckTiltSideways, 0, 0 },
	// Body Test
	{ 9500, SequenceStep::ActionBodyRotate,      -100, 0 },
	{ 11000, SequenceStep::ActionBodyRotate,     100, 0 },
	{ 12500, SequenceStep::ActionBodyRotate,     0, 0 },
	{ 13000, SequenceStep::ActionBodyTiltForward,  100, 0 },
	{ 14500, SequenceStep::ActionBodyTiltForward,  -100, 0 },
	{ 16000, SequenceStep::ActionBodyTiltForward,  0, 0 },
	// Eye-States durchgehen
	{ 17000, SequenceStep::ActionEyeState,       HuyangFace::Sad, 0 },
	{ 18500, SequenceStep::ActionEyeState,       HuyangFace::Angry, 0 },
	{ 20000, SequenceStep::ActionEyeState,       HuyangFace::Focus, 0 },
	{ 21500, SequenceStep::ActionEyeState,       HuyangFace::Open, 0 },
	{ 22500, SequenceStep::ActionAudioStop,      0, 0 },
	{ 22500, SequenceStep::ActionSetAutomatic,   1, 0 },
};

static const SequenceStep SEQ_ANGRY[] = {
	{   0, SequenceStep::ActionSetAutomatic, 0, 0 },
	{   0, SequenceStep::ActionEyeState,     HuyangFace::Angry, 0 },
	{   0, SequenceStep::ActionBodyTiltSideways, -30, 0 },
	{ 200, SequenceStep::ActionBodyTiltSideways, 30, 0 },
	{ 400, SequenceStep::ActionBodyTiltSideways, -30, 0 },
	{ 600, SequenceStep::ActionAudioPlay,    4, 0 },
	{ 600, SequenceStep::ActionBodyTiltSideways, 0, 0 },
	{ 1500, SequenceStep::ActionEyeState,    HuyangFace::Open, 0 },
	{ 1800, SequenceStep::ActionSetAutomatic, 1, 0 },
};

// =========================================================================
// Klasse
// =========================================================================

HuyangSequence::HuyangSequence(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio)
	: _face(face), _neck(neck), _body(body), _audio(audio)
{
}

void HuyangSequence::play(const SequenceStep *steps, size_t numSteps, const char *name)
{
	_steps = steps;
	_numSteps = numSteps;
	_currentStep = 0;
	_startMs = millis();
	_playing = true;
	_totalDurationMs = (numSteps > 0) ? steps[numSteps - 1].atMs : 0;
	if (name) strncpy(_currentName, name, sizeof(_currentName) - 1);
	else _currentName[0] = 0;
	_currentName[sizeof(_currentName) - 1] = 0;
	Serial.print(F("Sequence start, "));
	Serial.print(numSteps);
	Serial.println(F(" steps"));
}

void HuyangSequence::stop()
{
	_playing = false;
	_steps = nullptr;
	_numSteps = 0;
	_currentStep = 0;
	_totalDurationMs = 0;
	_currentName[0] = 0;
}

bool HuyangSequence::isPlaying()
{
	return _playing;
}

float HuyangSequence::getProgress()
{
	if (!_playing || _totalDurationMs == 0) return 0.0f;
	unsigned long elapsed = millis() - _startMs;
	if (elapsed >= _totalDurationMs) return 1.0f;
	return (float)elapsed / (float)_totalDurationMs;
}

const char *HuyangSequence::getName()
{
	return _currentName;
}

void HuyangSequence::loop()
{
	if (!_playing || _steps == nullptr) return;

	unsigned long elapsed = millis() - _startMs;

	// Alle Steps abarbeiten, deren Zeitpunkt erreicht ist
	while (_currentStep < _numSteps && _steps[_currentStep].atMs <= elapsed)
	{
		executeStep(_steps[_currentStep]);
		_currentStep++;
	}

	// Sequenz zu Ende?
	if (_currentStep >= _numSteps)
	{
		_playing = false;
		Serial.println(F("Sequence done"));
	}
}

void HuyangSequence::executeStep(const SequenceStep &step)
{
	switch (step.action)
	{
	case SequenceStep::ActionEyeState:
		if (_face) _face->setEyesTo((HuyangFace::EyeState)step.param1);
		break;
	case SequenceStep::ActionNeckRotate:
		if (_neck) _neck->rotateHead((double)step.param1, (double)(step.param2 > 0 ? step.param2 : 1000));
		break;
	case SequenceStep::ActionNeckTiltForward:
		if (_neck) _neck->tiltNeckForward((double)step.param1, (double)(step.param2 > 0 ? step.param2 : 1000));
		break;
	case SequenceStep::ActionNeckTiltSideways:
		if (_neck) _neck->tiltNeckSideways((double)step.param1);
		break;
	case SequenceStep::ActionBodyRotate:
		if (_body) _body->rotateBody(step.param1);
		break;
	case SequenceStep::ActionBodyTiltForward:
		if (_body) _body->tiltBodyForward(step.param1);
		break;
	case SequenceStep::ActionBodyTiltSideways:
		if (_body) _body->tiltBodySideways(step.param1);
		break;
	case SequenceStep::ActionAudioPlay:
		if (_audio) _audio->playTrack(step.param1);
		break;
	case SequenceStep::ActionAudioStop:
		if (_audio) _audio->stop();
		break;
	case SequenceStep::ActionSetAutomatic:
	{
		bool on = (step.param1 != 0);
		if (_face) _face->automatic = on;
		if (_neck) _neck->automatic = on;
		if (_body) _body->automatic = on;
		break;
	}
	case SequenceStep::ActionNone:
	default:
		break;
	}
}

void HuyangSequence::playGreeting()  { play(SEQ_GREETING,  sizeof(SEQ_GREETING)  / sizeof(SequenceStep), "greeting"); }
void HuyangSequence::playSurprised() { play(SEQ_SURPRISED, sizeof(SEQ_SURPRISED) / sizeof(SequenceStep), "surprised"); }
void HuyangSequence::playSad()       { play(SEQ_SAD,       sizeof(SEQ_SAD)       / sizeof(SequenceStep), "sad"); }
void HuyangSequence::playAngry()     { play(SEQ_ANGRY,     sizeof(SEQ_ANGRY)     / sizeof(SequenceStep), "angry"); }
void HuyangSequence::playSelfTest()  { play(SEQ_SELFTEST,  sizeof(SEQ_SELFTEST)  / sizeof(SequenceStep), "selftest"); }

void HuyangSequence::playCustomSteps(const SequenceStep *steps, size_t count, const char *name)
{
	if (count > CUSTOM_MAX_STEPS) count = CUSTOM_MAX_STEPS;
	for (size_t i = 0; i < count; i++) _customBuffer[i] = steps[i];
	play(_customBuffer, count, name);
}

bool HuyangSequence::triggerByName(const String &name)
{
	if (name == "greeting")  { playGreeting();  return true; }
	if (name == "surprised") { playSurprised(); return true; }
	if (name == "sad")       { playSad();       return true; }
	if (name == "angry")     { playAngry();     return true; }
	if (name == "selftest" || name == "test") { playSelfTest(); return true; }
	return false;
}
