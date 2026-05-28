# Changelog

## 2.9 Single-Upload (No-LittleFS-Mode)
* All 13 HTML/CSS/JS UI assets embedded in PROGMEM (WebAssets.h)
* WebServer serves everything from PROGMEM via send_P() - no more LittleFS access
* gen-webassets.ps1 generator script: reads data/*.* and produces WebAssets.h
* Custom-sequence storage moved from LittleFS to EEPROM
  - HuyangConfig: new customSeqBlob[224] + customSeqCount fields
  - Config schema version 3, EEPROM size raised to 1024
  - SequenceStep packed to 7 bytes (static_assert guarantees layout)
* LittleFS dependency completely removed (includes, .begin(), readFile/writeFile)
* **End-user workflow:** flash the sketch - done. No LittleFS plugin needed anymore!
* Flash usage grows by ~67 KB (UI assets) - still ~50 % free

## 2.8 Big Feature Bundle
### Face / Eyes
* Eye-color presets (Huyang, Sith, Jedi, K-2SO, White, Toxic)
* Pupil size as a slider (6-80 px) via /eye/pupil?size=N
* Closed-eye color configurable (/eye/closedcolor?hex=...)
* Pupil drifts slightly in the eye while in idle mode (random)

### Audio
* Volume mute button (remembers previous volume)

### Servos
* Speed presets (slow/normal/fast) via /servo/speed?preset=X
* Scales easing duration 0.5x / 1x / 2x

### Sequences
* Self-test sequence (/test/run or /sequence?name=selftest)
* Sequence progress bar in the status bar
* Custom-sequence editor: JSON editor in the web UI, saved to LittleFS, played via /sequence?name=custom
* HuyangSequence::getProgress() and getName() for status display

### UI
* Fullscreen / kiosk-mode toggle button
* Heap monitor in the status bar (warns when <8 KB)
* Settings page fully functional (feature flags, config backup/restore, reboot, factory reset)
* Calibration page functional (direct PCA9685 channel sliders 150-595)
* API documentation page at /api

### Connectivity
* WiFi scan + selector in the web UI (/wifi/scan, /wifi/connect)
* MQTT Home Assistant auto-discovery (buttons for sequences, volume number, eye-state select)

### Security
* HuyangBody::setRawServo() for calibration directly on the PCA9685
* HuyangConfig get/set/reset endpoints in WebServer
* Simple CSRF protection on state-changing GET endpoints when web auth is enabled

## 2.7 Theme Split + Light Mode
* Eye color now affects ONLY the eye preview circles, not the UI accent color
  - New CSS variable --eye-color (separate from --accent)
  - Buttons and borders stay yellow regardless of chosen eye color
* Light theme as an option via toggle button (top right, next to Settings)
  - data-theme="light" CSS overrides via :root
  - Persisted in localStorage["huyang.theme"]
  - Pure client-side, no ESP round-trip needed
  - Accent + eye color stay user-controlled across both themes
  - Icon shows target theme: ☾ in light mode, ☀ in dark mode

## 2.6 Pupil Feature + Credits
* Pupil as an optional feature: on/off via checkbox, color via color picker (default black)
* HuyangFace::setPupilEnabled/setPupilColorRGB/setPupilColorHex APIs
* Drawn at the end of openEyes / openEye (fillCircle in the center)
* Immediate refresh when changed while the eye is open
* New endpoint /eye/pupil?enabled=1&hex=000000 (both params optional)
* Pupil UI in the face section (checkbox + color picker next to eye color)
* Demo visualization: pupil as a dot on the eye preview circles
* Footer text: "Made with heart by Jeanette Mueller - Enhanced by Printed-Droid"

## 2.5 Responsive Redesign & Eye Color
* CSS rewritten from scratch (mobile-first responsive, max-width 720px container)
  - Compact font sizes (14px base instead of 30pt), roughly 50% tighter on desktop
  - Card-based layout with ".section" classes
  - Smaller eye preview circles (90-100px instead of 200px)
  - Joystick 240-320px depending on viewport (was fixed at 400px)
  - Modern dark theme via CSS custom properties
  - Touch targets >= 44px (WCAG)
  - Media queries for 480/768/1024px breakpoints
* User-configurable eye color via color picker in the web UI
  - HuyangFace::setEyeColorRGB() and setEyeColorHex() (RGB888 -> RGB565 with display inversion)
  - New endpoint /eye/color?hex=ffcc00
  - Live preview in the web UI (small colored swatch)
  - Immediate display refresh when eyes are open

## 2.4 Web-UI Polish & Multi-Client
* Joystick bug fixed: JoyNeck callback was wrongly calling sendBodyUpdate() instead of sendNeckUpdate() (present since v1.9)
* fetch mode 'no-cors' -> 'same-origin' (semantically correct)
* Settings link in index.html made visible (was commented out -> /settings.html unreachable)
* Throttle (80 ms) on sendNeckUpdate / sendBodyUpdate - prevents >20 POSTs/s while wiggling the joystick
* New Audio UI: index.audio.html with track input, play, stop, volume slider
* New Sequence UI: index.sequence.html with 4 buttons (greeting/surprised/sad/angry) + stop
* Status bar fixed at the top: shows WiFi mode, IP, MQTT status, sequence playback - refreshed every 5s
* New endpoint GET /status returns system-state JSON
* Error handling: red status bar when offline, yellow on single failed request (instead of silent hang)
* Multi-client sync: server-known servo states shown as text below the joysticks
* CSS: padding for status bar, audio controls styling, monospace remote-state

## 2.3 Reliability, Features & APIs
* DFPlayer is now initialized in setup (huyangAudio->setup() was commented out)
* WiFi: auto-retry back to STA from AP mode every 60s (was: once AP, always AP)
* WiFi: boot STA timeout reduced from 10s to 5s (boot no longer feels dead)
* NeoPixel status LEDs: red=boot, yellow=WiFi connecting, green=STA ok, magenta=AP, red-blink=error/OTA
* OTA updates enabled (ArduinoOTA, hostname "huyang") - flashing over WiFi without USB
* Web interface with optional HTTP basic auth (enable in config.h)
* Audio Web API: /audio/play?track=N, /audio/stop, /audio/volume?v=N
* HuyangSequence class: time-scheduled animations (eyes + body + neck + audio in sync)
  Pre-defined: greeting, surprised, sad, angry - triggered via /sequence?name=X
* MQTT integration (optional, via #define HUYANG_MQTT_ENABLED in config.h)
  Subscribe: huyang/cmd/{eye,sequence,audio/...,neck/...,body/...}
  Publish: huyang/state/status
* Servo frequency documented (intentionally 60 Hz, SERVOMIN/MAX calibrated for it)

## 2.2 Printed-Droid PCB Pinout
* ESP32 block in pins.h switched to Lolin/WeMos ESP32 D1 Mini on Printed-Droid PCB
  (PCB sockets D0..D8 = ESP32 GPIO 26/22/21/17/16/18/19/23/5)
* I2C and SPI now use ESP32 default pins (SDA=21, SCL=22, SCK=18, MOSI=23)
* DFPlayer direction corrected: TX=GPIO 19 (D6), RX=-1 (PCB has no return line)
* TFT RST set to -1 (PCB ties display RST directly to ESP RST)
* NeoPixel moved to GPIO 17 (D3) - no boot-strap conflict anymore
* ESP8266 block fixed accordingly (was original v1.9 bug: RST=0 collided with NeoPixel=0,
  DFPlayer direction reversed) - affects anyone still building v2 on an ESP8266 D1 Mini

## 2.1 Multi-Board Support & Bug Fixes
* Multi-board support: ESP8266, ESP32, ESP32-S3 — automatic pin selection via pins.h
* Arduino IDE board selection automatically configures all GPIO pins (SPI, I2C, Audio, NeoPixel)
* ESP32/S3: Arduino_ESP32SPI with explicit pin assignment, Wire.begin(SDA, SCL)
* ESP32/S3: HardwareSerial for DFPlayer instead of SoftwareSerial
* HuyangAudio: Added public playTrack(), setVolume(), stop() methods
* HuyangAudio: Removed infinite loop on serial failure (graceful fallback)
* HuyangFace: Fixed millis overflow handling (proper unsigned subtraction)
* HuyangFace: Added yield() in blocking animation loops (prevents ESP8266 WDT reset)
* HuyangFace: Balanced random mood distribution (25% each instead of 50% Blink)
* HuyangBody: Servo constrain(0, 180) to prevent invalid PWM values
* HuyangBody: Reduced centerAll() delays from 500ms to 200ms each
* HuyangBody: Fixed millis overflow handling
* HuyangNeck: Fixed stale millis in rotateHead/tiltNeckForward (use millis() directly)
* EasingServo: Fixed stale millis in moveServoTo (use millis() directly)
* HuyangCLI: Input buffer limit (128 chars) prevents overflow
* HuyangCLI: reset requires "reset confirm" to prevent accidental resets
* HuyangCLI: eyes blink now correctly sets webserver state
* HuyangCLI: audio commands call actual playTrack/setVolume/stop methods
* HuyangCLI: Added "audio stop" command
* HuyangCLI: Null-termination on strncpy for WiFi credentials
* HuyangConfig: CRC-32 (IEEE 802.3) checksum replacing simple byte sum
* HuyangConfig: __attribute__((packed)) on config struct
* HuyangConfig: static_assert for struct size and field offset validation
* HuyangConfig: Null-terminate string fields after EEPROM load
* HuyangConfig: Preserve writeCount across resetToDefaults()
* HuyangWifi: Fixed softAPConfig() order (before softAP)
* HuyangWifi: MDNS.end() before MDNS.begin() to prevent memory leak
* HuyangWifi: mDNS restart after auto-reconnect
* HuyangWifi: AP password < 8 chars warning
* WebServer: Chunked POST body accumulation for large payloads
* WebServer: JSON parse error checking with 400 response
* WebServer: Nod/shake return servos to center position
* WebServer: Removed unnecessary delay before file.close()
* WebServer: LittleFS ready guard on file operations
* F() macros applied consistently across all files for SRAM savings
* Removed real WiFi credentials from config.h (empty defaults + CLI instructions)

## 2.0 Major Refactoring & New Features
* Replaced JxWifiManager with native WiFi management (STA with AP fallback)
* New EEPROM configuration system (HuyangConfig) with checksum validation and wear-leveling
* New Serial CLI (HuyangCLI) for runtime configuration and control
* Voice assistant trigger endpoints (/trigger/wakeup, /sleep, /nod, /shake, /look, /sad, /angry, /blink, /random, /reset, /sound)
* mDNS support (http://huyang.local)
* Feature flags stored in EEPROM, configurable via CLI
* Calibration values stored in EEPROM instead of calibration.h
* Consolidated split .cpp files (HuyangNeck, HuyangFace) into single files per class
* Flattened file structure (removed src/ subdirectories)
* Fixed include guard collision with ESP32 core WebServer.h
* Fixed ArduinoJson include order for ESP_Async_WebServer v3.10.0 compatibility
* Multiple bugfixes across the codebase
* Added board settings and pin documentation to .ino header
* Updated README with CLI commands, trigger endpoints, and configuration docs

## 1.9
* Changed Pins to match the Custom made PCB from https://shop.printed-droid.com/produkt/huyang-boards/
* New Audio System prepared (untested)
* Buttons with Titles
* Now using Config to show only needed Interface parts

## 1.8
* Interface update
* new Wiring Diagram to enable Servos
* Graphical buttons (beta)
* Usage of the full range of Sliders and Joysticks

## 1.7
* New Calibration File
* Styling
* Show current state of eyes

## 1.6 Javascript actions
* Send All Eye Actions as Javascript Post
* New Joystick to move the Head
* New Slider for Head Sidways Tilt

## 1.5 New Wifi Manager
* Switch wo custom Hotspot when no wifi accessable

## 1.4 New Filestructure
* All System Files under /src for better updateability

## 1.3 Whole Body preparations
* New empty Class: HuyangBody will control Torso rotation, roll, tilt and yaw
* New empty Class: HuyangNeck will control Head/Neck rotation, roll, tilt and yaw

## 1.2 Refactoring
* Bugfixing
* All Face Moods controlled with one HuyangFace Class

## 1.1 New Gestures
* Faster Blink
* Sad Mood
* Angry Mood
* Focus Mood

## 1.0 Initial Version
* With Webserver
* Control Eyes Open and Close
* Eyes can Blink
