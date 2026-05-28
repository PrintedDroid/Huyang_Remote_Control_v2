
#ifndef HuyangWebServer_h
#define HuyangWebServer_h

#include <ArduinoJson.h>

#ifdef ESP32
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#else
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

// No LittleFS needed: UI assets live in WebAssets.h (PROGMEM),
// custom sequence lives in HuyangConfig (EEPROM)

#include "HuyangFace.h"
#include "HuyangNeck.h"
#include "HuyangBody.h"
#include "HuyangAudio.h"
#include "HuyangSequence.h"

// Forward declarations - avoid circular includes with HuyangWifi/HuyangMqtt/HuyangConfig
class HuyangWifi;
class HuyangMqtt;
class HuyangConfig;

class WebServer
{
public:
	enum Page
	{
		indexPage,
		settingsPage,
		calibrationPage,
		apiPage
	};

	bool automaticAnimations = true;

	uint8_t leftEye = 0;
	uint8_t rightEye = 0;
	uint8_t allEyes = 0;

	int16_t neckRotate = 0;
	int16_t neckTiltForward = 0;
	int16_t neckTiltSideways = 0;

	int16_t bodyRotate = 0;
	int16_t bodyTiltForward = 0;
	int16_t bodyTiltSideways = 0;

	WebServer(uint32_t port);
	void setup(bool enableEyes,
			   bool enableMonacle,
			   bool enableNeckMovement,
			   bool enableHeadRotation,
			   bool enableBodyMovement,
			   bool enableBodyRotation,
			   bool enableTorsoLights);
	void start();

	void setComponents(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio);
	void setSequence(HuyangSequence *sequence) { _sequence = sequence; }
	void setStatusSources(HuyangWifi *wifi, HuyangMqtt *mqtt) { _wifi = wifi; _mqtt = mqtt; }
	void setConfig(HuyangConfig *config) { _config = config; }

	// HTTP Basic Auth fuer Web-Interface
	void setAuth(bool enabled, const char *user, const char *pass);

private:
	AsyncWebServer *_server;

	HuyangFace *_face = nullptr;
	HuyangNeck *_neck = nullptr;
	HuyangBody *_body = nullptr;
	HuyangAudio *_audio = nullptr;
	HuyangSequence *_sequence = nullptr;
	HuyangWifi *_wifi = nullptr;
	HuyangMqtt *_mqtt = nullptr;
	HuyangConfig *_config = nullptr;

	bool _enableEyes;
	bool _enableMonacle;
	bool _enableNeckMovement;
	bool _enableHeadRotation;
	bool _enableBodyMovement;
	bool _enableBodyRotation;
	bool _enableTorsoLights;

	// Auth
	bool _authEnabled = false;
	String _authUser;
	String _authPass;
	bool checkAuth(AsyncWebServerRequest *request);
	bool checkSameOrigin(AsyncWebServerRequest *request); // CSRF: only enforced when _authEnabled

	String _postBodyBuffer;

	void getBaseHtml(const String &body, String &target);
	String getPage(Page page, AsyncWebServerRequest *request);

	void apiPostAction(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);

	void setupTriggerRoutes();
	void sendTriggerResponse(AsyncWebServerRequest *request, const char *action);

	void notFound(AsyncWebServerRequest *request);
};

#endif
