#ifndef HuyangWifi_h
#define HuyangWifi_h

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#else
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#endif

#include "HuyangConfig.h"

class HuyangWifi
{
public:
	HuyangWifi(HuyangConfig *config);
	void setup();
	void loop();

	bool isConnected();
	IPAddress getIP();
	String getMode();

	void reconnect();

private:
	HuyangConfig *_config;

	bool _isAP = false;
	bool _needsMdnsRestart = false;
	unsigned long _lastReconnectAttempt = 0;
	unsigned long _reconnectInterval = 30000; // 30s between reconnect attempts in STA mode
	unsigned long _lastStaRetryFromAp = 0;
	unsigned long _staRetryFromApInterval = 60000; // 60s between STA retry attempts from AP mode

	void connectSTA();
	bool tryConnectSTA(uint8_t timeoutSeconds); // non-blocking-friendly variant, returns true on success
	void startAP();
	void stopAP();
	void startMDNS();
};

#endif
