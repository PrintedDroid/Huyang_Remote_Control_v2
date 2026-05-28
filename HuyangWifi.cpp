#include "HuyangWifi.h"
#include "config.h"

HuyangWifi::HuyangWifi(HuyangConfig *config)
{
	_config = config;
}

void HuyangWifi::setup()
{
	WiFi.disconnect(true);
	delay(100);

	if (_config->data.wifiMode == 1 && strlen(_config->data.wifiSSID) > 0)
	{
		// Try STA mode first
		connectSTA();

		if (!isConnected())
		{
			Serial.println(F("STA connection failed, falling back to AP mode"));
			startAP();
		}
	}
	else
	{
		startAP();
	}

	startMDNS();
}

void HuyangWifi::loop()
{
	unsigned long now = millis();

	if (!_isAP)
	{
		// STA-Mode: bei Disconnect periodisch reconnect versuchen
		bool connected = (WiFi.status() == WL_CONNECTED);

		if (!connected)
		{
			if (now - _lastReconnectAttempt > _reconnectInterval)
			{
				_lastReconnectAttempt = now;
				Serial.println(F("WiFi disconnected, attempting reconnect..."));
				WiFi.reconnect();
				_needsMdnsRestart = true;
			}
		}
		else if (_needsMdnsRestart)
		{
			// WiFi just reconnected - restart mDNS
			startMDNS();
			_needsMdnsRestart = false;
			Serial.print(F("WiFi reconnected! IP: "));
			Serial.println(WiFi.localIP());
		}
	}
	else
	{
		// AP-Mode: alle 60s testen ob das echte WLAN wieder da ist
		if (_config->data.wifiMode == 1 && strlen(_config->data.wifiSSID) > 0)
		{
			if (now - _lastStaRetryFromAp > _staRetryFromApInterval)
			{
				_lastStaRetryFromAp = now;
				Serial.println(F("AP-Mode: pruefe ob STA-WLAN wieder verfuegbar..."));

				if (tryConnectSTA(5))
				{
					// Erfolg! AP runterfahren, STA uebernimmt
					Serial.println(F("STA wieder verfuegbar - AP-Mode wird beendet"));
					stopAP();
					_isAP = false;
					startMDNS();
				}
				else
				{
					Serial.println(F("STA noch nicht da, bleibe im AP-Mode"));
				}
			}
		}
	}

#ifndef ESP32
	MDNS.update();
#endif
}

bool HuyangWifi::isConnected()
{
	if (_isAP)
	{
		return true; // AP is always "connected"
	}
	return WiFi.status() == WL_CONNECTED;
}

IPAddress HuyangWifi::getIP()
{
	if (_isAP)
	{
		return WiFi.softAPIP();
	}
	return WiFi.localIP();
}

String HuyangWifi::getMode()
{
	return _isAP ? "AP" : "STA";
}

void HuyangWifi::reconnect()
{
	Serial.println(F("Reconnecting WiFi..."));
	WiFi.disconnect(true);
	delay(100);

	if (_config->data.wifiMode == 1 && strlen(_config->data.wifiSSID) > 0)
	{
		connectSTA();
		if (!isConnected())
		{
			startAP();
		}
	}
	else
	{
		startAP();
	}

	startMDNS();
}

void HuyangWifi::connectSTA()
{
	// Wrapper fuer Abwaertskompatibilitaet - nutzt Default-Timeout 5s beim Boot
	tryConnectSTA(5);
}

bool HuyangWifi::tryConnectSTA(uint8_t timeoutSeconds)
{
	Serial.printf("Connecting to WiFi: %s ", _config->data.wifiSSID);

	WiFi.mode(WIFI_STA);
	WiFi.begin(_config->data.wifiSSID, _config->data.wifiPassword);

	// Wait up to timeoutSeconds for connection (500ms-Schritte)
	int iterations = timeoutSeconds * 2;
	while (WiFi.status() != WL_CONNECTED && iterations > 0)
	{
		delay(500);
		yield(); // Feed WDT and allow background tasks
		Serial.print(".");
		iterations--;
	}
	Serial.println();

	if (WiFi.status() == WL_CONNECTED)
	{
		_isAP = false;
		Serial.print(F("Connected! IP: "));
		Serial.println(WiFi.localIP());
		return true;
	}

	Serial.println(F("STA connection failed"));
	return false;
}

void HuyangWifi::stopAP()
{
	WiFi.softAPdisconnect(true);
	delay(50);
}

void HuyangWifi::startAP()
{
	_isAP = true;
	WiFi.mode(WIFI_AP);

	const char *apSSID = _config->data.apSSID;
	const char *apPass = _config->data.apPassword;

	// Configure AP IP BEFORE starting (required by ESP SDK)
	IPAddress apIP(192, 168, 10, 1);
	IPAddress subnet(255, 255, 255, 0);
	WiFi.softAPConfig(apIP, apIP, subnet);

	if (strlen(apPass) >= 8)
	{
		WiFi.softAP(apSSID, apPass);
	}
	else
	{
		if (strlen(apPass) > 0)
		{
			Serial.println(F("Warning: AP password too short (min 8 chars). Starting open AP."));
		}
		WiFi.softAP(apSSID); // Open network
	}

	Serial.printf("AP started: %s\n", apSSID);
	Serial.print(F("AP IP: "));
	Serial.println(WiFi.softAPIP());
}

void HuyangWifi::startMDNS()
{
	MDNS.end(); // Clean up previous instance to prevent memory leak

	if (MDNS.begin("huyang"))
	{
		MDNS.addService("http", "tcp", WebServerPort);
		Serial.println(F("mDNS: http://huyang.local"));
	}
	else
	{
		Serial.println(F("mDNS failed to start"));
	}
}
