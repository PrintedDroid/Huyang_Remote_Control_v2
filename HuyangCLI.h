#ifndef HuyangCLI_h
#define HuyangCLI_h

#include <Arduino.h>
#include "HuyangConfig.h"

#define CLI_BUFFER_SIZE 128
#include "HuyangWifi.h"
#include "HuyangFace.h"
#include "HuyangNeck.h"
#include "HuyangBody.h"
#include "HuyangAudio.h"
#include "WebServer.h"

enum HuyangCommand
{
	CMD_UNKNOWN,
	CMD_HELP,
	CMD_STATUS,
	CMD_REBOOT,
	CMD_CONFIG,
	CMD_SAVE,
	CMD_RESET,
	CMD_WIFI,
	CMD_NECK,
	CMD_BODY,
	CMD_EYES,
	CMD_AUDIO,
	CMD_CAL,
	CMD_AUTO
};

class HuyangCLI
{
public:
	HuyangCLI(HuyangConfig *config, HuyangWifi *wifi,
			  HuyangFace *face, HuyangNeck *neck,
			  HuyangBody *body, HuyangAudio *audio,
			  WebServer *webserver);
	void setup();
	void loop();

private:
	HuyangConfig *_config;
	HuyangWifi *_wifi;
	HuyangFace *_face;
	HuyangNeck *_neck;
	HuyangBody *_body;
	HuyangAudio *_audio;
	WebServer *_webserver;

	String _inputBuffer;

	HuyangCommand parseCommand(const String &cmd);
	void processCommand(const String &fullCommand);

	void showHelp();
	void showStatus();

	void handleWifiCommand(const String &params);
	void handleNeckCommand(const String &params);
	void handleBodyCommand(const String &params);
	void handleEyesCommand(const String &params);
	void handleAudioCommand(const String &params);
	void handleCalCommand(const String &params);
	void handleAutoCommand(const String &params);
};

#endif
