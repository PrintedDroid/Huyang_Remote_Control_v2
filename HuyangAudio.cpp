#include "HuyangAudio.h"
#include "pins.h"

void printDetail(uint8_t type, int value);

#ifdef ESP32
HuyangAudio::HuyangAudio() : _audioSerial(1)
{
}
#else
HuyangAudio::HuyangAudio()
{
}
#endif

void HuyangAudio::setup()
{
	Serial.println(F("HuyangAudio setup"));

#ifdef ESP32
	_audioSerial.begin(9600, SERIAL_8N1, PIN_AUDIO_RX, PIN_AUDIO_TX);
	_isSerialReady = true;
#else
	_audioSerial.begin(9600, SWSERIAL_8N1, PIN_AUDIO_RX, PIN_AUDIO_TX);
	if (!_audioSerial)
	{
		Serial.println(F("SoftwareSerial is not working, check config"));
		return; // Don't block forever - continue without audio
	}
	_isSerialReady = true;
#endif

	if (!_player.begin(_audioSerial, /*isACK = */ false, /*doReset = */ false))
	{
		Serial.println(F("Connecting to DFPlayer Mini failed!"));
	}
	else
	{
		_isPlayerReady = true;
		_player.volume(25);
		Serial.println(F("DFPlayer ready"));
	}
}

void HuyangAudio::playTrack(int track)
{
	if (_isPlayerReady)
	{
		Serial.printf("Audio: play track %d\n", track);
		_player.play(track);
	}
	else
	{
		Serial.println(F("Audio: player not ready"));
	}
}

void HuyangAudio::setVolume(uint8_t vol)
{
	if (_isPlayerReady)
	{
		_player.volume(vol);
		Serial.printf("Audio: volume %d\n", vol);
	}
}

void HuyangAudio::stop()
{
	if (_isPlayerReady)
	{
		_player.stop();
	}
}

void HuyangAudio::loop()
{
	if (!_isSerialReady || !_isPlayerReady) return;

	unsigned long now = millis();

	if (now - _previousMillis >= _audioPause)
	{
		if (_player.available())
		{
			printDetail(_player.readType(), _player.read());

			_previousMillis = now;

			if (_audioItemCount == 0)
			{
				int count = _player.readFileCounts();

				if (count > 0)
				{
					_audioItemCount = count;
					Serial.printf("Found %d mp3 files\n", count);
				}
				else
				{
					_audioItemCount = 1;
				}
			}

			if (_audioItemCount > 0)
			{
				uint16_t randomItemNumber = random(1, _audioItemCount + 1);

				if (randomItemNumber == 8)
				{
					randomItemNumber = (_audioItemCount > 8) ? 9 : 1;
				}

				Serial.printf("Play item %d of %d\n", randomItemNumber, _audioItemCount);

				_player.play(randomItemNumber);

				_audioPause = 2000 + (random(10, 50) * 100);
			}
		}
	}
}

void printDetail(uint8_t type, int value)
{
	switch (type)
	{
	case TimeOut:
		Serial.println(F("Time Out!"));
		break;
	case WrongStack:
		Serial.println(F("Stack Wrong!"));
		break;
	case DFPlayerCardInserted:
		Serial.println(F("Card Inserted!"));
		break;
	case DFPlayerCardRemoved:
		Serial.println(F("Card Removed!"));
		break;
	case DFPlayerCardOnline:
		Serial.println(F("Card Online!"));
		break;
	case DFPlayerUSBInserted:
		Serial.println(F("USB Inserted!"));
		break;
	case DFPlayerUSBRemoved:
		Serial.println(F("USB Removed!"));
		break;
	case DFPlayerPlayFinished:
		Serial.print(F("Number:"));
		Serial.print(value);
		Serial.println(F(" Play Finished!"));
		break;
	case DFPlayerError:
		Serial.print(F("DFPlayerError:"));
		switch (value)
		{
		case Busy:
			Serial.println(F("Card not found"));
			break;
		case Sleeping:
			Serial.println(F("Sleeping"));
			break;
		case SerialWrongStack:
			Serial.println(F("Get Wrong Stack"));
			break;
		case CheckSumNotMatch:
			Serial.println(F("Check Sum Not Match"));
			break;
		case FileIndexOut:
			Serial.println(F("File Index Out of Bound"));
			break;
		case FileMismatch:
			Serial.println(F("Cannot Find File"));
			break;
		case Advertise:
			Serial.println(F("In Advertise"));
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}
