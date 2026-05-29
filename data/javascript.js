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
}