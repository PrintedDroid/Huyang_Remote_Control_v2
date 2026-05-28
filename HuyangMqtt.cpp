#include "HuyangMqtt.h"

#if HUYANG_MQTT_ENABLED

HuyangMqtt::HuyangMqtt(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio, HuyangSequence *seq)
	: _mqtt(_net), _face(face), _neck(neck), _body(body), _audio(audio), _seq(seq)
{
}

void HuyangMqtt::setup(const char *host, uint16_t port, const char *user, const char *pass, const char *baseTopic)
{
	_host = host;
	_port = port;
	_user = user;
	_pass = pass;
	_baseTopic = baseTopic;

	String mac = WiFi.macAddress();
	mac.replace(":", "");
	_clientId = _baseTopic + "-" + mac.substring(mac.length() - 6);

	_mqtt.setServer(_host.c_str(), _port);
	_mqtt.setCallback([this](char *topic, byte *payload, unsigned int length) {
		handleMessage(topic, payload, length);
	});

	Serial.print(F("MQTT Setup: "));
	Serial.print(_host);
	Serial.print(F(":"));
	Serial.print(_port);
	Serial.print(F(" als "));
	Serial.println(_clientId);
}

void HuyangMqtt::loop()
{
	if (WiFi.status() != WL_CONNECTED) return;

	if (!_mqtt.connected())
	{
		unsigned long now = millis();
		if (now - _lastReconnect > 5000)
		{
			_lastReconnect = now;
			reconnect();
		}
	}
	else
	{
		_mqtt.loop();
	}
}

bool HuyangMqtt::isConnected()
{
	return _mqtt.connected();
}

void HuyangMqtt::reconnect()
{
	Serial.print(F("MQTT connect..."));
	bool ok;
	if (_user.length() > 0)
	{
		ok = _mqtt.connect(_clientId.c_str(), _user.c_str(), _pass.c_str());
	}
	else
	{
		ok = _mqtt.connect(_clientId.c_str());
	}

	if (ok)
	{
		Serial.println(F(" connected"));
		// Subscribe alle Command-Topics
		String sub = _baseTopic + "/cmd/#";
		_mqtt.subscribe(sub.c_str());
		// Online-Status
		publishState("status", "online");
		// Home-Assistant-Discovery publishen
		publishDiscovery();
	}
	else
	{
		Serial.print(F(" connect failed, rc="));
		Serial.println(_mqtt.state());
	}
}

void HuyangMqtt::publishState(const char *subtopic, const char *value)
{
	if (!_mqtt.connected()) return;
	String topic = _baseTopic + "/state/" + subtopic;
	_mqtt.publish(topic.c_str(), value);
}

void HuyangMqtt::publishDiscovery()
{
	if (!_mqtt.connected()) return;

	// Device-Block - identifiziert alle Entities als zugehoerig zum gleichen Geraet
	String dev = "\"device\":{\"identifiers\":[\"huyang\"],\"name\":\"Huyang Droid\","
				 "\"manufacturer\":\"Printed-Droid\",\"model\":\"Huyang v2.7\"}";

	auto pub = [&](const char *component, const char *objId, const String &payload) {
		String topic = "homeassistant/";
		topic += component; topic += "/huyang/"; topic += objId; topic += "/config";
		_mqtt.publish(topic.c_str(), payload.c_str(), true); // retained
	};

	// 4 Sequence-Buttons + Selftest
	const char *seqs[] = { "greeting", "surprised", "sad", "angry", "selftest" };
	for (int i = 0; i < 5; i++) {
		String p = "{\"name\":\"Huyang ";
		p += seqs[i];
		p += "\",\"command_topic\":\"";
		p += _baseTopic + "/cmd/sequence\",\"payload_press\":\"";
		p += seqs[i];
		p += "\",\"unique_id\":\"huyang_seq_";
		p += seqs[i];
		p += "\",";
		p += dev + "}";
		String objId = "seq_";
		objId += seqs[i];
		pub("button", objId.c_str(), p);
	}

	// Audio-Volume Number
	{
		String p = "{\"name\":\"Huyang Volume\",\"command_topic\":\"";
		p += _baseTopic + "/cmd/audio/volume\",\"min\":0,\"max\":30,\"step\":1,";
		p += "\"unique_id\":\"huyang_volume\","; p += dev + "}";
		pub("number", "volume", p);
	}

	// Eye-State Select
	{
		String p = "{\"name\":\"Huyang Eyes\",\"command_topic\":\"";
		p += _baseTopic + "/cmd/eye\","
			 "\"options\":[\"open\",\"closed\",\"blink\",\"focus\",\"sad\",\"angry\"],"
			 "\"unique_id\":\"huyang_eyes\",";
		p += dev + "}";
		pub("select", "eyes", p);
	}

	Serial.println(F("MQTT: HA discovery published"));
}

void HuyangMqtt::handleMessage(char *topic, byte *payload, unsigned int length)
{
	// Payload zu String konvertieren
	String msg;
	msg.reserve(length);
	for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];

	// Topic nach baseTopic abschneiden -> sub-Pfad
	String t(topic);
	String prefix = _baseTopic + "/cmd/";
	if (!t.startsWith(prefix)) return;
	String sub = t.substring(prefix.length()); // z.B. "eye", "sequence", "audio/play"

	Serial.print(F("MQTT cmd: "));
	Serial.print(sub);
	Serial.print(F(" = "));
	Serial.println(msg);

	if (sub == "eye" && _face)
	{
		// "open", "closed", "blink", "focus", "sad", "angry"
		HuyangFace::EyeState s = HuyangFace::Open;
		if      (msg == "open")    s = HuyangFace::Open;
		else if (msg == "closed")  s = HuyangFace::Closed;
		else if (msg == "blink")   s = HuyangFace::Blink;
		else if (msg == "focus")   s = HuyangFace::Focus;
		else if (msg == "sad")     s = HuyangFace::Sad;
		else if (msg == "angry")   s = HuyangFace::Angry;
		_face->setEyesTo(s);
		_face->automatic = false;
	}
	else if (sub == "sequence" && _seq)
	{
		_seq->triggerByName(msg);
	}
	else if (sub == "audio/play" && _audio)
	{
		_audio->playTrack(msg.toInt());
	}
	else if (sub == "audio/stop" && _audio)
	{
		_audio->stop();
	}
	else if (sub == "audio/volume" && _audio)
	{
		_audio->setVolume(constrain(msg.toInt(), 0, 30));
	}
	else if (sub == "neck/rotate" && _neck)
	{
		_neck->rotateHead(msg.toInt());
		_neck->automatic = false;
	}
	else if (sub == "body/rotate" && _body)
	{
		_body->rotateBody(msg.toInt());
		_body->automatic = false;
	}
}

#endif // HUYANG_MQTT_ENABLED
