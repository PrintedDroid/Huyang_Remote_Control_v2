#include "HuyangCLI.h"

HuyangCLI::HuyangCLI(HuyangConfig *config, HuyangWifi *wifi,
					  HuyangFace *face, HuyangNeck *neck,
					  HuyangBody *body, HuyangAudio *audio,
					  WebServer *webserver)
{
	_config = config;
	_wifi = wifi;
	_face = face;
	_neck = neck;
	_body = body;
	_audio = audio;
	_webserver = webserver;
}

void HuyangCLI::setup()
{
	_inputBuffer.reserve(CLI_BUFFER_SIZE);
	Serial.println(F("CLI ready. Type 'help' for commands."));
}

void HuyangCLI::loop()
{
	while (Serial.available())
	{
		char c = Serial.read();
		if (c == '\n' || c == '\r')
		{
			if (_inputBuffer.length() > 0)
			{
				_inputBuffer.trim();
				processCommand(_inputBuffer);
				_inputBuffer = "";
			}
		}
		else
		{
			if (_inputBuffer.length() < CLI_BUFFER_SIZE - 1)
			{
				_inputBuffer += c;
			}
		}
	}
}

HuyangCommand HuyangCLI::parseCommand(const String &cmd)
{
	if (cmd == "help") return CMD_HELP;
	if (cmd == "status") return CMD_STATUS;
	if (cmd == "reboot") return CMD_REBOOT;
	if (cmd == "config") return CMD_CONFIG;
	if (cmd == "save") return CMD_SAVE;
	if (cmd == "reset") return CMD_RESET;
	if (cmd == "wifi") return CMD_WIFI;
	if (cmd == "neck") return CMD_NECK;
	if (cmd == "body") return CMD_BODY;
	if (cmd == "eyes") return CMD_EYES;
	if (cmd == "audio") return CMD_AUDIO;
	if (cmd == "cal") return CMD_CAL;
	if (cmd == "auto") return CMD_AUTO;
	return CMD_UNKNOWN;
}

void HuyangCLI::processCommand(const String &fullCommand)
{
	String cmd = fullCommand;
	String params = "";

	int spaceIndex = fullCommand.indexOf(' ');
	if (spaceIndex > 0)
	{
		cmd = fullCommand.substring(0, spaceIndex);
		params = fullCommand.substring(spaceIndex + 1);
		params.trim();
	}

	cmd.toLowerCase();

	switch (parseCommand(cmd))
	{
	case CMD_HELP:
		showHelp();
		break;
	case CMD_STATUS:
		showStatus();
		break;
	case CMD_REBOOT:
		Serial.println(F("Rebooting..."));
		delay(500);
		ESP.restart();
		break;
	case CMD_CONFIG:
		_config->printConfig();
		break;
	case CMD_SAVE:
		_config->save();
		Serial.println(F("Configuration saved."));
		break;
	case CMD_RESET:
		if (params == "confirm")
		{
			Serial.println(F("Resetting to defaults..."));
			_config->resetToDefaults();
			Serial.println(F("Done. Reboot recommended."));
		}
		else
		{
			Serial.println(F("Type 'reset confirm' to reset config to defaults."));
		}
		break;
	case CMD_WIFI:
		handleWifiCommand(params);
		break;
	case CMD_NECK:
		handleNeckCommand(params);
		break;
	case CMD_BODY:
		handleBodyCommand(params);
		break;
	case CMD_EYES:
		handleEyesCommand(params);
		break;
	case CMD_AUDIO:
		handleAudioCommand(params);
		break;
	case CMD_CAL:
		handleCalCommand(params);
		break;
	case CMD_AUTO:
		handleAutoCommand(params);
		break;
	default:
		Serial.printf("Unknown command: %s\n", cmd.c_str());
		Serial.println(F("Type 'help' for available commands."));
		break;
	}
}

void HuyangCLI::showHelp()
{
	Serial.println(F("\n=== Huyang CLI Commands ==="));
	Serial.println(F("help                    - Show this help"));
	Serial.println(F("status                  - Show system status"));
	Serial.println(F("config                  - Show configuration"));
	Serial.println(F("save                    - Save config to EEPROM"));
	Serial.println(F("reset confirm           - Reset config to defaults"));
	Serial.println(F("reboot                  - Restart ESP"));
	Serial.println(F(""));
	Serial.println(F("wifi status             - Show WiFi status"));
	Serial.println(F("wifi ssid <name>        - Set WiFi SSID"));
	Serial.println(F("wifi password <pass>    - Set WiFi password"));
	Serial.println(F("wifi mode <ap|sta>      - Set WiFi mode"));
	Serial.println(F("wifi reconnect          - Reconnect WiFi"));
	Serial.println(F(""));
	Serial.println(F("neck rotate <-100..100> - Rotate head"));
	Serial.println(F("neck tilt <-100..100>   - Tilt neck forward"));
	Serial.println(F("neck sideways <-100..100> - Tilt neck sideways"));
	Serial.println(F(""));
	Serial.println(F("body rotate <-100..100> - Rotate body"));
	Serial.println(F("body tilt <-100..100>   - Tilt body forward"));
	Serial.println(F("body sideways <-100..100> - Tilt body sideways"));
	Serial.println(F(""));
	Serial.println(F("eyes open               - Open eyes"));
	Serial.println(F("eyes close              - Close eyes"));
	Serial.println(F("eyes blink              - Blink eyes"));
	Serial.println(F("eyes focus              - Focus eyes"));
	Serial.println(F("eyes sad                - Sad eyes"));
	Serial.println(F("eyes angry              - Angry eyes"));
	Serial.println(F("eyes auto               - Enable automatic eye animations"));
	Serial.println(F(""));
	Serial.println(F("audio volume <0..30>    - Set volume"));
	Serial.println(F("audio play <track>      - Play track number"));
	Serial.println(F("audio stop              - Stop playback"));
	Serial.println(F(""));
	Serial.println(F("cal neck rotation <val> - Set neck rotation offset"));
	Serial.println(F("cal neck tiltforward <val> - Set neck tilt offset"));
	Serial.println(F("cal neck tiltsideways <val> - Set neck sideways offset"));
	Serial.println(F("cal show                - Show calibration values"));
	Serial.println(F(""));
	Serial.println(F("auto on                 - Enable automatic animations"));
	Serial.println(F("auto off                - Disable automatic animations"));
	Serial.println(F("===========================\n"));
}

void HuyangCLI::showStatus()
{
	Serial.println(F("\n=== Huyang Status ==="));
	Serial.printf("WiFi Mode: %s\n", _wifi->getMode().c_str());
	Serial.printf("WiFi Connected: %s\n", _wifi->isConnected() ? "yes" : "no");
	Serial.print(F("IP: "));
	Serial.println(_wifi->getIP());
	Serial.printf("Automatic: %s\n", _webserver->automaticAnimations ? "on" : "off");
	Serial.printf("Free Heap: %u bytes\n", ESP.getFreeHeap());
	Serial.printf("Uptime: %lu s\n", millis() / 1000);
	Serial.println(F("====================\n"));
}

void HuyangCLI::handleWifiCommand(const String &params)
{
	if (params == "status" || params.length() == 0)
	{
		Serial.printf("Mode: %s\n", _wifi->getMode().c_str());
		Serial.printf("Connected: %s\n", _wifi->isConnected() ? "yes" : "no");
		Serial.print(F("IP: "));
		Serial.println(_wifi->getIP());
		Serial.printf("SSID: %s\n", _config->data.wifiSSID);
		Serial.printf("AP SSID: %s\n", _config->data.apSSID);
	}
	else if (params.startsWith("ssid "))
	{
		String ssid = params.substring(5);
		ssid.trim();
		strncpy(_config->data.wifiSSID, ssid.c_str(), sizeof(_config->data.wifiSSID) - 1);
		_config->data.wifiSSID[sizeof(_config->data.wifiSSID) - 1] = '\0';
		Serial.printf("WiFi SSID set to: %s\n", _config->data.wifiSSID);
		Serial.println(F("Use 'save' then 'wifi reconnect' to apply."));
	}
	else if (params.startsWith("password "))
	{
		String pass = params.substring(9);
		pass.trim();
		strncpy(_config->data.wifiPassword, pass.c_str(), sizeof(_config->data.wifiPassword) - 1);
		_config->data.wifiPassword[sizeof(_config->data.wifiPassword) - 1] = '\0';
		Serial.println(F("WiFi password updated."));
		Serial.println(F("Use 'save' then 'wifi reconnect' to apply."));
	}
	else if (params.startsWith("mode "))
	{
		String mode = params.substring(5);
		mode.trim();
		mode.toLowerCase();
		if (mode == "ap")
		{
			_config->data.wifiMode = 0;
			Serial.println(F("WiFi mode set to AP (hotspot)."));
		}
		else if (mode == "sta")
		{
			_config->data.wifiMode = 1;
			Serial.println(F("WiFi mode set to STA (connect to network)."));
		}
		else
		{
			Serial.println(F("Usage: wifi mode <ap|sta>"));
		}
		Serial.println(F("Use 'save' then 'wifi reconnect' to apply."));
	}
	else if (params == "reconnect")
	{
		_wifi->reconnect();
	}
	else
	{
		Serial.println(F("Usage: wifi [status|ssid|password|mode|reconnect]"));
	}
}

void HuyangCLI::handleNeckCommand(const String &params)
{
	if (params.startsWith("rotate "))
	{
		int val = constrain(params.substring(7).toInt(), -100, 100);
		_webserver->neckRotate = val;
		_webserver->automaticAnimations = false;
		if (_neck) _neck->rotateHead(val);
		Serial.printf("Neck rotate: %d\n", val);
	}
	else if (params.startsWith("tilt "))
	{
		int val = constrain(params.substring(5).toInt(), -100, 100);
		_webserver->neckTiltForward = val;
		_webserver->automaticAnimations = false;
		if (_neck) _neck->tiltNeckForward(val);
		Serial.printf("Neck tilt forward: %d\n", val);
	}
	else if (params.startsWith("sideways "))
	{
		int val = constrain(params.substring(9).toInt(), -100, 100);
		_webserver->neckTiltSideways = val;
		_webserver->automaticAnimations = false;
		if (_neck) _neck->tiltNeckSideways(val);
		Serial.printf("Neck tilt sideways: %d\n", val);
	}
	else
	{
		Serial.println(F("Usage: neck [rotate|tilt|sideways] <-100..100>"));
	}
}

void HuyangCLI::handleBodyCommand(const String &params)
{
	if (params.startsWith("rotate "))
	{
		int val = constrain(params.substring(7).toInt(), -100, 100);
		_webserver->bodyRotate = val;
		_webserver->automaticAnimations = false;
		if (_body) _body->rotateBody(val);
		Serial.printf("Body rotate: %d\n", val);
	}
	else if (params.startsWith("tilt "))
	{
		int val = constrain(params.substring(5).toInt(), -100, 100);
		_webserver->bodyTiltForward = val;
		_webserver->automaticAnimations = false;
		if (_body) _body->tiltBodyForward(val);
		Serial.printf("Body tilt forward: %d\n", val);
	}
	else if (params.startsWith("sideways "))
	{
		int val = constrain(params.substring(9).toInt(), -100, 100);
		_webserver->bodyTiltSideways = val;
		_webserver->automaticAnimations = false;
		if (_body) _body->tiltBodySideways(val);
		Serial.printf("Body tilt sideways: %d\n", val);
	}
	else
	{
		Serial.println(F("Usage: body [rotate|tilt|sideways] <-100..100>"));
	}
}

void HuyangCLI::handleEyesCommand(const String &params)
{
	String p = params;
	p.toLowerCase();
	p.trim();

	_webserver->automaticAnimations = false;

	if (p == "open")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Open);
		_webserver->allEyes = HuyangFace::EyeState::Open;
		Serial.println(F("Eyes: open"));
	}
	else if (p == "close" || p == "closed")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Closed);
		_webserver->allEyes = HuyangFace::EyeState::Closed;
		Serial.println(F("Eyes: closed"));
	}
	else if (p == "blink")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Blink);
		_webserver->allEyes = HuyangFace::EyeState::Blink;
		Serial.println(F("Eyes: blink"));
	}
	else if (p == "focus")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Focus);
		_webserver->allEyes = HuyangFace::EyeState::Focus;
		Serial.println(F("Eyes: focus"));
	}
	else if (p == "sad")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Sad);
		_webserver->allEyes = HuyangFace::EyeState::Sad;
		Serial.println(F("Eyes: sad"));
	}
	else if (p == "angry")
	{
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Angry);
		_webserver->allEyes = HuyangFace::EyeState::Angry;
		Serial.println(F("Eyes: angry"));
	}
	else if (p == "auto")
	{
		_webserver->automaticAnimations = true;
		_webserver->allEyes = 0;
		_webserver->leftEye = 0;
		_webserver->rightEye = 0;
		Serial.println(F("Eyes: automatic"));
	}
	else
	{
		Serial.println(F("Usage: eyes [open|close|blink|focus|sad|angry|auto]"));
	}
}

void HuyangCLI::handleAudioCommand(const String &params)
{
	if (params.startsWith("volume "))
	{
		int vol = constrain(params.substring(7).toInt(), 0, 30);
		_config->data.volume = vol;
		if (_audio) _audio->setVolume(vol);
		Serial.printf("Volume set to: %d\n", vol);
	}
	else if (params.startsWith("play "))
	{
		int track = params.substring(5).toInt();
		if (_audio) _audio->playTrack(track);
		Serial.printf("Play track: %d\n", track);
	}
	else if (params == "stop")
	{
		if (_audio) _audio->stop();
		Serial.println(F("Audio stopped."));
	}
	else
	{
		Serial.println(F("Usage: audio [volume <0..30>|play <track>|stop]"));
	}
}

void HuyangCLI::handleCalCommand(const String &params)
{
	if (params == "show" || params.length() == 0)
	{
		Serial.println(F("\n=== Calibration ==="));
		Serial.printf("Neck Rotation: %d\n", _config->data.calNeckRotation);
		Serial.printf("Neck Tilt Forward: %d\n", _config->data.calNeckTiltForward);
		Serial.printf("Neck Tilt Sideways: %d\n", _config->data.calNeckTiltSideways);
		Serial.println(F("===================\n"));
	}
	else if (params.startsWith("neck rotation "))
	{
		int val = constrain(params.substring(14).toInt(), -127, 127);
		_config->data.calNeckRotation = val;
		Serial.printf("Neck rotation offset: %d\n", val);
		Serial.println(F("Use 'save' to persist."));
	}
	else if (params.startsWith("neck tiltforward "))
	{
		int val = constrain(params.substring(17).toInt(), -127, 127);
		_config->data.calNeckTiltForward = val;
		Serial.printf("Neck tilt forward offset: %d\n", val);
		Serial.println(F("Use 'save' to persist."));
	}
	else if (params.startsWith("neck tiltsideways "))
	{
		int val = constrain(params.substring(18).toInt(), -127, 127);
		_config->data.calNeckTiltSideways = val;
		Serial.printf("Neck tilt sideways offset: %d\n", val);
		Serial.println(F("Use 'save' to persist."));
	}
	else
	{
		Serial.println(F("Usage: cal [show|neck rotation|neck tiltforward|neck tiltsideways] <value>"));
	}
}

void HuyangCLI::handleAutoCommand(const String &params)
{
	String p = params;
	p.toLowerCase();
	p.trim();

	if (p == "on")
	{
		_webserver->automaticAnimations = true;
		_webserver->allEyes = 0;
		_webserver->leftEye = 0;
		_webserver->rightEye = 0;
		Serial.println(F("Automatic animations: ON"));
	}
	else if (p == "off")
	{
		_webserver->automaticAnimations = false;
		Serial.println(F("Automatic animations: OFF"));
	}
	else
	{
		Serial.println(F("Usage: auto [on|off]"));
	}
}
