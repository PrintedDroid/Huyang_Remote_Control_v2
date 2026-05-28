#ifndef HuyangAudio_h
#define HuyangAudio_h

#include <Arduino.h>
#include "DFRobotDFPlayerMini.h"

#ifdef ESP32
#include <HardwareSerial.h>
#else
#include "SoftwareSerial.h"
#endif

class HuyangAudio
{
public:
	HuyangAudio();
	void setup();
	void loop();

	void playTrack(int track);
	void setVolume(uint8_t vol);
	void stop();

private:
	unsigned long _previousMillis = 0;

	bool _isSerialReady = false;
	bool _isPlayerReady = false;

	DFRobotDFPlayerMini _player;

#ifdef ESP32
	HardwareSerial _audioSerial;
#else
	SoftwareSerial _audioSerial;
#endif

	uint16_t _audioPause = 2000;
	uint16_t _audioItemCount = 0;
};

#endif
