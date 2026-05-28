# Changelog

## 2.9 Single-Upload (No-LittleFS-Mode)
* UI-Assets (alle 13 HTML/CSS/JS-Files) in PROGMEM eingebettet (WebAssets.h)
* WebServer servt alles aus PROGMEM via send_P() - kein LittleFS-Zugriff mehr
* gen-webassets.ps1 Generator-Skript: liest data/*.* und erzeugt WebAssets.h
* Custom-Sequence-Storage von LittleFS auf EEPROM umgestellt
  - HuyangConfig: neues customSeqBlob[224] + customSeqCount Feld
  - Config-Version 3, EEPROM-Size auf 1024 erweitert
  - SequenceStep packed (7 Bytes, static_assert garantiert)
* LittleFS-Dependency komplett entfernt (Include, .begin(), readFile/writeFile)
* **End-User-Workflow:** Sketch flashen - fertig. Kein LittleFS-Plugin mehr noetig!
* Flash-Verbrauch steigt um ~67 KB (UI-Assets) - immer noch ~50 % frei

## 2.8 Big-Feature-Bundle
### Face / Eyes
* Eye-Color-Presets (Huyang, Sith, Jedi, K-2SO, White, Toxic)
* Pupillen-Groesse als Slider (6-80 px) via /eye/pupil?size=N
* Closed-Eye-Color konfigurierbar (/eye/closedcolor?hex=...)
* Pupille bewegt sich im Idle-Mode leicht im Auge (autom. zufaellig)

### Audio
* Volume-Mute-Button (merkt vorherige Lautstaerke)

### Servos
* Speed-Presets (slow/normal/fast) via /servo/speed?preset=X
* Skaliert Easing-Dauer 0.5x / 1x / 2x

### Sequences
* Self-Test-Sequenz (/test/run oder /sequence?name=selftest)
* Sequence-Progress-Bar in der Status-Bar
* Custom-Sequence-Editor: JSON-Editor im Web, Save in LittleFS, abspielen via /sequence?name=custom
* HuyangSequence::getProgress() und getName() fuer Status-Display

### UI
* Fullscreen/Kiosk-Modus Toggle-Button
* Heap-Monitor in Status-Bar (warnt bei <8 KB)
* Settings-Page komplett funktional (Feature-Flags, Config Backup/Restore, Reboot, Factory Reset)
* Calibration-Page funktional (direkte PCA9685-Channel-Slider 150-595)
* API-Dokumentations-Seite unter /api

### Connectivity
* WiFi-Scan + Selector im Web-Interface (/wifi/scan, /wifi/connect)
* MQTT Home-Assistant-Auto-Discovery (Buttons fuer Sequenzen, Volume-Number, Eye-State-Select)

### Security
* HuyangBody::setRawServo() fuer Calibration direkt am PCA9685
* HuyangConfig get/set/reset Endpoints in WebServer
* Simpler CSRF-Schutz auf state-aendernden GET-Endpoints bei aktivem Web-Auth

## 2.7 Theme-Trennung + Light-Mode
* Eye-Color faerbt jetzt NUR die Eye-Vorschau-Kreise, nicht die UI-Akzentfarbe
  - Neue CSS-Variable --eye-color (separate von --accent)
  - Buttons + Borders bleiben gelb auch bei abweichender Augenfarbe
* Light-Theme als Option ueber Toggle-Button (oben rechts neben Settings)
  - data-theme="light" CSS-Overrides via :root
  - Persistenz in localStorage["huyang.theme"]
  - Rein client-side, kein ESP-Roundtrip noetig
  - Akzentfarben + Eye-Color bleiben in beiden Themes gleich (User-Wahl)
  - Icon zeigt Ziel-Theme: ☾ im Light-Mode, ☀ im Dark-Mode

## 2.6 Pupillen-Feature + Credits
* Pupille als optionales Feature: an/aus per Checkbox, Farbe per Color-Picker (Default Schwarz)
* HuyangFace::setPupilEnabled/setPupilColorRGB/setPupilColorHex APIs
* Wird in openEyes / openEye nach Animation gezeichnet (fillCircle in der Mitte)
* Sofortige Aktualisierung bei Aenderung wenn Auge offen ist
* Neuer Endpoint /eye/pupil?enabled=1&hex=000000 (beide Parameter optional)
* Pupil-UI in Face-Sektion (Checkbox + Color-Picker neben Eye-Color)
* Demo-Visualisierung: Pupille als Punkt auf den Eye-Vorschau-Kreisen
* Footer-Text: "Made with heart by Jeanette Mueller - Enhanced by Printed-Droid"

## 2.5 Responsive Redesign & Eye-Color
* CSS komplett neu (mobile-first responsive, max-width 720px Container)
  - Kompakte Schriften (14px Base statt 30pt), gefuehlt 50% kompakter auf Desktop
  - Card-basiertes Layout mit ".section"-Klassen
  - Kleinere Eye-Vorschau-Kreise (90-100px statt 200px)
  - Joystick 240-320px je nach Viewport (vorher fix 400px)
  - Modernes Dark Theme mit CSS Custom Properties
  - Touch-Targets >= 44px (WCAG)
  - Media Queries fuer 480/768/1024px Breakpoints
* User-konfigurierbare Augenfarbe ueber Color-Picker im Web-UI
  - HuyangFace::setEyeColorRGB() und setEyeColorHex() (RGB888 -> RGB565 mit Display-Invertierung)
  - Neuer Endpoint /eye/color?hex=ffcc00
  - Live-Preview im Web (kleines farbiges Vorschau-Kreischen)
  - Soforige Display-Aktualisierung wenn Augen offen sind

## 2.4 Web-UI Polish & Multi-Client
* Joystick-Bug behoben: JoyNeck-Callback rief faelschlicherweise sendBodyUpdate() statt sendNeckUpdate() auf (bestand seit v1.9)
* fetch mode 'no-cors' -> 'same-origin' (semantisch korrekt)
* Settings-Link in index.html sichtbar (war auskommentiert -> /settings.html unerreichbar)
* Throttle (80 ms) auf sendNeckUpdate / sendBodyUpdate - verhindert >20 POSTs/s beim Joystick-Wackeln
* Neue Audio-UI: index.audio.html mit Track-Input, Play, Stop, Volume-Slider
* Neue Sequence-UI: index.sequence.html mit 4 Buttons (greeting/surprised/sad/angry) + Stop
* Status-Bar fixed oben: zeigt WiFi-Mode, IP, MQTT-Status, Sequence-Wiedergabe - alle 5s aktualisiert
* Neuer Endpoint GET /status liefert System-State-JSON
* Error-Handling: rote Status-Bar bei Offline, gelbe bei einzelnem Request-Fehler (statt stillem Hang)
* Multi-Client-Sync: Server-bekannte Servo-States werden als Text unter den Joysticks angezeigt
* CSS: Padding fuer Status-Bar, Audio-Controls-Styling, monospace Remote-State

## 2.3 Reliability, Features & APIs
* DFPlayer wird jetzt im Setup initialisiert (huyangAudio->setup() war auskommentiert)
* WiFi: Auto-Retry vom AP-Mode zurueck nach STA alle 60s (vorher: einmal AP, immer AP)
* WiFi: Boot-STA-Timeout reduziert von 10s auf 5s (Boot fuehlt sich nicht mehr tot an)
* NeoPixel-Status-LEDs: rot=Boot, gelb=WiFi-Connect, gruen=STA ok, magenta=AP, rot-blink=Error/OTA
* OTA-Updates aktiviert (ArduinoOTA, Hostname "huyang") - flashen ueber WiFi ohne USB
* Web-Interface mit optionalem HTTP-Basic-Auth (in config.h aktivierbar)
* Audio-Web-API: /audio/play?track=N, /audio/stop, /audio/volume?v=N
* HuyangSequence-Klasse: zeitgesteuerte Animationen (Eyes+Body+Neck+Audio synchron)
  Vordefiniert: greeting, surprised, sad, angry - via /sequence?name=X triggerbar
* MQTT-Integration (optional, via #define HUYANG_MQTT_ENABLED in config.h)
  Subscribe: huyang/cmd/{eye,sequence,audio/...,neck/...,body/...}
  Publish: huyang/state/status
* Servo-Frequenz dokumentiert (bewusst 60 Hz, SERVOMIN/MAX darauf kalibriert)

## 2.2 Printed-Droid PCB Pinout
* ESP32-Block in pins.h umgestellt auf Lolin/WeMos ESP32 D1 Mini auf Printed-Droid-PCB
  (PCB-Sockel D0..D8 = ESP32 GPIO 26/22/21/17/16/18/19/23/5)
* I2C und SPI nutzen jetzt ESP32-Default-Pins (SDA=21, SCL=22, SCK=18, MOSI=23)
* DFPlayer-Richtung korrigiert: TX=GPIO 19 (D6), RX=-1 (PCB hat keine Rueckleitung)
* TFT-RST auf -1 gesetzt (PCB fuehrt Display-RST direkt an ESP-RST)
* NeoPixel auf GPIO 17 (D3) - kein Boot-Strap mehr, kein Konflikt
* ESP8266-Block analog gefixt (war Original-Bug: RST=0 kollidierte mit NeoPixel=0,
  DFPlayer-Richtung verkehrt) - betrifft alle, die v2 noch auf ESP8266 D1 Mini bauen

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
