#include "WebServer.h"
#include "HuyangWifi.h"
#include "HuyangMqtt.h"
#include "HuyangConfig.h"
#include "HuyangSequence.h"
#include "WebAssets.h"

// File-scope-Helper fuer Custom-Sequence-JSON <-> SequenceStep[]
static SequenceStep::Action actionFromString(const String &act)
{
	if (act == "eye")            return SequenceStep::ActionEyeState;
	if (act == "neck_rot")       return SequenceStep::ActionNeckRotate;
	if (act == "neck_tilt_fwd")  return SequenceStep::ActionNeckTiltForward;
	if (act == "neck_tilt_side") return SequenceStep::ActionNeckTiltSideways;
	if (act == "body_rot")       return SequenceStep::ActionBodyRotate;
	if (act == "body_tilt_fwd")  return SequenceStep::ActionBodyTiltForward;
	if (act == "body_tilt_side") return SequenceStep::ActionBodyTiltSideways;
	if (act == "audio")          return SequenceStep::ActionAudioPlay;
	if (act == "audio_stop")     return SequenceStep::ActionAudioStop;
	if (act == "auto")           return SequenceStep::ActionSetAutomatic;
	return SequenceStep::ActionNone;
}

static const char *actionToString(SequenceStep::Action a)
{
	switch (a) {
		case SequenceStep::ActionEyeState:         return "eye";
		case SequenceStep::ActionNeckRotate:       return "neck_rot";
		case SequenceStep::ActionNeckTiltForward:  return "neck_tilt_fwd";
		case SequenceStep::ActionNeckTiltSideways: return "neck_tilt_side";
		case SequenceStep::ActionBodyRotate:       return "body_rot";
		case SequenceStep::ActionBodyTiltForward:  return "body_tilt_fwd";
		case SequenceStep::ActionBodyTiltSideways: return "body_tilt_side";
		case SequenceStep::ActionAudioPlay:        return "audio";
		case SequenceStep::ActionAudioStop:        return "audio_stop";
		case SequenceStep::ActionSetAutomatic:     return "auto";
		default:                                   return "none";
	}
}

static size_t parseCustomSequenceJson(JsonDocument &d, SequenceStep *out)
{
	JsonArray steps = d["steps"].as<JsonArray>();
	size_t count = 0;
	for (JsonObject s : steps) {
		if (count >= HuyangSequence::CUSTOM_MAX_STEPS) break;
		out[count].atMs   = s["atMs"]   | 0;
		out[count].action = actionFromString(s["action"] | "");
		out[count].param1 = s["param1"] | 0;
		out[count].param2 = s["param2"] | 0;
		count++;
	}
	return count;
}

WebServer::WebServer(uint32_t port)
{
	_server = new AsyncWebServer(port);
}

void WebServer::setup(bool enableEyes,
					  bool enableMonacle,
					  bool enableNeckMovement,
					  bool enableHeadRotation,
					  bool enableBodyMovement,
					  bool enableBodyRotation,
					  bool enableTorsoLights)
{
	_enableEyes = enableEyes;
	_enableMonacle = enableMonacle;
	_enableNeckMovement = enableNeckMovement;
	_enableHeadRotation = enableHeadRotation;
	_enableBodyMovement = enableBodyMovement;
	_enableBodyRotation = enableBodyRotation;
	_enableTorsoLights = enableTorsoLights;
	// Kein LittleFS noetig - UI ist in PROGMEM (WebAssets.h)
}

void WebServer::setComponents(HuyangFace *face, HuyangNeck *neck, HuyangBody *body, HuyangAudio *audio)
{
	_face = face;
	_neck = neck;
	_body = body;
	_audio = audio;
}

void WebServer::setAuth(bool enabled, const char *user, const char *pass)
{
	_authEnabled = enabled;
	_authUser = user ? String(user) : "";
	_authPass = pass ? String(pass) : "";
	if (enabled)
	{
		Serial.print(F("Web-Auth aktiv (user: "));
		Serial.print(_authUser);
		Serial.println(F(")"));
	}
}

bool WebServer::checkAuth(AsyncWebServerRequest *request)
{
	if (!_authEnabled) return true;
	if (!request->authenticate(_authUser.c_str(), _authPass.c_str()))
	{
		request->requestAuthentication();
		return false;
	}
	return true;
}

// Simple CSRF-Schutz: pruefe ob Origin/Referer zum eigenen Host passt.
// Greift nur wenn Web-Auth aktiv ist - sonst ist das Geraet eh offen im LAN.
// Akzeptiert auch fehlende Header (z.B. curl), nur explizit fremder Origin wird blockiert.
bool WebServer::checkSameOrigin(AsyncWebServerRequest *request)
{
	if (!_authEnabled) return true;
	String host = request->host();
	if (host.length() == 0) return true;

	String check;
	if (request->hasHeader("Origin")) check = request->header("Origin");
	else if (request->hasHeader("Referer")) check = request->header("Referer");
	else return true; // kein Origin/Referer -> nicht-Browser-Request, durchlassen

	// erlaubt: http://host oder http://host:port am Anfang
	if (check.indexOf("://" + host) > 0) return true;
	request->send(403, "application/json", F("{\"error\":\"csrf: foreign origin\"}"));
	return false;
}

void WebServer::start()
{
	_server->on(
		"/api/post.json", HTTP_POST, [&](AsyncWebServerRequest *request) {}, nullptr, [&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
		{ if (!checkAuth(request)) return; apiPostAction(request, data, len, index, total); });

	_server->on("/styles.css", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/css", WEB_STYLES_CSS); });

	_server->on("/javascript.js", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/javascript", WEB_JAVASCRIPT_JS); });

	_server->on("/joystick.js", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/javascript", WEB_JOYSTICK_JS); });

	_server->on("/", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send(200, "text/html", getPage(indexPage, request)); });

	_server->on("/index.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send(200, "text/html", getPage(indexPage, request)); });

	_server->on("/index.face.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/html", WEB_FACE_HTML); });

	_server->on("/index.neck.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/html", WEB_NECK_HTML); });

	_server->on("/index.body.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/html", WEB_BODY_HTML); });

	_server->on("/index.audio.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/html", WEB_AUDIO_HTML); });

	_server->on("/index.sequence.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send_P(200, "text/html", WEB_SEQUENCE_HTML); });

	_server->on("/settings.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send(200, "text/html", getPage(settingsPage, request)); });

	_server->on("/calibration.html", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send(200, "text/html", getPage(calibrationPage, request)); });

	_server->on("/api", HTTP_GET, [&](AsyncWebServerRequest *request)
				{ if (!checkAuth(request)) return; request->send(200, "text/html", getPage(apiPage, request)); });

	setupTriggerRoutes();

	_server->onNotFound([&](AsyncWebServerRequest *request)
						{ notFound(request); });

	_server->begin();
	Serial.println(F("Web server started"));
}

void WebServer::setupTriggerRoutes()
{
	_server->on("/trigger/wakeup", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Open);
		allEyes = HuyangFace::EyeState::Open;
		automaticAnimations = true;
		sendTriggerResponse(request, "wakeup"); });

	_server->on("/trigger/sleep", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Closed);
		allEyes = HuyangFace::EyeState::Closed;
		if (_neck) {
			neckRotate = 0; neckTiltForward = 0; neckTiltSideways = 0;
			_neck->rotateHead(0); _neck->tiltNeckForward(0); _neck->tiltNeckSideways(0);
		}
		automaticAnimations = false;
		sendTriggerResponse(request, "sleep"); });

	_server->on("/trigger/nod", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_neck) _neck->tiltNeckForward(50, 500);
		neckTiltForward = 0;
		sendTriggerResponse(request, "nod"); });

	_server->on("/trigger/shake", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_neck) _neck->rotateHead(-50, 500);
		neckRotate = 0;
		sendTriggerResponse(request, "shake"); });

	_server->on("/trigger/look", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Focus);
		allEyes = HuyangFace::EyeState::Focus;
		automaticAnimations = false;
		sendTriggerResponse(request, "look"); });

	_server->on("/trigger/sad", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Sad);
		if (_neck) _neck->tiltNeckForward(30, 1000);
		allEyes = HuyangFace::EyeState::Sad;
		neckTiltForward = 30;
		automaticAnimations = false;
		sendTriggerResponse(request, "sad"); });

	_server->on("/trigger/angry", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Angry);
		allEyes = HuyangFace::EyeState::Angry;
		automaticAnimations = false;
		sendTriggerResponse(request, "angry"); });

	_server->on("/trigger/blink", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) _face->setEyesTo(HuyangFace::EyeState::Blink);
		allEyes = HuyangFace::EyeState::Blink;
		sendTriggerResponse(request, "blink"); });

	_server->on("/trigger/random", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		automaticAnimations = true;
		allEyes = 0; leftEye = 0; rightEye = 0;
		sendTriggerResponse(request, "random"); });

	_server->on("/trigger/reset", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		automaticAnimations = true;
		allEyes = 0; leftEye = 0; rightEye = 0;
		neckRotate = 0; neckTiltForward = 0; neckTiltSideways = 0;
		bodyRotate = 0; bodyTiltForward = 0; bodyTiltSideways = 0;
		if (_neck) { _neck->rotateHead(0); _neck->tiltNeckForward(0); _neck->tiltNeckSideways(0); }
		if (_body) { _body->rotateBody(0); _body->tiltBodyForward(0); _body->tiltBodySideways(0); }
		sendTriggerResponse(request, "reset"); });

	_server->on("/trigger/sound", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("id"))
		{
			int trackNum = request->getParam("id")->value().toInt();
			Serial.print(F("Trigger: play sound "));
			Serial.println(trackNum);
			if (_audio) _audio->playTrack(trackNum);
		}
		sendTriggerResponse(request, "sound"); });

	// =========================================================================
	// Audio-API: gezielte DFPlayer-Kontrolle
	// =========================================================================
	_server->on("/audio/play", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		int track = 1;
		if (request->hasParam("track")) track = request->getParam("track")->value().toInt();
		if (_audio) _audio->playTrack(track);
		sendTriggerResponse(request, "audio/play"); });

	_server->on("/audio/stop", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_audio) _audio->stop();
		sendTriggerResponse(request, "audio/stop"); });

	_server->on("/audio/volume", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("v"))
		{
			int v = request->getParam("v")->value().toInt();
			v = constrain(v, 0, 30);
			if (_audio) _audio->setVolume((uint8_t)v);
		}
		sendTriggerResponse(request, "audio/volume"); });

	// =========================================================================
	// Sequence-API: vordefinierte Animation-Sequenzen abspielen
	// =========================================================================
	_server->on("/sequence", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("name") && _sequence)
		{
			String name = request->getParam("name")->value();
			bool ok = false;
			if (name == "custom") {
				// Custom-Sequence direkt aus EEPROM-Blob abspielen
				if (_config && _config->data.customSeqCount > 0) {
					const SequenceStep *st = (const SequenceStep *)_config->data.customSeqBlob;
					_sequence->playCustomSteps(st, _config->data.customSeqCount, "custom");
					ok = true;
				}
			} else {
				ok = _sequence->triggerByName(name);
			}
			JsonDocument r;
			r["status"] = ok ? "ok" : "unknown_or_empty";
			r["sequence"] = name;
			String result;
			serializeJson(r, result);
			request->send(ok ? 200 : 404, "application/json", result);
		}
		else
		{
			request->send(400, "application/json", F("{\"error\":\"missing name param\"}"));
		} });

	_server->on("/sequence/stop", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_sequence) _sequence->stop();
		sendTriggerResponse(request, "sequence/stop"); });

	// =========================================================================
	// Custom-Sequence: save (POST JSON) / load / play - persistiert in EEPROM
	// =========================================================================
	_server->on(
		"/sequence/save", HTTP_POST, [&](AsyncWebServerRequest *request) {}, nullptr,
		[&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
			if (!checkAuth(request)) return;
			if (index == 0) { _postBodyBuffer = ""; _postBodyBuffer.reserve(total); }
			_postBodyBuffer.concat((const char *)data, len);
			if (index + len < total) return;

			JsonDocument d;
			DeserializationError err = deserializeJson(d, _postBodyBuffer);
			_postBodyBuffer = "";
			if (err) { request->send(400, "application/json", F("{\"error\":\"invalid json\"}")); return; }

			if (_config) {
				SequenceStep buf[HuyangSequence::CUSTOM_MAX_STEPS];
				size_t count = parseCustomSequenceJson(d, buf);
				_config->data.customSeqCount = (uint8_t)count;
				memset(_config->data.customSeqBlob, 0, HUYANG_CUSTOM_SEQ_MAX_BYTES);
				memcpy(_config->data.customSeqBlob, buf, count * sizeof(SequenceStep));
				_config->save();
			}
			request->send(200, "application/json", F("{\"status\":\"saved\"}"));
		});

	_server->on("/sequence/load", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		JsonDocument j;
		j["name"] = "custom";
		JsonArray steps = j["steps"].to<JsonArray>();
		if (_config && _config->data.customSeqCount > 0) {
			const SequenceStep *st = (const SequenceStep *)_config->data.customSeqBlob;
			for (uint8_t i = 0; i < _config->data.customSeqCount; i++) {
				JsonObject step = steps.add<JsonObject>();
				step["atMs"] = st[i].atMs;
				step["action"] = actionToString(st[i].action);
				step["param1"] = st[i].param1;
				step["param2"] = st[i].param2;
			}
		}
		String out;
		serializeJson(j, out);
		request->send(200, "application/json", out); });

	// =========================================================================
	// Config: get / set / reset (Feature-Flags + WiFi-Settings)
	// =========================================================================
	_server->on("/config/get", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		JsonDocument c;
		if (_config) {
			c["enableEyes"]          = _config->data.enableEyes;
			c["enableMonacle"]       = _config->data.enableMonacle;
			c["enableNeckMovement"]  = _config->data.enableNeckMovement;
			c["enableHeadRotation"]  = _config->data.enableHeadRotation;
			c["enableBodyMovement"]  = _config->data.enableBodyMovement;
			c["enableBodyRotation"]  = _config->data.enableBodyRotation;
			c["enableTorsoLights"]   = _config->data.enableTorsoLights;
			c["wifiSSID"]            = _config->data.wifiSSID;
			c["wifiMode"]            = _config->data.wifiMode;
			c["volume"]              = _config->data.volume;
		}
		String out;
		serializeJson(c, out);
		request->send(200, "application/json", out); });

	_server->on(
		"/config/set", HTTP_POST, [&](AsyncWebServerRequest *request) {}, nullptr,
		[&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
			if (!checkAuth(request)) return;
			if (index == 0) { _postBodyBuffer = ""; _postBodyBuffer.reserve(total); }
			_postBodyBuffer.concat((const char *)data, len);
			if (index + len < total) return;
			JsonDocument j;
			DeserializationError err = deserializeJson(j, _postBodyBuffer);
			_postBodyBuffer = "";
			if (err) { request->send(400, "application/json", F("{\"error\":\"invalid json\"}")); return; }
			if (_config) {
				if (!j["enableEyes"].isNull())         _config->data.enableEyes         = j["enableEyes"];
				if (!j["enableMonacle"].isNull())      _config->data.enableMonacle      = j["enableMonacle"];
				if (!j["enableNeckMovement"].isNull()) _config->data.enableNeckMovement = j["enableNeckMovement"];
				if (!j["enableHeadRotation"].isNull()) _config->data.enableHeadRotation = j["enableHeadRotation"];
				if (!j["enableBodyMovement"].isNull()) _config->data.enableBodyMovement = j["enableBodyMovement"];
				if (!j["enableBodyRotation"].isNull()) _config->data.enableBodyRotation = j["enableBodyRotation"];
				if (!j["enableTorsoLights"].isNull())  _config->data.enableTorsoLights  = j["enableTorsoLights"];
				if (!j["volume"].isNull())             _config->data.volume             = j["volume"];
				_config->save();
			}
			request->send(200, "application/json", F("{\"status\":\"ok\"}"));
		});

	_server->on("/config/reset", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (!checkSameOrigin(request)) return;
		if (_config) { _config->resetToDefaults(); _config->save(); }
		sendTriggerResponse(request, "config/reset"); });

	_server->on("/reboot", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (!checkSameOrigin(request)) return;
		sendTriggerResponse(request, "reboot");
		delay(200);
		ESP.restart(); });

	// =========================================================================
	// WiFi: scan + connect
	// =========================================================================
	_server->on("/wifi/scan", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		int n = WiFi.scanNetworks(false, true); // sync, includeHidden
		JsonDocument j;
		JsonArray arr = j["networks"].to<JsonArray>();
		for (int i = 0; i < n; i++) {
			JsonObject net = arr.add<JsonObject>();
			net["ssid"] = WiFi.SSID(i);
			net["rssi"] = WiFi.RSSI(i);
		}
		String out;
		serializeJson(j, out);
		request->send(200, "application/json", out); });

	_server->on("/wifi/connect", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (!checkSameOrigin(request)) return;
		if (request->hasParam("ssid") && _config) {
			String ssid = request->getParam("ssid")->value();
			String pass = request->hasParam("pass") ? request->getParam("pass")->value() : "";
			strncpy(_config->data.wifiSSID, ssid.c_str(), sizeof(_config->data.wifiSSID) - 1);
			strncpy(_config->data.wifiPassword, pass.c_str(), sizeof(_config->data.wifiPassword) - 1);
			_config->data.wifiSSID[sizeof(_config->data.wifiSSID) - 1] = 0;
			_config->data.wifiPassword[sizeof(_config->data.wifiPassword) - 1] = 0;
			_config->data.wifiMode = 1;
			_config->save();
		}
		sendTriggerResponse(request, "wifi/connect"); });

	// =========================================================================
	// Calibration: direkter PCA9685-Channel-Write (/cal/servo?ch=N&pwm=M)
	// =========================================================================
	_server->on("/cal/servo", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("ch") && request->hasParam("pwm") && _body) {
			int ch = request->getParam("ch")->value().toInt();
			int pwm = request->getParam("pwm")->value().toInt();
			if (ch >= 0 && ch <= 15) {
				_body->setRawServo((uint8_t)ch, (uint16_t)constrain(pwm, 0, 4095));
			}
		}
		sendTriggerResponse(request, "cal/servo"); });

	// =========================================================================
	// Self-Test (/test/run) - durchlaeuft Servos, Augen, Audio
	// =========================================================================
	_server->on("/test/run", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_sequence) _sequence->playSelfTest();
		sendTriggerResponse(request, "test/run"); });

	// =========================================================================
	// Servo-Speed-Preset (/servo/speed?preset=slow|normal|fast)
	// =========================================================================
	_server->on("/servo/speed", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("preset") && _neck) {
			String p = request->getParam("preset")->value();
			float m = 1.0f;
			if (p == "slow") m = 0.5f;
			else if (p == "fast") m = 2.0f;
			else m = 1.0f;
			_neck->setSpeedMultiplier(m);
		}
		JsonDocument r;
		r["status"] = "ok";
		r["multiplier"] = _neck ? _neck->getSpeedMultiplier() : 1.0f;
		String out;
		serializeJson(r, out);
		request->send(200, "application/json", out); });

	// =========================================================================
	// Status-Endpoint: liefert aktuellen System-Zustand fuer das Web-Dashboard
	// =========================================================================
	// =========================================================================
	// Eye-Color-API: User kann Augenfarbe per Hex setzen (/eye/color?hex=ffcc00)
	// =========================================================================
	_server->on("/eye/color", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("hex") && _face) {
			String hex = request->getParam("hex")->value();
			if (hex.startsWith("#")) hex = hex.substring(1);
			uint32_t rgb = strtoul(hex.c_str(), nullptr, 16);
			_face->setEyeColorHex(rgb);
		}
		JsonDocument r;
		r["status"] = "ok";
		if (_face) {
			char hex[8];
			snprintf(hex, sizeof(hex), "%06lX", (unsigned long)_face->getEyeColorHex());
			r["color"] = hex;
		}
		String out;
		serializeJson(r, out);
		request->send(200, "application/json", out); });

	// =========================================================================
	// Pupil-API: an/aus + Farbe + Groesse (/eye/pupil?enabled=1&hex=000000&size=30)
	// =========================================================================
	_server->on("/eye/pupil", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (_face) {
			if (request->hasParam("enabled")) {
				int v = request->getParam("enabled")->value().toInt();
				_face->setPupilEnabled(v != 0);
			}
			if (request->hasParam("hex")) {
				String hex = request->getParam("hex")->value();
				if (hex.startsWith("#")) hex = hex.substring(1);
				uint32_t rgb = strtoul(hex.c_str(), nullptr, 16);
				_face->setPupilColorHex(rgb);
			}
			if (request->hasParam("size")) {
				int sz = request->getParam("size")->value().toInt();
				_face->setPupilRadius((uint8_t)constrain(sz, 6, 80));
			}
		}
		JsonDocument r;
		r["status"] = "ok";
		if (_face) {
			r["enabled"] = _face->getPupilEnabled();
			char hex[8];
			snprintf(hex, sizeof(hex), "%06lX", (unsigned long)_face->getPupilColorHex());
			r["color"] = hex;
			r["size"] = _face->getPupilRadius();
		}
		String out;
		serializeJson(r, out);
		request->send(200, "application/json", out); });

	// =========================================================================
	// Closed-Eye-Color (/eye/closedcolor?hex=ffffff)
	// =========================================================================
	_server->on("/eye/closedcolor", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		if (request->hasParam("hex") && _face) {
			String hex = request->getParam("hex")->value();
			if (hex.startsWith("#")) hex = hex.substring(1);
			_face->setClosedEyeColorHex(strtoul(hex.c_str(), nullptr, 16));
		}
		JsonDocument r;
		r["status"] = "ok";
		if (_face) {
			char hex[8];
			snprintf(hex, sizeof(hex), "%06lX", (unsigned long)_face->getClosedEyeColorHex());
			r["color"] = hex;
		}
		String out;
		serializeJson(r, out);
		request->send(200, "application/json", out); });

	_server->on("/status", HTTP_GET, [&](AsyncWebServerRequest *request)
				{
		if (!checkAuth(request)) return;
		JsonDocument s;
		if (_wifi) {
			s["wifi"]["mode"] = _wifi->getMode();
			s["wifi"]["connected"] = _wifi->isConnected();
			s["wifi"]["ip"] = _wifi->getIP().toString();
		}
		s["mqtt"]["connected"] = _mqtt ? _mqtt->isConnected() : false;
		s["sequence"]["playing"] = _sequence ? _sequence->isPlaying() : false;
		s["sequence"]["name"] = _sequence ? _sequence->getName() : "";
		s["sequence"]["progress"] = _sequence ? _sequence->getProgress() : 0;
		s["hostname"] = "huyang";
		s["heap"] = ESP.getFreeHeap();
		String out;
		serializeJson(s, out);
		request->send(200, "application/json", out); });
}

void WebServer::sendTriggerResponse(AsyncWebServerRequest *request, const char *action)
{
	JsonDocument r;
	r["status"] = "ok";
	r["action"] = action;
	String result;
	serializeJson(r, result);
	request->send(200, "application/json", result);
	Serial.print(F("Trigger: "));
	Serial.println(action);
}

void WebServer::apiPostAction(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
{
	// Accumulate chunked body
	if (index == 0)
	{
		_postBodyBuffer = "";
		_postBodyBuffer.reserve(total);
	}
	_postBodyBuffer.concat((const char *)data, len);

	if (index + len < total) return; // Wait for more chunks

	JsonDocument json;
	DeserializationError err = deserializeJson(json, _postBodyBuffer);
	_postBodyBuffer = "";

	if (err)
	{
		request->send(400, "application/json", F("{\"error\":\"invalid json\"}"));
		return;
	}

	if (!json["automatic"].isNull())
	{
		automaticAnimations = json["automatic"];
		if (automaticAnimations) { allEyes = 0; leftEye = 0; rightEye = 0; }
	}

	if (!json["face"].isNull())
	{
		uint8_t face_all = json["face"]["all"].as<uint8_t>();
		if (face_all != 0)
		{
			allEyes = face_all;
			leftEye = 0; rightEye = 0;
			automaticAnimations = false;
		}
		else
		{
			allEyes = 0;
			leftEye = json["face"]["left"].as<uint8_t>();
			rightEye = json["face"]["right"].as<uint8_t>();
			automaticAnimations = false;
		}
	}

	if (!json["neck"].isNull())
	{
		if (!json["neck"]["rotate"].isNull())
		{ neckRotate = constrain((int16_t)json["neck"]["rotate"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
		if (!json["neck"]["tiltForward"].isNull())
		{ neckTiltForward = constrain((int16_t)json["neck"]["tiltForward"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
		if (!json["neck"]["tiltSideways"].isNull())
		{ neckTiltSideways = constrain((int16_t)json["neck"]["tiltSideways"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
	}

	if (!json["body"].isNull())
	{
		if (!json["body"]["rotate"].isNull())
		{ bodyRotate = constrain((int16_t)json["body"]["rotate"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
		if (!json["body"]["tiltForward"].isNull())
		{ bodyTiltForward = constrain((int16_t)json["body"]["tiltForward"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
		if (!json["body"]["tiltSideways"].isNull())
		{ bodyTiltSideways = constrain((int16_t)json["body"]["tiltSideways"], (int16_t)-100, (int16_t)100); automaticAnimations = false; }
	}

	JsonDocument r;
	r["automatic"] = automaticAnimations;
	r["face"]["eyes"]["all"] = allEyes;
	r["face"]["eyes"]["left"] = leftEye;
	r["face"]["eyes"]["right"] = rightEye;
	r["neck"]["rotate"] = neckRotate;
	r["neck"]["tiltForward"] = neckTiltForward;
	r["neck"]["tiltSideways"] = neckTiltSideways;
	r["body"]["rotate"] = bodyRotate;
	r["body"]["tiltForward"] = bodyTiltForward;
	r["body"]["tiltSideways"] = bodyTiltSideways;

	String result;
	serializeJson(r, result);
	request->send(200, "application/json", result);
}

String WebServer::getPage(Page page, AsyncWebServerRequest *request)
{
	String html;

	switch (page)
	{
	case indexPage:
	{
		String base = FPSTR(WEB_BASE_HTML);
		String index = FPSTR(WEB_INDEX_HTML);
		html.reserve(base.length() + index.length() + 512);
		html = base;
		html.replace("###BODY###", index);

		html.replace("###FACE###", _enableEyes ?
			"<div id=\"container_face\" class=\"section\" onclick=\"loadContainer('/index.face.html', 'container_face')\">Activate Face Interface</div>" : "");
		html.replace("###NECK###", (_enableNeckMovement || _enableHeadRotation) ?
			"<div id=\"container_neck\" class=\"section\" onclick=\"loadContainer('/index.neck.html', 'container_neck')\">Activate Neck Interface</div>" : "");
		html.replace("###BODY###", (_enableBodyMovement || _enableBodyRotation) ?
			"<div id=\"container_body\" class=\"section\" onclick=\"loadContainer('/index.body.html', 'container_body')\">Activate Body Interface</div>" : "");
		html.replace("###AUDIO###",
			"<div id=\"container_audio\" class=\"section\" onclick=\"loadContainer('/index.audio.html', 'container_audio')\">Activate Audio Interface</div>");
		html.replace("###SEQUENCE###",
			"<div id=\"container_sequence\" class=\"section\" onclick=\"loadContainer('/index.sequence.html', 'container_sequence')\">Activate Sequence Interface</div>");
	}
	break;
	case settingsPage:
	{
		String settings = FPSTR(WEB_SETTINGS_HTML);
		getBaseHtml(settings, html);
	}
	break;
	case calibrationPage:
	{
		String calibration = FPSTR(WEB_CALIBRATION_HTML);
		getBaseHtml(calibration, html);
	}
	break;
	case apiPage:
	{
		String api = FPSTR(WEB_API_HTML);
		getBaseHtml(api, html);
	}
	break;
	}
	return html;
}

void WebServer::notFound(AsyncWebServerRequest *request)
{
	request->send(404, "text/plain", "Not found");
}

void WebServer::getBaseHtml(const String &body, String &target)
{
	target = FPSTR(WEB_BASE_HTML);
	target.replace("###BODY###", body);
}
