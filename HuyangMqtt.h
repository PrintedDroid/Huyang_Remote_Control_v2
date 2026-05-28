#ifndef HuyangMqtt_h
#define HuyangMqtt_h

#include <Arduino.h>   // muss IMMER includiert sein - der Stub-Branch nutzt uint16_t
#include "config.h"

// Wenn MQTT global deaktiviert: Stub-Klasse die nichts macht.
// So kompiliert der Sketch auch ohne PubSubClient-Library.

#if HUYANG_MQTT_ENABLED

#ifdef ESP32
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <PubSubClient.h>

#include "HuyangFace.h"
#include "HuyangNeck.h"
#include "HuyangBody.h"
#include "HuyangAudio.h"
#include "HuyangSequence.h"

class HuyangMqtt
{
public:
	HuyangMqtt(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio, HuyangSequence *seq);

	void setup(const char *host, uint16_t port, const char *user, const char *pass, const char *baseTopic);
	void loop();

	bool isConnected();
	void publishState(const char *subtopic, const char *value);
	void publishDiscovery();

private:
	WiFiClient _net;
	PubSubClient _mqtt;
	HuyangFace *_face;
	HuyangNeck *_neck;
	HuyangBody *_body;
	HuyangAudio *_audio;
	HuyangSequence *_seq;

	String _host;
	uint16_t _port = 1883;
	String _user;
	String _pass;
	String _baseTopic;
	String _clientId;

	unsigned long _lastReconnect = 0;

	void reconnect();
	void handleMessage(char *topic, byte *payload, unsigned int length);
};

#else // HUYANG_MQTT_ENABLED == false

// Stub - existiert nur damit der Aufrufer-Code kompiliert
class HuyangFace; class HuyangNeck; class HuyangBody; class HuyangAudio; class HuyangSequence;

class HuyangMqtt
{
public:
	HuyangMqtt(HuyangFace *, HuyangNeck *, HuyangBody *, HuyangAudio *, HuyangSequence *) {}
	void setup(const char *, uint16_t, const char *, const char *, const char *) {}
	void loop() {}
	bool isConnected() { return false; }
	void publishState(const char *, const char *) {}
	void publishDiscovery() {}
};

#endif // HUYANG_MQTT_ENABLED

#endif
