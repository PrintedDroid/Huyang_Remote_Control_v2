// =============================================================================
// AUTO-GENERIERT von gen-webassets.ps1 - NICHT VON HAND BEARBEITEN
// Source: data/*.* files
// Generiert: 2026-05-28 10:36:35
// =============================================================================
#ifndef WebAssets_h
#define WebAssets_h

#include <Arduino.h>
// ----- api.html (5214 bytes) -----
const char WEB_API_HTML[] PROGMEM = R"HUY(
<h4>HTTP API Reference</h4>
<p style="font-size: 12px; color: var(--text-dim);">Alle Endpoints liefern JSON. Bei aktiver Auth: HTTP Basic Auth Header.</p>

<style>
.api-table { width: 100%; border-collapse: collapse; font-size: 12px; text-align: left; }
.api-table th, .api-table td { padding: 8px; border-bottom: 1px solid var(--border); vertical-align: top; }
.api-table th { color: var(--text-dim); font-weight: 600; }
.api-table td.method { font-family: monospace; color: var(--accent); width: 60px; }
.api-table td.path { font-family: monospace; color: var(--text); }
.api-table td.desc { color: var(--text-dim); }
.api-group { margin: 16px 0; }
.api-group h5 { color: var(--accent); border-bottom: 2px solid var(--border); padding-bottom: 4px; margin: 12px 0; font-size: 14px; text-transform: uppercase; letter-spacing: 1px; }
</style>

<div class="api-group">
<h5>Status / System</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/status</td><td class="desc">WiFi-Mode, IP, MQTT, Sequence-State, Heap</td></tr>
<tr><td class="method">GET</td><td class="path">/api</td><td class="desc">Diese Doku-Seite</td></tr>
</table>
</div>

<div class="api-group">
<h5>Eye / Face</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/eye/color?hex=ffdd22</td><td class="desc">Setzt Augenfarbe (RGB888 hex)</td></tr>
<tr><td class="method">GET</td><td class="path">/eye/closedcolor?hex=ffffff</td><td class="desc">Setzt Farbe der geschlossenen Augen</td></tr>
<tr><td class="method">GET</td><td class="path">/eye/pupil?enabled=1&hex=000000&size=30</td><td class="desc">Pupille an/aus + Farbe + Groesse (6-80 px). Alle Params optional.</td></tr>
</table>
</div>

<div class="api-group">
<h5>Trigger (kurze Aktionen)</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/trigger/wakeup</td><td class="desc">Augen auf, Auto-Mode an</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/sleep</td><td class="desc">Augen zu, alles Mitte</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/nod</td><td class="desc">Kopf nicken</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/shake</td><td class="desc">Kopf schuetteln</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/look | /sad | /angry | /blink</td><td class="desc">Eye-State setzen</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/random</td><td class="desc">Auto-Animation an</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/reset</td><td class="desc">Alles auf Neutral</td></tr>
<tr><td class="method">GET</td><td class="path">/trigger/sound?id=N</td><td class="desc">DFPlayer-Track N</td></tr>
</table>
</div>

<div class="api-group">
<h5>Live-Steuerung</h5>
<table class="api-table">
<tr><td class="method">POST</td><td class="path">/api/post.json</td><td class="desc">JSON body: {automatic, face:{}, neck:{rotate,tiltForward,tiltSideways}, body:{...}}</td></tr>
</table>
</div>

<div class="api-group">
<h5>Audio</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/audio/play?track=N</td><td class="desc">DFPlayer-Track abspielen</td></tr>
<tr><td class="method">GET</td><td class="path">/audio/stop</td><td class="desc">Stopp</td></tr>
<tr><td class="method">GET</td><td class="path">/audio/volume?v=0-30</td><td class="desc">Lautstaerke</td></tr>
</table>
</div>

<div class="api-group">
<h5>Sequences</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/sequence?name=X</td><td class="desc">X = greeting, surprised, sad, angry, selftest</td></tr>
<tr><td class="method">GET</td><td class="path">/sequence/stop</td><td class="desc">Abort the current sequence</td></tr>
</table>
</div>

<div class="api-group">
<h5>Servos</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/servo/speed?preset=slow|normal|fast</td><td class="desc">Skaliert Easing-Geschwindigkeit (0.5x / 1x / 2x)</td></tr>
</table>
</div>

<div class="api-group">
<h5>System / Test</h5>
<table class="api-table">
<tr><td class="method">GET</td><td class="path">/test/run</td><td class="desc">Self-Test: Servos sweep, Eyes durch alle States, Audio anspielen</td></tr>
<tr><td class="method">GET</td><td class="path">/wifi/scan</td><td class="desc">List available WiFi networks</td></tr>
<tr><td class="method">POST</td><td class="path">/config/backup</td><td class="desc">EEPROM-Config als JSON-Download</td></tr>
<tr><td class="method">POST</td><td class="path">/config/restore</td><td class="desc">Upload JSON back into EEPROM</td></tr>
<tr><td class="method">GET</td><td class="path">/reboot</td><td class="desc">ESP neustarten</td></tr>
</table>
</div>

<div class="api-group">
<h5>Beispiele (curl)</h5>
<pre style="background: var(--bg-elev-2); padding: 10px; border-radius: 6px; font-size: 11px; overflow-x: auto;">
curl http://huyang.local/eye/color?hex=ff0000
curl http://huyang.local/sequence?name=greeting
curl http://huyang.local/audio/play?track=5

# mit POST + JSON
curl -X POST http://huyang.local/api/post.json \
     -H "Content-Type: application/json" \
     -d '{"neck":{"rotate":50,"tiltForward":-20}}'
</pre>
</div>
)HUY";

// ----- base.html (794 bytes) -----
const char WEB_BASE_HTML[] PROGMEM = R"HUY(
<!DOCTYPE HTML><html>
<head>
<title>Huyang Control</title>
<meta name="viewport" content="width=device-width, initial-scale=1, user-scalable=0, viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes" />
<meta name="theme-color" content="#0a0a0a" />
<link rel="stylesheet" type="text/css" href="/styles.css">
<script src="/joystick.js"></script>
<script src="/javascript.js"></script>
</head>
<body>
<div id="status_bar" class="status_bar">
  <span id="status_text">connecting&hellip;</span>
  <span id="status_net"></span>
  <div id="status_progress" class="status_progress"><div id="status_progress_bar"></div></div>
</div>

<div class="app">
###BODY###

<footer>Made with &hearts; by Jeanette M&uuml;ller &mdash; Enhanced by Printed-Droid</footer>
</div>
</body>
</html>
)HUY";

// ----- calibration.html (2570 bytes) -----
const char WEB_CALIBRATION_HTML[] PROGMEM = R"HUY(
<h4>Servo Calibration</h4>

<p style="font-size: 12px; color: var(--text-dim); text-align: left;">
<b>Disconnect all servo horns before testing!</b><br>
Slider bewegt PCA9685-Kanal direkt. Center-Wert notieren, dann in <code>HuyangBody.h</code> / <code>HuyangNeck.h</code> als SERVOMIN/SERVOMAX hinterlegen.
Range: 150 (≈0°) bis 595 (≈180°), Mitte typisch ~373.
</p>

<div id="cal_container"></div>

<div class="menuContainer">
    <a href="#" class="button" onclick="calAllCenter(); return false;">All to 373 (center)</a>
    <a href="#" class="button" onclick="calAllOff();   return false;">All OFF</a>
</div>

<script>
(function() {
    const channels = [
        { ch: 1,  label: "Monocle (HuyangNeck)" },
        { ch: 4,  label: "Head Rotate" },
        { ch: 5,  label: "Head Top Left" },
        { ch: 6,  label: "Head Top Right" },
        { ch: 7,  label: "Neck Tilt" },
        { ch: 8,  label: "Head Rotate Servo" },
        { ch: 9,  label: "Neck Servo" },
        { ch: 11, label: "Body Rotate (HuyangBody)" },
        { ch: 12, label: "Body Forward Left" },
        { ch: 13, label: "Body Forward Right" },
        { ch: 14, label: "Body Sideways Left" },
        { ch: 15, label: "Body Sideways Right" },
    ];
    const container = document.getElementById('cal_container');
    channels.forEach(c => {
        const div = document.createElement('div');
        div.className = 'slidecontainer';
        div.style.textAlign = 'left';
        div.innerHTML = '<span style="font-size:12px;">' + c.label + ' (ch ' + c.ch + ')</span> '
            + '<span id="cal_val_' + c.ch + '" style="float:right; font-family:monospace; font-size:12px;">373</span>'
            + '<input id="cal_' + c.ch + '" type="range" min="150" max="595" value="373" class="slider"'
            + ' onInput="calSet(' + c.ch + ', this.value)" />';
        container.appendChild(div);
    });
})();

function calSet(ch, pwm) {
    const label = document.getElementById('cal_val_' + ch);
    if (label) label.innerText = pwm;
    fetch('/cal/servo?ch=' + ch + '&pwm=' + pwm).catch(() => {});
}
function calAllCenter() {
    document.querySelectorAll('[id^="cal_"][type="range"]').forEach(s => {
        s.value = 373;
        const ch = s.id.replace('cal_', '');
        calSet(ch, 373);
    });
}
function calAllOff() {
    // PWM 0 = Servo deaktiviert (kein Drehmoment)
    document.querySelectorAll('[id^="cal_"][type="range"]').forEach(s => {
        const ch = s.id.replace('cal_', '');
        fetch('/cal/servo?ch=' + ch + '&pwm=0').catch(() => {});
    });
}
</script>
)HUY";

// ----- index.audio.html (610 bytes) -----
const char WEB_AUDIO_HTML[] PROGMEM = R"HUY(
<h4>Audio</h4>

<div class="audio_controls">
    <input id="audio_track" type="number" min="1" max="999" value="1" class="audio_input" />
    <a href="#" class="button primary" onclick="audioPlay(); return false;">PLAY</a>
    <a href="#" class="button" onclick="audioStop(); return false;">STOP</a>
    <a href="#" id="audio_mute_btn" class="button" onclick="audioMute(); return false;">🔊</a>
</div>

<div class="slidecontainer">
    Volume <span id="audio_volume_label">20</span>
    <input id="slider_audioVolume" type="range" min="0" max="30" value="20" class="slider" onInput="audioVolume()" />
</div>
)HUY";

// ----- index.body.html (344 bytes) -----
const char WEB_BODY_HTML[] PROGMEM = R"HUY(
<h4>Body</h4>

<div class="joystickContainer">
    <div id="joyBody" class="joystick"></div>
</div>
<div id="body_state" class="remote_state">rot 0 | fwd 0 | side 0</div>

<div class="slidecontainer">
    Tilt
    <input id="slider_bodyTiltSideways" type="range" min="-100" max="100" value="0" class="slider" onInput="sendBodyUpdate()">
</div>
)HUY";

// ----- index.face.html (3924 bytes) -----
const char WEB_FACE_HTML[] PROGMEM = R"HUY(
<h4>Face</h4>

<div class="eye_color_row">
    <label for="eye_color">Eye:</label>
    <input id="eye_color" type="color" value="#ffdd22" onChange="sendEyeColor()" />
    <span id="eye_color_label">#ffdd22</span>
</div>
<div class="preset_row">
    <span class="preset_chip" style="background:#ffdd22"  onclick="setEyeColorPreset('ffdd22')" title="Huyang"></span>
    <span class="preset_chip" style="background:#ff2020"  onclick="setEyeColorPreset('ff2020')" title="Sith"></span>
    <span class="preset_chip" style="background:#22aaff"  onclick="setEyeColorPreset('22aaff')" title="Jedi"></span>
    <span class="preset_chip" style="background:#ff7a00"  onclick="setEyeColorPreset('ff7a00')" title="K-2SO"></span>
    <span class="preset_chip" style="background:#ffffff"  onclick="setEyeColorPreset('ffffff')" title="White"></span>
    <span class="preset_chip" style="background:#00ff66"  onclick="setEyeColorPreset('00ff66')" title="Toxic"></span>
</div>

<div class="eye_color_row">
    <label><input id="pupil_enabled" type="checkbox" onChange="sendPupilSettings()" /> Pupil</label>
    <input id="pupil_color" type="color" value="#000000" onChange="sendPupilSettings()" />
    <span id="pupil_color_label">#000000</span>
</div>
<div class="slidecontainer">
    Pupil size <span id="pupil_size_label">30</span> px
    <input id="slider_pupilSize" type="range" min="6" max="80" value="30" class="slider" onInput="sendPupilSize()" />
</div>

<div class="eye_color_row">
    <label for="closed_eye_color">Closed-Eye:</label>
    <input id="closed_eye_color" type="color" value="#ffffff" onChange="sendClosedEyeColor()" />
    <span id="closed_eye_color_label">#ffffff</span>
</div>

<div class="menu_eye_buttons">
    <div class="eye_open"  onClick="sendEyeUpdate('all', 1);" id="eye_all_open">OPEN</div>
    <div class="eye_close" onClick="sendEyeUpdate('all', 2);" id="eye_all_close">CLOSE</div>
    <div class="eye_open"  onClick="sendEyeUpdate('all', 3);" id="eye_all_blink">BLINK</div>
    <div class="eye_focus" onClick="sendEyeUpdate('all', 4);" id="eye_all_focus"><div></div></div>
    <div class="eye_sad"   onClick="sendEyeUpdate('all', 5);" id="eye_all_sad"><div class="right"></div>SAD</div>
    <div class="eye_angry" onClick="sendEyeUpdate('all', 6);" id="eye_all_angry"><div class="left"></div>ANGRY</div>
</div>

<div class="eye_preview_row">
    <div class="eyeContainer">
        <h4>Left</h4>
        <div class="eye" onClick="sendEyeUpdate('left', 3);" id="eye_left_blink"><h5>BLINK</h5></div>
        <div class="eye_buttons">
            <div class="eye_open"  onClick="sendEyeUpdate('left', 1);" id="eye_left_open">OPEN</div>
            <div class="eye_close" onClick="sendEyeUpdate('left', 2);" id="eye_left_close">CLOSE</div>
            <div class="eye_focus" onClick="sendEyeUpdate('left', 4);" id="eye_left_focus"><div></div></div>
            <div class="eye_sad"   onClick="sendEyeUpdate('left', 5);" id="eye_left_sad"><div class="right"></div></div>
            <div class="eye_angry" onClick="sendEyeUpdate('left', 6);" id="eye_left_angry"><div class="left"></div></div>
        </div>
    </div>
    <div class="eyeContainer">
        <h4>Right</h4>
        <div class="eye" onClick="sendEyeUpdate('right', 3);" id="eye_right_blink"><h5>BLINK</h5></div>
        <div class="eye_buttons">
            <div class="eye_open"  onClick="sendEyeUpdate('right', 1);" id="eye_right_open">OPEN</div>
            <div class="eye_close" onClick="sendEyeUpdate('right', 2);" id="eye_right_close">CLOSE</div>
            <div class="eye_focus" onClick="sendEyeUpdate('right', 4);" id="eye_right_focus"><div></div></div>
            <div class="eye_sad"   onClick="sendEyeUpdate('right', 5);" id="eye_right_sad"><div class="left"></div></div>
            <div class="eye_angry" onClick="sendEyeUpdate('right', 6);" id="eye_right_angry"><div class="right"></div></div>
        </div>
    </div>
</div>
)HUY";

// ----- index.html (1106 bytes) -----
const char WEB_INDEX_HTML[] PROGMEM = R"HUY(
<div class="menuContainer">
    <a href="#" id="button_automatic" class="button" onClick="changeAutomatic(true);">LOADING</a>
    <a href="/index.html" class="button">Start</a>
    <a href="/settings.html" class="button">Settings</a>
    <a href="#" id="button_theme" class="button" onClick="toggleTheme(); return false;">☾</a>
    <a href="#" id="button_fullscreen" class="button" onClick="toggleFullscreen(); return false;">⛶</a>
</div>

###FACE###
###NECK###
###BODY###
###AUDIO###
###SEQUENCE###

<script language="javascript">
    window.onload = function () {
        systemInit();
    }
    async function getHtml(url = "") {
        return await fetch(url);
    }
    function loadContainer(url, elementId) {
        getHtml(url)
            .then(response => response.text())
            .then(html => {
                document.getElementById(elementId).innerHTML = html;
                document.getElementById(elementId).removeAttribute("onclick");
                document.getElementById(elementId).classList.add('section');
                initJoystick();
            });
    }
</script>
)HUY";

// ----- index.neck.html (701 bytes) -----
const char WEB_NECK_HTML[] PROGMEM = R"HUY(
<h4>Neck</h4>

<div class="joystickContainer">
    <div id="joyNeck" class="joystick"></div>
</div>
<div id="neck_state" class="remote_state">rot 0 | fwd 0 | side 0</div>

<div class="slidecontainer">
    Tilt
    <input id="slider_neckTiltSideways" type="range" min="-100" max="100" value="0" class="slider" onInput="sendNeckUpdate()">
</div>

<div class="menuContainer">
    <span class="speed_label">Speed:</span>
    <a href="#" class="button" onclick="setServoSpeed('slow');   return false;">SLOW</a>
    <a href="#" class="button primary" onclick="setServoSpeed('normal'); return false;">NORMAL</a>
    <a href="#" class="button" onclick="setServoSpeed('fast');   return false;">FAST</a>
</div>
)HUY";

// ----- index.sequence.html (2528 bytes) -----
const char WEB_SEQUENCE_HTML[] PROGMEM = R"HUY(
<h4>Sequences</h4>

<div class="menuContainer">
    <a href="#" class="button primary" onclick="sequencePlay('greeting');  return false;">GREETING</a>
    <a href="#" class="button primary" onclick="sequencePlay('surprised'); return false;">SURPRISED</a>
    <a href="#" class="button primary" onclick="sequencePlay('sad');       return false;">SAD</a>
    <a href="#" class="button primary" onclick="sequencePlay('angry');     return false;">ANGRY</a>
    <a href="#" class="button" onclick="sequenceStop(); return false;">STOP</a>
</div>

<hr>

<h4>Custom Sequence Editor</h4>
<div style="font-size:11px; color:var(--text-dim); text-align:left; padding: 0 8px;">
Actions: eye, neck_rot, neck_tilt_fwd, neck_tilt_side, body_rot, body_tilt_fwd, body_tilt_side, audio, audio_stop, auto<br>
EyeState param1: 1=Open, 2=Closed, 3=Blink, 4=Focus, 5=Sad, 6=Angry
</div>
<textarea id="custom_seq_json" rows="14" style="width:100%; background:var(--bg-elev-2); color:var(--text); border:1px solid var(--border); border-radius:6px; padding:8px; font-family:monospace; font-size:11px;">{
  "name": "custom",
  "steps": [
    { "atMs": 0,    "action": "auto",     "param1": 0 },
    { "atMs": 0,    "action": "eye",      "param1": 1 },
    { "atMs": 200,  "action": "neck_rot", "param1": -60, "param2": 600 },
    { "atMs": 900,  "action": "neck_rot", "param1": 60,  "param2": 600 },
    { "atMs": 1600, "action": "neck_rot", "param1": 0,   "param2": 400 },
    { "atMs": 2000, "action": "audio",    "param1": 1 },
    { "atMs": 2500, "action": "auto",     "param1": 1 }
  ]
}</textarea>
<div class="menuContainer">
    <a href="#" class="button" onclick="customSeqLoad(); return false;">Load saved</a>
    <a href="#" class="button" onclick="customSeqSave(); return false;">Save</a>
    <a href="#" class="button primary" onclick="customSeqPlay(); return false;">▶ Play Custom</a>
</div>

<script>
function customSeqLoad() {
    fetch('/sequence/load').then(r => r.json()).then(j => {
        document.getElementById('custom_seq_json').value = JSON.stringify(j, null, 2);
    });
}
function customSeqSave() {
    const txt = document.getElementById('custom_seq_json').value;
    try { JSON.parse(txt); } catch (e) { alert('Invalid JSON: ' + e.message); return; }
    fetch('/sequence/save', { method:'POST', headers:{'Content-Type':'application/json'}, body: txt })
        .then(r => r.json()).then(() => alert('Saved'));
}
function customSeqPlay() {
    customSeqSave();
    setTimeout(() => sequencePlay('custom'), 200);
}
</script>
)HUY";

// ----- javascript.js (19546 bytes) -----
const char WEB_JAVASCRIPT_JS[] PROGMEM = R"HUY(
var automatic = true;

var face_eyes_all = null;
var face_eyes_left = 'blink';
var face_eyes_right = 'blink';

var neck_rotate = 0;
var neck_tiltForward = 0;
var neck_tiltSideways = 0;

var body_rotate = 0;
var body_tiltForward = 0;
var body_tiltSideways = 0;

var JoyNeck;
var JoyNeckX = 0;
var JoyNeckY = 0;

var JoyBody;
var JoyBodyX = 0;
var JoyBodyY = 0;

function sendEyeUpdate(position, action) {
    data = {
        automatic: false,
        face: {
            left: face_eyes_left,
            right: face_eyes_right
        }
    };

    data["face"][position] = action;

    sendData(data);
}

function changeAutomatic(newState) {
    const data = {
        automatic: newState
    };

    sendData(data);
}

// Throttle helper: fires at most once per `ms`, with a trailing call so the last
// position is guaranteed to arrive. Prevents >20 POSTs/s while wiggling joystick.
function throttle(fn, ms) {
    let lastCall = 0;
    let trailing = null;
    return function () {
        const now = Date.now();
        const elapsed = now - lastCall;
        if (elapsed >= ms) {
            lastCall = now;
            fn();
        } else {
            if (trailing) clearTimeout(trailing);
            trailing = setTimeout(() => {
                lastCall = Date.now();
                fn();
                trailing = null;
            }, ms - elapsed);
        }
    };
}

function _sendNeckUpdateNow() {
    const data = {
        automatic: false,
        neck: {
            rotate: JoyNeck.GetX(),
            tiltForward: JoyNeck.GetY(),
            tiltSideways: document.getElementById('slider_neckTiltSideways').value
        }
    };
    sendData(data);
}

function _sendBodyUpdateNow() {
    const data = {
        automatic: false,
        body: {
            rotate: JoyBody.GetX(),
            tiltForward: JoyBody.GetY(),
            tiltSideways: document.getElementById('slider_bodyTiltSideways').value
        }
    };
    sendData(data);
}

const sendNeckUpdate = throttle(_sendNeckUpdateNow, 80);
const sendBodyUpdate = throttle(_sendBodyUpdateNow, 80);

async function postDataJson(url = "", data = {}) {
    try {
        const response = await fetch(url, {
            method: 'POST',
            mode: 'same-origin',
            cache: 'no-cache',
            headers: { 'Content-Type': 'application/json' },
            referrerPolicy: 'no-referrer',
            body: JSON.stringify(data),
        });
        if (!response.ok) throw new Error('HTTP ' + response.status);
        return await response.json();
    } catch (e) {
        setStatus('POST failed: ' + url, 'warning');
        return null;
    }
}

function sendData(data) {
    console.log(data);

    postDataJson('api/post.json', data).then(json => {
        if (!json) return; // Fehler wurde schon in postDataJson signalisiert
        console.log('Result from Server: ');
        console.log(json);

        automatic = json.automatic;

        if (json.face != null) {
            face_eyes_all = json.face.eyes.all;
            face_eyes_left = json.face.eyes.left;
            face_eyes_right = json.face.eyes.right;
        }

        if (json.neck != null) {
            neck_rotate = json.neck.rotate;
            neck_tiltForward = json.neck.tiltForward;
            neck_tiltSideways = json.neck.tiltSideways;
        }

        if (json.body != null) {
            body_rotate = json.body.rotate;
            body_tiltForward = json.body.tiltForward;
            body_tiltSideways = json.body.tiltSideways;
        }

        updateUserInterface();
    });
}

function updateUserInterface() {
    button_automatic = document.getElementById('button_automatic');
    if (automatic == true) {
        button_automatic.innerHTML = "Auto is ON";
    } else {
        button_automatic.innerHTML = "Auto is OFF";
    }

    if (document.getElementById('eye_all_open') != null) {

        button_eye_all_open = document.getElementById('eye_all_open');
        button_eye_all_close = document.getElementById('eye_all_close');
        button_eye_all_blink = document.getElementById('eye_all_blink');
        button_eye_all_focus = document.getElementById('eye_all_focus');
        button_eye_all_sad = document.getElementById('eye_all_sad');
        button_eye_all_angry = document.getElementById('eye_all_angry');

        button_eye_left_open = document.getElementById('eye_left_open');
        button_eye_left_close = document.getElementById('eye_left_close');
        button_eye_left_blink = document.getElementById('eye_left_blink');
        button_eye_left_focus = document.getElementById('eye_left_focus');
        button_eye_left_sad = document.getElementById('eye_left_sad');
        button_eye_left_angry = document.getElementById('eye_left_angry');

        button_eye_right_open = document.getElementById('eye_right_open');
        button_eye_right_close = document.getElementById('eye_right_close');
        button_eye_right_blink = document.getElementById('eye_right_blink');
        button_eye_right_focus = document.getElementById('eye_right_focus');
        button_eye_right_sad = document.getElementById('eye_right_sad');
        button_eye_right_angry = document.getElementById('eye_right_angry');

        activeState = 'selected';

        button_eye_all_open.classList.remove(activeState);
        button_eye_all_close.classList.remove(activeState);
        button_eye_all_blink.classList.remove(activeState);
        button_eye_all_focus.classList.remove(activeState);
        button_eye_all_sad.classList.remove(activeState);
        button_eye_all_angry.classList.remove(activeState);

        button_eye_left_open.classList.remove(activeState);
        button_eye_left_close.classList.remove(activeState);
        button_eye_left_blink.classList.remove(activeState);
        button_eye_left_focus.classList.remove(activeState);
        button_eye_left_sad.classList.remove(activeState);
        button_eye_left_angry.classList.remove(activeState);

        button_eye_right_open.classList.remove(activeState);
        button_eye_right_close.classList.remove(activeState);
        button_eye_right_blink.classList.remove(activeState);
        button_eye_right_focus.classList.remove(activeState);
        button_eye_right_sad.classList.remove(activeState);
        button_eye_right_angry.classList.remove(activeState);

        if (face_eyes_all == 1) {
            button_eye_all_open.classList.add(activeState);
        } else if (face_eyes_all == 2) {
            button_eye_all_close.classList.add(activeState);
        } else if (face_eyes_all == 3) {
            button_eye_all_blink.classList.add(activeState);
        } else if (face_eyes_all == 4) {
            button_eye_all_focus.classList.add(activeState);
        } else if (face_eyes_all == 5) {
            button_eye_all_sad.classList.add(activeState);
        } else if (face_eyes_all == 6) {
            button_eye_all_angry.classList.add(activeState);
        }

        if (face_eyes_left == 1) {
            button_eye_left_open.classList.add(activeState);
        } else if (face_eyes_left == 2) {
            button_eye_left_close.classList.add(activeState);
        } else if (face_eyes_left == 3) {
            button_eye_left_blink.classList.add(activeState);
        } else if (face_eyes_left == 4) {
            button_eye_left_focus.classList.add(activeState);
        } else if (face_eyes_left == 5) {
            button_eye_left_sad.classList.add(activeState);
        } else if (face_eyes_left == 6) {
            button_eye_left_angry.classList.add(activeState);
        }

        if (face_eyes_right == 1) {
            button_eye_right_open.classList.add(activeState);
        } else if (face_eyes_right == 2) {
            button_eye_right_close.classList.add(activeState);
        } else if (face_eyes_right == 3) {
            button_eye_right_blink.classList.add(activeState);
        } else if (face_eyes_right == 4) {
            button_eye_right_focus.classList.add(activeState);
        } else if (face_eyes_right == 5) {
            button_eye_right_sad.classList.add(activeState);
        } else if (face_eyes_right == 6) {
            button_eye_right_angry.classList.add(activeState);
        }
    }

    slider_neckTiltSideways = document.getElementById('slider_neckTiltSideways');
    slider_bodyTiltSideways = document.getElementById('slider_bodyTiltSideways');

    if (slider_neckTiltSideways != null) {
        slider_neckTiltSideways.value = neck_tiltSideways;
    }
    if (slider_bodyTiltSideways != null) {
        slider_bodyTiltSideways.value = body_tiltSideways;
    }

    // Multi-client sync: display the server-known servo state below each joystick
    const neckState = document.getElementById('neck_state');
    if (neckState != null) {
        neckState.innerText = 'rot ' + (neck_rotate || 0) +
            ' | fwd ' + (neck_tiltForward || 0) +
            ' | side ' + (neck_tiltSideways || 0);
    }
    const bodyState = document.getElementById('body_state');
    if (bodyState != null) {
        bodyState.innerText = 'rot ' + (body_rotate || 0) +
            ' | fwd ' + (body_tiltForward || 0) +
            ' | side ' + (body_tiltSideways || 0);
    }
}

function getServerData() {
    const data = {};

    sendData(data);
}

function systemInit() {
    console.log('systemInit started');

    getServerData();

    // Populate status bar immediately and then poll periodically
    pollStatus();
    setInterval(pollStatus, 5000);

    setInterval(getServerData, 2000);

    timeToWait = 0;
    timeToPause = 200;

    if (document.getElementById('container_face') != null) {
        setTimeout(function(){
            loadContainer('index.face.html', 'container_face');
        }, timeToWait);
        timeToWait += timeToPause;
    }

    if (document.getElementById('container_neck') != null) {
        setTimeout(function(){
            loadContainer('index.neck.html', 'container_neck');
        }, timeToWait);
        timeToWait += timeToPause;
    }

    if (document.getElementById('container_body') != null) {
        setTimeout(function(){
            loadContainer('index.body.html', 'container_body');
        }, timeToWait);
        timeToWait += timeToPause;
    }

    if (document.getElementById('container_audio') != null) {
        setTimeout(function(){
            loadContainer('index.audio.html', 'container_audio');
        }, timeToWait);
        timeToWait += timeToPause;
    }

    if (document.getElementById('container_sequence') != null) {
        setTimeout(function(){
            loadContainer('index.sequence.html', 'container_sequence');
        }, timeToWait);
        timeToWait += timeToPause;
    }
}

// =========================================================================
// Status bar + error handler (v2.3+)
// =========================================================================
function setStatus(text, klass) {
    const bar = document.getElementById('status_bar');
    const txt = document.getElementById('status_text');
    if (!bar || !txt) return;
    txt.innerText = text;
    bar.classList.remove('error', 'warning');
    if (klass) bar.classList.add(klass);
}

function setNetInfo(text) {
    const el = document.getElementById('status_net');
    if (el) el.innerText = text || '';
}

async function pollStatus() {
    try {
        const r = await fetch('/status', { method: 'GET', cache: 'no-cache', mode: 'same-origin' });
        if (!r.ok) throw new Error('HTTP ' + r.status);
        const s = await r.json();
        let txt = 'WiFi: ' + (s.wifi ? s.wifi.mode : '?');
        if (s.mqtt && s.mqtt.connected) txt += ' | MQTT';
        if (s.sequence && s.sequence.playing) {
            txt += ' | ' + (s.sequence.name || 'sequence');
        }
        // Progress-Bar updaten
        const pb = document.getElementById('status_progress_bar');
        if (pb) {
            const p = (s.sequence && s.sequence.playing) ? Math.round((s.sequence.progress || 0) * 100) : 0;
            pb.style.width = p + '%';
        }
        if (s.heap !== undefined) {
            const kb = Math.round(s.heap / 1024);
            txt += ' | heap ' + kb + 'k';
            // bei knappem Heap (<8k) gelb warnen
            if (kb < 8) { setStatus(txt, 'warning'); setNetInfo(s.wifi ? s.wifi.ip : ''); return; }
        }
        setStatus(txt, null);
        setNetInfo(s.wifi ? s.wifi.ip : '');
    } catch (e) {
        setStatus('offline / connection error', 'error');
        setNetInfo('');
    }
}

// =========================================================================
// Audio + Sequence API calls (v2.3+)
// =========================================================================
async function getJson(url) {
    try {
        const r = await fetch(url, { method: 'GET', cache: 'no-cache', mode: 'same-origin' });
        if (!r.ok) throw new Error('HTTP ' + r.status);
        return await r.json();
    } catch (e) {
        setStatus('request failed: ' + url, 'warning');
        // Status bar will be overwritten on the next pollStatus call
        return null;
    }
}

function audioPlay() {
    const track = document.getElementById('audio_track').value || 1;
    getJson('/audio/play?track=' + encodeURIComponent(track));
}

function audioStop() {
    getJson('/audio/stop');
}

function audioVolume() {
    const v = document.getElementById('slider_audioVolume').value;
    const label = document.getElementById('audio_volume_label');
    if (label) label.innerText = v;
    getJson('/audio/volume?v=' + encodeURIComponent(v));
    const btn = document.getElementById('audio_mute_btn');
    if (btn) btn.innerText = (parseInt(v) === 0) ? '🔇' : '🔊';
}

let _preMuteVolume = null;
function audioMute() {
    const slider = document.getElementById('slider_audioVolume');
    const label = document.getElementById('audio_volume_label');
    const btn = document.getElementById('audio_mute_btn');
    if (!slider) return;
    const current = parseInt(slider.value);
    if (current > 0) {
        _preMuteVolume = current;
        slider.value = 0;
        if (label) label.innerText = '0';
        if (btn) btn.innerText = '🔇';
        getJson('/audio/volume?v=0');
    } else {
        const restore = _preMuteVolume || 20;
        slider.value = restore;
        if (label) label.innerText = restore;
        if (btn) btn.innerText = '🔊';
        getJson('/audio/volume?v=' + restore);
        _preMuteVolume = null;
    }
}

function sequencePlay(name) {
    getJson('/sequence?name=' + encodeURIComponent(name));
}

function sequenceStop() {
    getJson('/sequence/stop');
}

function setServoSpeed(preset) {
    getJson('/servo/speed?preset=' + encodeURIComponent(preset));
}

function sendEyeColor() {
    const picker = document.getElementById('eye_color');
    const label = document.getElementById('eye_color_label');
    if (!picker) return;
    const hex = picker.value.replace('#', '');
    if (label) label.innerText = '#' + hex;
    document.documentElement.style.setProperty('--eye-color', '#' + hex);
    getJson('/eye/color?hex=' + encodeURIComponent(hex));
}

function setEyeColorPreset(hex) {
    const picker = document.getElementById('eye_color');
    if (picker) picker.value = '#' + hex;
    sendEyeColor();
}

function sendClosedEyeColor() {
    const picker = document.getElementById('closed_eye_color');
    const label = document.getElementById('closed_eye_color_label');
    if (!picker) return;
    const hex = picker.value.replace('#', '');
    if (label) label.innerText = '#' + hex;
    getJson('/eye/closedcolor?hex=' + encodeURIComponent(hex));
}

function sendPupilSize() {
    const slider = document.getElementById('slider_pupilSize');
    const label = document.getElementById('pupil_size_label');
    if (!slider) return;
    if (label) label.innerText = slider.value;
    getJson('/eye/pupil?size=' + encodeURIComponent(slider.value));
}

function sendPupilSettings() {
    const cb = document.getElementById('pupil_enabled');
    const picker = document.getElementById('pupil_color');
    const label = document.getElementById('pupil_color_label');
    if (!cb || !picker) return;
    const hex = picker.value.replace('#', '');
    if (label) label.innerText = '#' + hex;
    const params = 'enabled=' + (cb.checked ? 1 : 0) + '&hex=' + encodeURIComponent(hex);
    getJson('/eye/pupil?' + params);
}

// =========================================================================
// Theme toggle (client-side only, persisted in localStorage)
// =========================================================================
function applyTheme(theme) {
    if (theme === 'light') {
        document.documentElement.setAttribute('data-theme', 'light');
    } else {
        document.documentElement.removeAttribute('data-theme');
    }
    // Icon zeigt was passiert beim Klick: im Light-Mode den Moon (wechselt zu Dark), im Dark-Mode die Sonne
    const btn = document.getElementById('button_theme');
    if (btn) btn.innerText = (theme === 'light') ? '☾' : '☀';
}

function toggleTheme() {
    const current = document.documentElement.getAttribute('data-theme');
    const next = (current === 'light') ? 'dark' : 'light';
    try { localStorage.setItem('huyang.theme', next); } catch (e) {}
    applyTheme(next);
}

function toggleFullscreen() {
    const el = document.documentElement;
    if (!document.fullscreenElement) {
        if (el.requestFullscreen) el.requestFullscreen();
        else if (el.webkitRequestFullscreen) el.webkitRequestFullscreen();
    } else {
        if (document.exitFullscreen) document.exitFullscreen();
        else if (document.webkitExitFullscreen) document.webkitExitFullscreen();
    }
}
document.addEventListener('fullscreenchange', () => {
    const btn = document.getElementById('button_fullscreen');
    if (btn) btn.innerText = document.fullscreenElement ? '🗗' : '⛶';
});

// Apply immediately at script load (before systemInit so no flicker).
// Note: runs BEFORE DOMContentLoaded, so the icon will be re-applied later when the button exists.
function _initTheme() {
    try {
        const saved = localStorage.getItem('huyang.theme') || 'dark';
        applyTheme(saved);
    } catch (e) { applyTheme('dark'); }
}
_initTheme();
// Run again after DOMContentLoaded so the button (if loaded later) gets its icon
if (document.readyState !== 'loading') _initTheme();
else document.addEventListener('DOMContentLoaded', _initTheme);

function initJoystick() {

    fillColor = '#fd2';
    strokeColor = '#b90';

    joystickSettings = {
        autoReturnToCenter: true,
        internalFillColor: fillColor,
        internalLineWidth: 2,
        internalStrokeColor: strokeColor,
        externalLineWidth: 2,
        externalStrokeColor: strokeColor
    };

    if (JoyNeck == null && document.getElementById('joyNeck') != null) {
        JoyNeck = new JoyStick('joyNeck', joystickSettings, function (stickData) {

            if (JoyNeckX != stickData.x || JoyNeckY != stickData.y) {

                JoyNeckX = stickData.x;
                JoyNeckY = stickData.y;

                sendNeckUpdate();
            }
        });
    }

    if (JoyBody == null && document.getElementById('joyBody') != null) {
        JoyBody = new JoyStick('joyBody', joystickSettings, function (stickData) {

            if (JoyBodyX != stickData.x || JoyBodyY != stickData.y) {

                JoyBodyX = stickData.x;
                JoyBodyY = stickData.y;

                sendBodyUpdate();
            }
        });
    }
})HUY";

// ----- joystick.js (16361 bytes) -----
const char WEB_JOYSTICK_JS[] PROGMEM = R"HUY(
/*
 * Name          : joy.js
 * @author       : Roberto D'Amico (Bobboteck)
 * Last modified : 09.06.2020
 * Revision      : 1.1.6
 *
 * Modification History:
 * Date         Version     Modified By     Description
 * 2021-12-21   2.0.0       Roberto D'Amico New version of the project that integrates the callback functions, while 
 *                                          maintaining compatibility with previous versions. Fixed Issue #27 too, 
 *                                          thanks to @artisticfox8 for the suggestion.
 * 2020-06-09   1.1.6       Roberto D'Amico Fixed Issue #10 and #11
 * 2020-04-20   1.1.5       Roberto D'Amico Correct: Two sticks in a row, thanks to @liamw9534 for the suggestion
 * 2020-04-03               Roberto D'Amico Correct: InternalRadius when change the size of canvas, thanks to 
 *                                          @vanslipon for the suggestion
 * 2020-01-07   1.1.4       Roberto D'Amico Close #6 by implementing a new parameter to set the functionality of 
 *                                          auto-return to 0 position
 * 2019-11-18   1.1.3       Roberto D'Amico Close #5 correct indication of East direction
 * 2019-11-12   1.1.2       Roberto D'Amico Removed Fix #4 incorrectly introduced and restored operation with touch 
 *                                          devices
 * 2019-11-12   1.1.1       Roberto D'Amico Fixed Issue #4 - Now JoyStick work in any position in the page, not only 
 *                                          at 0,0
 * 
 * The MIT License (MIT)
 *
 *  This file is part of the JoyStick Project (https://github.com/bobboteck/JoyStick).
 *	Copyright (c) 2015 Roberto D'Amico (Bobboteck).
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

 let StickStatus =
 {
     xPosition: 0,
     yPosition: 0,
     x: 0,
     y: 0,
     cardinalDirection: "C"
 };
 
 /**
  * @desc Principal object that draw a joystick, you only need to initialize the object and suggest the HTML container
  * @costructor
  * @param container {String} - HTML object that contains the Joystick
  * @param parameters (optional) - object with following keys:
  *  title {String} (optional) - The ID of canvas (Default value is 'joystick')
  *  width {Int} (optional) - The width of canvas, if not specified is setted at width of container object (Default value is the width of container object)
  *  height {Int} (optional) - The height of canvas, if not specified is setted at height of container object (Default value is the height of container object)
  *  internalFillColor {String} (optional) - Internal color of Stick (Default value is '#00AA00')
  *  internalLineWidth {Int} (optional) - Border width of Stick (Default value is 2)
  *  internalStrokeColor {String}(optional) - Border color of Stick (Default value is '#003300')
  *  externalLineWidth {Int} (optional) - External reference circonference width (Default value is 2)
  *  externalStrokeColor {String} (optional) - External reference circonference color (Default value is '#008000')
  *  autoReturnToCenter {Bool} (optional) - Sets the behavior of the stick, whether or not, it should return to zero position when released (Default value is True and return to zero)
  * @param callback {StickStatus} - 
  */
 var JoyStick = (function(container, parameters, callback)
 {
     parameters = parameters || {};
     var title = (typeof parameters.title === "undefined" ? "joystick" : parameters.title),
         width = (typeof parameters.width === "undefined" ? 0 : parameters.width),
         height = (typeof parameters.height === "undefined" ? 0 : parameters.height),
         internalFillColor = (typeof parameters.internalFillColor === "undefined" ? "#00AA00" : parameters.internalFillColor),
         internalLineWidth = (typeof parameters.internalLineWidth === "undefined" ? 2 : parameters.internalLineWidth),
         internalStrokeColor = (typeof parameters.internalStrokeColor === "undefined" ? "#003300" : parameters.internalStrokeColor),
         externalLineWidth = (typeof parameters.externalLineWidth === "undefined" ? 2 : parameters.externalLineWidth),
         externalStrokeColor = (typeof parameters.externalStrokeColor ===  "undefined" ? "#008000" : parameters.externalStrokeColor),
         autoReturnToCenter = (typeof parameters.autoReturnToCenter === "undefined" ? true : parameters.autoReturnToCenter);
 
     callback = callback || function(StickStatus) {};
 
     // Create Canvas element and add it in the Container object
     var objContainer = document.getElementById(container);
     
     // Fixing Unable to preventDefault inside passive event listener due to target being treated as passive in Chrome [Thanks to https://github.com/artisticfox8 for this suggestion]
     objContainer.style.touchAction = "none";
 
     var canvas = document.createElement("canvas");
     canvas.id = title;
     if(width === 0) { width = objContainer.clientWidth; }
     if(height === 0) { height = objContainer.clientHeight; }
     canvas.width = width;
     canvas.height = height;
     objContainer.appendChild(canvas);
     var context=canvas.getContext("2d");
 
     var pressed = 0; // Bool - 1=Yes - 0=No
     var circumference = 2 * Math.PI;
     var internalRadius = (canvas.width-((canvas.width/2)+10))/2;
     var maxMoveStick = internalRadius + 5;
     var externalRadius = internalRadius + 30;
     var centerX = canvas.width / 2;
     var centerY = canvas.height / 2;
     var directionHorizontalLimitPos = canvas.width / 10;
     var directionHorizontalLimitNeg = directionHorizontalLimitPos * -1;
     var directionVerticalLimitPos = canvas.height / 10;
     var directionVerticalLimitNeg = directionVerticalLimitPos * -1;
     // Used to save current position of stick
     var movedX=centerX;
     var movedY=centerY;
 
     // Check if the device support the touch or not
     if("ontouchstart" in document.documentElement)
     {
         canvas.addEventListener("touchstart", onTouchStart, false);
         document.addEventListener("touchmove", onTouchMove, false);
         document.addEventListener("touchend", onTouchEnd, false);
     }
     else
     {
         canvas.addEventListener("mousedown", onMouseDown, false);
         document.addEventListener("mousemove", onMouseMove, false);
         document.addEventListener("mouseup", onMouseUp, false);
     }
     // Draw the object
     drawExternal();
     drawInternal();
 
     /******************************************************
      * Private methods
      *****************************************************/
 
     /**
      * @desc Draw the external circle used as reference position
      */
     function drawExternal()
     {
         context.beginPath();
         context.arc(centerX, centerY, externalRadius, 0, circumference, false);
         context.lineWidth = externalLineWidth;
         context.strokeStyle = externalStrokeColor;
         context.stroke();
     }
 
     /**
      * @desc Draw the internal stick in the current position the user have moved it
      */
     function drawInternal()
     {
         context.beginPath();
         if(movedX<internalRadius) { movedX=maxMoveStick; }
         if((movedX+internalRadius) > canvas.width) { movedX = canvas.width-(maxMoveStick); }
         if(movedY<internalRadius) { movedY=maxMoveStick; }
         if((movedY+internalRadius) > canvas.height) { movedY = canvas.height-(maxMoveStick); }
         context.arc(movedX, movedY, internalRadius, 0, circumference, false);
         // create radial gradient
         var grd = context.createRadialGradient(centerX, centerY, 5, centerX, centerY, 200);
         // Light color
         grd.addColorStop(0, internalFillColor);
         // Dark color
         grd.addColorStop(1, internalStrokeColor);
         context.fillStyle = grd;
         context.fill();
         context.lineWidth = internalLineWidth;
         context.strokeStyle = internalStrokeColor;
         context.stroke();
     }
 
     /**
      * @desc Events for manage touch
      */
     let touchId = null;
     function onTouchStart(event)
     {
         pressed = 1;
         touchId = event.targetTouches[0].identifier;
     }
 
     function onTouchMove(event)
     {
         if(pressed === 1 && event.targetTouches[0].target === canvas)
         {
             movedX = event.targetTouches[0].pageX;
             movedY = event.targetTouches[0].pageY;
             // Manage offset
             if(canvas.offsetParent.tagName.toUpperCase() === "BODY")
             {
                 movedX -= canvas.offsetLeft;
                 movedY -= canvas.offsetTop;
             }
             else
             {
                 movedX -= canvas.offsetParent.offsetLeft;
                 movedY -= canvas.offsetParent.offsetTop;
             }
             // Delete canvas
             context.clearRect(0, 0, canvas.width, canvas.height);
             // Redraw object
             drawExternal();
             drawInternal();
 
             // Set attribute of callback
             StickStatus.xPosition = movedX;
             StickStatus.yPosition = movedY;
             StickStatus.x = (100*((movedX - centerX)/maxMoveStick)).toFixed();
             StickStatus.y = ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();
             StickStatus.cardinalDirection = getCardinalDirection();
             callback(StickStatus);
         }
     }
 
     function onTouchEnd(event)
     {
         if (event.changedTouches[0].identifier !== touchId) return;
 
         pressed = 0;
         // If required reset position store variable
         if(autoReturnToCenter)
         {
             movedX = centerX;
             movedY = centerY;
         }
         // Delete canvas
         context.clearRect(0, 0, canvas.width, canvas.height);
         // Redraw object
         drawExternal();
         drawInternal();
 
         // Set attribute of callback
         StickStatus.xPosition = movedX;
         StickStatus.yPosition = movedY;
         StickStatus.x = (100*((movedX - centerX)/maxMoveStick)).toFixed();
         StickStatus.y = ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();
         StickStatus.cardinalDirection = getCardinalDirection();
         callback(StickStatus);
     }
 
     /**
      * @desc Events for manage mouse
      */
     function onMouseDown(event) 
     {
         pressed = 1;
     }
 
     /* To simplify this code there was a new experimental feature here: https://developer.mozilla.org/en-US/docs/Web/API/MouseEvent/offsetX , but it present only in Mouse case not metod presents in Touch case :-( */
     function onMouseMove(event) 
     {
         if(pressed === 1)
         {
             movedX = event.pageX;
             movedY = event.pageY;
             // Manage offset
             if(canvas.offsetParent.tagName.toUpperCase() === "BODY")
             {
                 movedX -= canvas.offsetLeft;
                 movedY -= canvas.offsetTop;
             }
             else
             {
                 movedX -= canvas.offsetParent.offsetLeft;
                 movedY -= canvas.offsetParent.offsetTop;
             }
             // Delete canvas
             context.clearRect(0, 0, canvas.width, canvas.height);
             // Redraw object
             drawExternal();
             drawInternal();
 
             // Set attribute of callback
             StickStatus.xPosition = movedX;
             StickStatus.yPosition = movedY;
             StickStatus.x = (100*((movedX - centerX)/maxMoveStick)).toFixed();
             StickStatus.y = ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();
             StickStatus.cardinalDirection = getCardinalDirection();
             callback(StickStatus);
         }
     }
 
     function onMouseUp(event) 
     {
         pressed = 0;
         // If required reset position store variable
         if(autoReturnToCenter)
         {
             movedX = centerX;
             movedY = centerY;
         }
         // Delete canvas
         context.clearRect(0, 0, canvas.width, canvas.height);
         // Redraw object
         drawExternal();
         drawInternal();
 
         // Set attribute of callback
         StickStatus.xPosition = movedX;
         StickStatus.yPosition = movedY;
         StickStatus.x = (100*((movedX - centerX)/maxMoveStick)).toFixed();
         StickStatus.y = ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();
         StickStatus.cardinalDirection = getCardinalDirection();
         callback(StickStatus);
     }
 
     function getCardinalDirection()
     {
         let result = "";
         let orizontal = movedX - centerX;
         let vertical = movedY - centerY;
         
         if(vertical >= directionVerticalLimitNeg && vertical <= directionVerticalLimitPos)
         {
             result = "C";
         }
         if(vertical < directionVerticalLimitNeg)
         {
             result = "N";
         }
         if(vertical > directionVerticalLimitPos)
         {
             result = "S";
         }
         
         if(orizontal < directionHorizontalLimitNeg)
         {
             if(result === "C")
             { 
                 result = "W";
             }
             else
             {
                 result += "W";
             }
         }
         if(orizontal > directionHorizontalLimitPos)
         {
             if(result === "C")
             { 
                 result = "E";
             }
             else
             {
                 result += "E";
             }
         }
         
         return result;
     }
 
     /******************************************************
      * Public methods
      *****************************************************/
 
     /**
      * @desc The width of canvas
      * @return Number of pixel width 
      */
     this.GetWidth = function () 
     {
         return canvas.width;
     };
 
     /**
      * @desc The height of canvas
      * @return Number of pixel height
      */
     this.GetHeight = function () 
     {
         return canvas.height;
     };
 
     /**
      * @desc The X position of the cursor relative to the canvas that contains it and to its dimensions
      * @return Number that indicate relative position
      */
     this.GetPosX = function ()
     {
         return movedX;
     };
 
     /**
      * @desc The Y position of the cursor relative to the canvas that contains it and to its dimensions
      * @return Number that indicate relative position
      */
     this.GetPosY = function ()
     {
         return movedY;
     };
 
     /**
      * @desc Normalizzed value of X move of stick
      * @return Integer from -100 to +100
      */
     this.GetX = function ()
     {
         return (100*((movedX - centerX)/maxMoveStick)).toFixed();
     };
 
     /**
      * @desc Normalizzed value of Y move of stick
      * @return Integer from -100 to +100
      */
     this.GetY = function ()
     {
         return ((100*((movedY - centerY)/maxMoveStick))*-1).toFixed();
     };
 
     /**
      * @desc Get the direction of the cursor as a string that indicates the cardinal points where this is oriented
      * @return String of cardinal point N, NE, E, SE, S, SW, W, NW and C when it is placed in the center
      */
     this.GetDir = function()
     {
         return getCardinalDirection();
     };
 });)HUY";

// ----- settings.html (5977 bytes) -----
const char WEB_SETTINGS_HTML[] PROGMEM = R"HUY(
<h4>Settings</h4>

<div class="menuContainer">
    <a href="/index.html" class="button">← Start</a>
    <a href="/calibration.html" class="button">Calibration</a>
    <a href="/api" class="button">API Docs</a>
</div>

<hr>

<h4>Features</h4>
<div id="features_container" style="text-align: left; padding: 0 12px;">
    Loading…
</div>
<div class="menuContainer">
    <a href="#" class="button primary" onclick="saveFeatures(); return false;">Save Features</a>
</div>

<hr>

<h4>WiFi</h4>
<div id="wifi_container" style="text-align: left; padding: 0 12px;">
    <div style="font-size:12px; color: var(--text-dim); margin-bottom: 8px;">Scan + connect in AP mode if STA was lost.</div>
    <div class="menuContainer">
        <a href="#" class="button" onclick="wifiScan(); return false;">🔄 Scan</a>
    </div>
    <div id="wifi_list" style="margin-top: 8px;"></div>
    <div style="font-size:12px; color: var(--text-dim); margin: 12px 0 4px 0;">Or enter manually:</div>
    <input id="wifi_ssid_in" placeholder="SSID" class="audio_input" style="width: 90%;" /><br>
    <input id="wifi_pass_in" placeholder="Password" type="password" class="audio_input" style="width: 90%; margin-top: 6px;" /><br>
    <div class="menuContainer">
        <a href="#" class="button primary" onclick="wifiConnect(); return false;">Connect</a>
    </div>
</div>

<hr>

<h4>System</h4>
<div class="menuContainer">
    <a href="#" class="button" onclick="configBackup(); return false;">⬇ Backup Config</a>
    <a href="#" class="button" onclick="document.getElementById('config_upload').click(); return false;">⬆ Restore Config</a>
    <input id="config_upload" type="file" accept="application/json" style="display:none" onchange="configRestore(this)" />
    <a href="#" class="button" onclick="runSelfTest(); return false;">▶ Run Self-Test</a>
</div>
<div class="menuContainer">
    <a href="#" class="button" style="border-color:#c33" onclick="rebootEsp(); return false;">⟳ Reboot ESP</a>
    <a href="#" class="button" style="border-color:#c33" onclick="factoryReset(); return false;">⚠ Factory Reset</a>
</div>

<script>
// ====== Features ======
const FEATURE_KEYS = ['enableEyes','enableMonacle','enableNeckMovement','enableHeadRotation','enableBodyMovement','enableBodyRotation','enableTorsoLights'];
function loadFeatures() {
    fetch('/config/get').then(r => r.json()).then(cfg => {
        const c = document.getElementById('features_container');
        c.innerHTML = '';
        FEATURE_KEYS.forEach(k => {
            const id = 'feat_' + k;
            const checked = cfg[k] ? 'checked' : '';
            const label = k.replace('enable', '');
            c.innerHTML += '<label style="display:block; padding:6px 0;"><input type="checkbox" id="' + id + '" ' + checked + ' style="width:20px;height:20px;vertical-align:middle;margin-right:8px;accent-color:var(--accent)"> ' + label + '</label>';
        });
    }).catch(() => {
        document.getElementById('features_container').innerHTML = '<span style="color:var(--danger)">Could not load config</span>';
    });
}
function saveFeatures() {
    const data = {};
    FEATURE_KEYS.forEach(k => { data[k] = document.getElementById('feat_' + k).checked; });
    fetch('/config/set', { method: 'POST', headers: {'Content-Type':'application/json'}, body: JSON.stringify(data) })
        .then(r => r.json()).then(() => alert('Saved. Reboot may be required.'));
}

// ====== WiFi ======
function wifiScan() {
    const list = document.getElementById('wifi_list');
    list.innerHTML = 'scanning…';
    fetch('/wifi/scan').then(r => r.json()).then(d => {
        if (!d.networks || d.networks.length === 0) { list.innerHTML = '(keine Netze gefunden)'; return; }
        list.innerHTML = d.networks.map(n =>
            '<div style="padding:6px; border-bottom:1px solid var(--border); cursor:pointer;" onclick="pickWifi(\'' + n.ssid.replace(/'/g, '') + '\')">'
            + n.ssid + ' <span style="color:var(--text-dim); float:right;">' + n.rssi + ' dBm</span></div>'
        ).join('');
    });
}
function pickWifi(ssid) { document.getElementById('wifi_ssid_in').value = ssid; }
function wifiConnect() {
    const ssid = document.getElementById('wifi_ssid_in').value;
    const pass = document.getElementById('wifi_pass_in').value;
    if (!ssid) { alert('SSID fehlt'); return; }
    fetch('/wifi/connect?ssid=' + encodeURIComponent(ssid) + '&pass=' + encodeURIComponent(pass))
        .then(r => r.json()).then(() => alert('WiFi config saved. Reboot for new STA attempt.'));
}

// ====== System ======
function configBackup() {
    fetch('/config/get').then(r => r.json()).then(cfg => {
        const blob = new Blob([JSON.stringify(cfg, null, 2)], {type: 'application/json'});
        const a = document.createElement('a');
        a.href = URL.createObjectURL(blob);
        a.download = 'huyang-config-' + new Date().toISOString().slice(0,10) + '.json';
        a.click();
    });
}
function configRestore(input) {
    const file = input.files[0];
    if (!file) return;
    const r = new FileReader();
    r.onload = function (e) {
        if (!confirm('Restore config from ' + file.name + '? Current settings will be overwritten.')) return;
        fetch('/config/set', { method:'POST', headers:{'Content-Type':'application/json'}, body: e.target.result })
            .then(r => r.json()).then(() => alert('Restored. Reboot recommended.'));
    };
    r.readAsText(file);
}
function runSelfTest() { fetch('/test/run').then(() => alert('Self-Test running, takes ~23s')); }
function rebootEsp() {
    if (!confirm('Really reboot the ESP?')) return;
    fetch('/reboot').then(() => alert('Reboot sent, wait ~5s...'));
}
function factoryReset() {
    if (!confirm('Factory reset erases ALL settings (WiFi, features). Really proceed?')) return;
    if (!confirm('Really really? This cannot be undone.')) return;
    fetch('/config/reset').then(() => alert('Reset done. Reboot follows.'));
}

loadFeatures();
</script>
)HUY";

// ----- styles.css (9364 bytes) -----
const char WEB_STYLES_CSS[] PROGMEM = R"HUY(
/* ============================================================================
   Huyang Remote Control v2.4 - Responsive UI
   Mobile-first, scales 320px ... 1920px+
   Dark theme, Huyang-yellow accents (#fd2)
   ============================================================================ */

:root {
	--bg: #0a0a0a;
	--bg-elev: #1a1a1a;
	--bg-elev-2: #242424;
	--border: #333;
	--text: #eee;
	--text-dim: #888;
	--accent: #fd2;           /* UI accent (fixed, never changes) */
	--accent-dark: #b90;
	--eye-color: #ffdd22;     /* current eye color - changes with user picker */
	--eye-color-dark: #b90;   /* Border-Variante der Augenfarbe */
	--pupil-color: #000000;
	--danger: #c33;
	--ok: #6c6;
	--warn: #fa3;
}

/* Light theme - slightly gray, not stark white */
[data-theme="light"] {
	--bg: #c8c8cc;          /* main background - medium gray, not pure white */
	--bg-elev: #d8d8dc;     /* sections - slightly lighter */
	--bg-elev-2: #b8b8bc;   /* slider/input bg - slightly darker */
	--border: #9090a0;
	--text: #1a1a1c;
	--text-dim: #50505a;
	/* Accent + eye color remain user-controlled, untouched by theme */
}

* { box-sizing: border-box; }

body {
	margin: 0;
	padding: 36px 12px 24px 12px;
	font-family: 'Helvetica Neue', 'Helvetica', 'Arial', sans-serif;
	font-size: 14px;
	background-color: var(--bg);
	color: var(--text);
	text-align: center;
	min-height: 100vh;
}

/* Container - max-width so it doesn't sprawl on desktop */
.app { max-width: 720px; margin: 0 auto; }

h4 { margin: 0 0 8px 0; font-size: 15px; color: var(--text-dim); text-transform: uppercase; letter-spacing: 1px; font-weight: 600; }
hr { border: none; border-top: 1px solid var(--border); margin: 16px 0; }

/* ===== Status-Bar oben ===== */
.status_bar {
	position: fixed; top: 0; left: 0; right: 0; height: 28px;
	line-height: 28px;
	font-size: 11px; letter-spacing: 0.5px;
	background-color: var(--bg-elev); color: var(--text-dim);
	text-align: center;
	z-index: 9999;
	border-bottom: 1px solid var(--border);
	transition: background-color 0.3s;
}
.status_bar .status_net { color: var(--ok); margin-left: 12px; }
.status_bar.error { background-color: #5a1010; color: #fff; }
.status_bar.warning { background-color: #5a3a10; color: #fff; }

/* Sequence-Progress-Bar in der Status-Bar */
.status_progress {
	position: absolute; bottom: 0; left: 0; right: 0;
	height: 2px; background: transparent;
}
#status_progress_bar {
	height: 100%; width: 0%;
	background: var(--accent);
	transition: width 0.4s linear;
}
.speed_label { font-size: 12px; color: var(--text-dim); align-self: center; margin-right: 4px; }

/* ===== Buttons ===== */
.button {
	display: inline-block;
	padding: 10px 16px;
	margin: 4px;
	background-color: var(--bg-elev);
	color: var(--text);
	text-decoration: none;
	border: 1px solid var(--border);
	border-radius: 6px;
	font-size: 13px;
	cursor: pointer;
	transition: background-color 0.15s, border-color 0.15s;
	min-height: 44px; /* touch target */
	line-height: 24px;
}
.button:hover { background-color: var(--bg-elev-2); border-color: var(--accent-dark); }
.button.primary { background-color: var(--accent); color: #000; border-color: var(--accent); }
.button.primary:hover { background-color: var(--accent-dark); }

a:link, a:visited, a:active { color: var(--text); text-decoration: none; }

.menuContainer {
	display: flex;
	flex-wrap: wrap;
	justify-content: center;
	align-items: center;
	gap: 4px;
	margin: 0 auto;
}

/* ===== Card-Section ===== */
.section {
	background-color: var(--bg-elev);
	border: 1px solid var(--border);
	border-radius: 10px;
	padding: 14px;
	margin: 12px 0;
}

/* ===== Face / Eyes ===== */
.menu_eye_buttons,
.eye_buttons {
	display: flex;
	flex-wrap: wrap;
	justify-content: center;
	gap: 8px;
	margin: 8px 0;
}

.menu_eye_buttons > div,
.eye_buttons > div {
	width: 56px; height: 56px;
	border-radius: 50%;
	border: 3px solid var(--accent);
	display: flex; align-items: center; justify-content: center;
	font-size: 10px; font-weight: 600;
	color: #000;
	cursor: pointer;
	position: relative;
	overflow: hidden;
	transition: transform 0.1s, border-color 0.15s;
	user-select: none;
}
.menu_eye_buttons > div:active,
.eye_buttons > div:active { transform: scale(0.92); }

.eye_open { background-color: var(--accent); }
.eye_close { background-color: #000; color: var(--accent); }
.eye_focus { background-color: #000; }
.eye_focus > div {
	width: 100%; height: 16px;
	background-color: var(--accent);
	position: absolute; top: 50%; transform: translateY(-50%);
}
.eye_sad, .eye_angry { background-color: var(--accent); position: relative; }
.eye_sad > div, .eye_angry > div {
	width: calc(100% + 16px); height: 28px;
	position: absolute; background-color: #000; top: -16px;
}
.eye_sad > .left, .eye_angry > .left { left: 0; transform: rotate(25deg); }
.eye_sad > .right, .eye_angry > .right { left: -16px; transform: rotate(-25deg); }

.selected { border-color: var(--danger) !important; box-shadow: 0 0 0 2px rgba(204, 51, 51, 0.3); }

/* Big preview eye circles */
.eye_preview_row {
	display: flex;
	justify-content: center;
	gap: 24px;
	margin: 12px 0;
	flex-wrap: wrap;
}
.eyeContainer {
	display: flex; flex-direction: column; align-items: center; gap: 8px;
	flex: 1 1 200px;
	max-width: 280px;
}
.eye {
	width: 90px; height: 90px;
	border-radius: 50%;
	background-color: var(--eye-color);
	border: 4px solid var(--eye-color);
	display: flex; align-items: center; justify-content: center;
	cursor: pointer;
	transition: transform 0.1s, background-color 0.2s, border-color 0.2s;
}
.eye:active { transform: scale(0.95); }
.eye > h5 { margin: 0; font-size: 12px; color: #000; font-weight: 700; }

/* Color picker for eyes */
.eye_color_row {
	display: flex; justify-content: center; align-items: center;
	gap: 10px; margin: 8px 0;
	font-size: 12px; color: var(--text-dim);
}
.eye_color_row input[type="color"] {
	width: 44px; height: 44px; border: 2px solid var(--border);
	border-radius: 8px; background: none; cursor: pointer; padding: 2px;
}
.eye_color_row input[type="checkbox"] {
	width: 20px; height: 20px; vertical-align: middle; margin-right: 6px;
	accent-color: var(--accent); cursor: pointer;
}
.eye_color_row label { cursor: pointer; user-select: none; }

.preset_row {
	display: flex; justify-content: center; gap: 8px;
	flex-wrap: wrap; margin: 4px 0 12px 0;
}
.preset_chip {
	width: 36px; height: 36px; border-radius: 50%;
	border: 2px solid var(--border); cursor: pointer;
	transition: transform 0.1s, border-color 0.15s;
	display: inline-block;
}
.preset_chip:hover { border-color: var(--accent); }
.preset_chip:active { transform: scale(0.9); }

/* ===== Joystick ===== */
.joystickContainer { display: flex; justify-content: center; margin: 8px 0; }
.joystick {
	width: 240px; height: 240px;
	cursor: pointer;
	touch-action: none;
}

.remote_state {
	font-size: 11px;
	color: var(--text-dim);
	font-family: 'SF Mono', 'Menlo', 'Courier New', monospace;
	margin: 4px 0 8px 0;
	letter-spacing: 0.5px;
}

/* ===== Slider ===== */
.slidecontainer {
	margin: 8px 0;
	font-size: 12px;
	color: var(--text-dim);
}
.slider {
	-webkit-appearance: none;
	appearance: none;
	width: 100%;
	height: 28px;
	background: var(--bg-elev-2);
	outline: none;
	border-radius: 14px;
	border: 1px solid var(--border);
	margin: 6px 0;
}
.slider::-webkit-slider-thumb {
	-webkit-appearance: none; appearance: none;
	width: 36px; height: 36px;
	background: var(--accent);
	cursor: pointer;
	border-radius: 50%;
	border: 2px solid var(--accent-dark);
}
.slider::-moz-range-thumb {
	width: 36px; height: 36px;
	background: var(--accent);
	cursor: pointer;
	border-radius: 50%;
	border: 2px solid var(--accent-dark);
}

/* ===== Audio ===== */
.audio_controls {
	display: flex; justify-content: center; align-items: center;
	gap: 8px; flex-wrap: wrap;
}
.audio_input {
	font-size: 14px;
	width: 80px;
	text-align: center;
	background-color: var(--bg-elev-2);
	color: var(--text);
	border: 1px solid var(--border);
	border-radius: 6px;
	padding: 10px;
	height: 44px;
}
#audio_volume_label {
	display: inline-block; min-width: 30px;
	font-size: 12px; color: var(--text-dim);
	font-family: monospace;
}

/* ===== Auto-button (Top bar) ===== */
#button_automatic { background-color: var(--bg-elev-2); }
#button_automatic.on { background-color: var(--accent); color: #000; border-color: var(--accent); }

/* ===== Footer ===== */
footer { font-size: 10px; color: var(--text-dim); margin-top: 24px; }

/* ===== Responsive Tweaks ===== */
/* Tablet and up: zwei-spaltige Eye-Vorschau gleichzeitig im View */
@media (min-width: 480px) {
	body { font-size: 15px; }
	.eye { width: 100px; height: 100px; }
	.eye > h5 { font-size: 13px; }
	.menu_eye_buttons > div, .eye_buttons > div { width: 60px; height: 60px; font-size: 11px; }
}

/* Desktop: leichter Anstieg, aber Container bleibt max 720 */
@media (min-width: 768px) {
	body { padding: 40px 20px 24px 20px; }
	.section { padding: 18px; }
	.joystick { width: 280px; height: 280px; }
}

/* Wide screens: ein bisschen mehr Luft */
@media (min-width: 1024px) {
	.app { max-width: 800px; }
	.joystick { width: 320px; height: 320px; }
}

/* Touch devices: disable hover so it doesn't stick after a tap */
@media (hover: none) {
	.button:hover { background-color: var(--bg-elev); border-color: var(--border); }
}

/* Loading-State Klassen */
.hidden { display: none; }
.clear { clear: both; }
)HUY";

#endif // WebAssets_h
