# Huyang Remote Control v2.9
**ESP8266/ESP32/ESP32-S3 based animatronics controller for Star Wars Huyang droid builds (DroidDivision)**

## Project Overview

This animatronics controller brings your Huyang droid to life with animated TFT eyes, servo-driven neck and body movement, a web interface, serial CLI, and voice assistant integration.
Designed for the DroidDivision Huyang build with the Printed-Droid PCB.

### Key Features

- **Multi-Board Support** - ESP8266, ESP32, ESP32-S3 with automatic pin selection
- **Animated TFT Eyes** - Two GC9A01 round displays with 6 eye expressions (open, closed, blink, focus, sad, angry)
- **9-Servo Motion Control** - Head tilt/rotation + body tilt/rotation via PCA9685 PWM driver
- **WiFi Web Interface** - Complete control via responsive web UI from any browser
- **Voice Assistant Control** - Google Home, Alexa & Siri integration with 11 trigger commands
- **Serial CLI** - Complete command-line interface for configuration and control
- **EEPROM Configuration** - Persistent settings with CRC-32 checksum validation and wear leveling
- **Automatic Animations** - Random eye expressions and head movement
- **Chest Lights** - NeoPixel LED strip for torso illumination
- **Audio System** - DFPlayer Mini for sound playback (prepared)
- **WiFi Fallback** - STA mode with automatic AP hotspot fallback
- **mDNS Support** - Access via `http://huyang.local`

---

## Changelog

[Full Changelog](changelog.md)

### Version 2.4 (2026-05)

**Web-UI Polish & Multi-Client**

- Joystick-Bug fixed (JoyNeck rief sendBodyUpdate auf - seit v1.9)
- Throttle auf Joystick/Slider (80 ms, statt >20 POSTs/s)
- Neue Audio-UI (Track-Input, Play, Stop, Volume-Slider)
- Neue Sequence-UI (greeting/surprised/sad/angry Buttons)
- Status-Bar fixed oben: WiFi-Mode, IP, MQTT, Sequence-Wiedergabe
- Error-Handler mit visueller Rueckmeldung (statt stillem Hang bei Netzproblemen)
- Multi-Client-Servo-State unter Joysticks (Text-Anzeige)
- Settings-Link sichtbar gemacht, fetch mode korrigiert

### Version 2.3 (2026-05)

**Reliability, Features & APIs**

- DFPlayer-Init aktiviert (war auskommentiert)
- WiFi: Auto-Retry vom AP-Mode zurueck nach STA alle 60s
- WiFi: Boot-STA-Timeout 10s -> 5s
- NeoPixel-Status-LEDs: Boot/Connect/STA/AP/Error/OTA
- OTA-Updates aktiv (Hostname "huyang")
- Optionales HTTP-Basic-Auth fuer Web-Interface
- Audio-Web-API (/audio/play, /audio/stop, /audio/volume)
- HuyangSequence-Klasse mit 4 vordefinierten Animationen
- MQTT-Integration (optional, via #define HUYANG_MQTT_ENABLED)

### Version 2.2 (2026-05)

**Printed-Droid PCB Pinout**

- ESP32-Block auf Lolin/WeMos ESP32 D1 Mini auf Printed-Droid-PCB umgestellt
- I2C/SPI nutzen ESP32-Default-Pins
- DFPlayer-Richtung korrigiert (TX statt RX)
- TFT-RST auf -1 (PCB fuehrt Display-RST direkt an ESP-RST)
- NeoPixel auf GPIO 17 (kein Boot-Strap, kein Konflikt)
- ESP8266-Block analog gefixt (war Original-Bug)

### Version 2.1 (2025-03)

**Multi-Board Support & Bug Fixes**

- Multi-board support: automatic pin selection via `pins.h`
- ESP32/S3: Arduino_ESP32SPI, HardwareSerial for DFPlayer, Wire.begin(SDA, SCL)
- HuyangAudio: public playTrack(), setVolume(), stop() methods
- HuyangFace: millis overflow fix, yield() in animations, balanced random moods
- HuyangConfig: CRC-32 checksum, packed struct, static_assert validation
- HuyangCLI: buffer limit, reset confirmation, audio stop command
- HuyangWifi: mDNS leak fix, softAPConfig order, auto-reconnect mDNS
- WebServer: chunked POST body, JSON error checking, LittleFS guard
- F() macros across all files for SRAM savings
- Many additional bugfixes (see changelog.md)

### Version 2.0 (2025-03)

**Major Refactoring & New Features**

- Replaced JxWifiManager with native WiFi management (STA with AP fallback)
- New EEPROM configuration system with checksum validation and wear leveling
- New Serial CLI for runtime configuration and control
- 11 voice assistant trigger endpoints
- mDNS support (`http://huyang.local`)
- Feature flags stored in EEPROM, configurable via CLI
- Calibration values stored in EEPROM instead of calibration.h
- Consolidated source files (one .cpp per class)
- Flattened file structure (removed src/ subdirectories)
- Multiple bugfixes and compatibility improvements
- Added board settings and pin documentation

### Version 1.9

- Changed pins to match Printed-Droid PCB
- Audio system prepared (DFPlayer Mini)
- UI improvements with titled buttons
- Config-driven interface (show only enabled features)

---

## Hardware Requirements

### Core Components
- **ESP8266** (NodeMCU, Wemos D1 Mini, Generic ESP8266 Module)
  or **ESP32-S3** (Waveshare ESP32-S3 Zero, Lolin S3 Mini - recommended)
  or **ESP32** (ESP32 Dev Module, Waveshare ESP32-Zero)
- **PCA9685 PWM Servo Driver** - 16-channel I2C servo controller (address 0x40)
- **2x GC9A01 Round TFT Displays** - 240x240 pixel, SPI interface (for eyes)
- **Up to 9x Standard Servos** (depending on build configuration)
- **5V Power Supply** - Adequate for servos (minimum 3A recommended)

### Audio System (Optional)
- **DFPlayer Mini** - MP3 player module
- **MicroSD Card** - FAT32 format
- **Speaker** - 4-8 ohm

### Optional Components
- **NeoPixel LED Strip** - WS2812B for chest/torso lights
- **Monacle Servo** - For monacle lens movement
- **IR Receiver** (TSOP38238) - ESP32-S3 only
- **PIR Motion Sensor** (HC-SR501) - ESP32-S3 only

### Recommended Carrier Board
- **Huyang Board** from [Printed-Droid.com](https://shop.printed-droid.com/produkt/huyang-boards/)

## Pin Configuration

Pin assignments are automatically selected based on the board chosen in Arduino IDE.
All pin definitions are in `pins.h`. No manual pin changes needed.

### GPIO Pin Mapping

| Function | ESP8266 | ESP32-S3 (S3 Zero / S3 Mini) | ESP32 (ESP32-Zero / Dev Module) |
|---|---|---|---|
| TFT DC | GPIO 2 | GPIO 4 | GPIO 4 |
| TFT CS Left Eye | GPIO 16 | GPIO 5 | GPIO 5 |
| TFT CS Right Eye | GPIO 15 | GPIO 3 | GPIO 3 |
| TFT RST | GPIO 0 | GPIO 6 | GPIO 6 |
| SPI SCK | GPIO 14 (default) | GPIO 10 | GPIO 18 |
| SPI MOSI | GPIO 13 (default) | GPIO 11 | GPIO 23 |
| I2C SDA | GPIO 4 (default) | GPIO 8 | GPIO 21 |
| I2C SCL | GPIO 5 (default) | GPIO 9 | GPIO 22 |
| Audio RX | GPIO 12 | GPIO 1 | GPIO 16 |
| Audio TX | -1 (unused) | GPIO 7 | GPIO 17 |
| NeoPixel | GPIO 0 | GPIO 2 | GPIO 2 |
| IR Receiver | - | GPIO 12 | - |
| PIR Sensor | - | GPIO 13 | - |

### PCA9685 PWM Servo Driver (I2C address 0x40)

| Channel | Function |
|---------|----------|
| 4 | Monacle servo |
| 5 | Head left servo |
| 6 | Head right servo |
| 8 | Head rotation servo |
| 9 | Neck tilt servo |
| 11 | Body rotation servo |
| 12 | Body forward left servo |
| 13 | Body forward right servo |
| 14 | Body sideways left servo |
| 15 | Body sideways right servo |

## Power Requirements

- **Main Supply**: 5V / 3A minimum
- **Servos**: 5-6V via PCA9685 V+ (external supply, current depends on servo count)
- **TFT Displays**: 3.3V (from ESP regulator)
- **NeoPixel LEDs**: 5V with current limiting
- **ESP**: 3.3V internal regulation
- **ESP32-S3** (S3 Zero / S3 Mini): 330uF + 100nF capacitors between 3V3 and GND recommended

**Warning**: Ensure adequate current capacity for your servo configuration.

---

## Installation

### Step 1: Install Board Support

1. Open Arduino IDE and go to **File** -> **Preferences**
2. In **Additional boards manager URLs**, add:
   - ESP8266: `http://arduino.esp8266.com/stable/package_esp8266com_index.json`
   - ESP32: `https://espressif.github.io/arduino-esp32/package_esp32_index.json`
3. Go to **Tools** -> **Board** -> **Board Manager**
4. Search for `esp` and install the appropriate package
5. Select your board under **Tools** -> **Board**

#### Board Settings - **ESP8266 D1 Mini** (EMPFOHLEN für Printed-Droid PCB)

Diese Settings sind **wichtig** — besonders MMU + lwIP für das volle Feature-Set:

| Setting | Value | Warum |
|---|---|---|
| Board | `LOLIN(WEMOS) D1 mini (clone)` oder `Generic ESP8266 Module` | |
| Upload Speed | `921600` | schnell |
| Debug port | `Disabled` | spart RAM |
| Debug Level | `None` | spart Flash |
| Flash Size | **`4MB (FS:none OTA:~1019KB)`** | **kein LittleFS mehr nötig!** |
| C++ Exceptions | `Disabled (new aborts on oom)` | Standard |
| Flash Frequency | `40MHz` | stabil |
| Flash Mode | `DOUT (compatible)` | kompatibel mit allen D1-Mini-Clones |
| **lwIP Variant** | **`v2 Lower Memory`** | **spart IRAM** |
| **MMU** | **`16KB cache + 48KB IRAM`** | **erweitert IRAM von 32 → 48 KB - PFLICHT für Feature-Set** |
| Non-32-Bit Access | `Use pgm_read macros for IRAM/PROGMEM` | korrekt für PROGMEM-UI |
| **SSL Support** | **`Basic SSL ciphers (lower ROM use)`** | **spart ~50 KB Flash** |
| Stack Protection | `Disabled` | spart Flash |
| VTables | `Flash` | spart RAM |
| Erase Flash | `Only Sketch` | EEPROM bleibt erhalten |
| **CPU Frequency** | **`160 MHz`** | **doppelte Performance, flüssigere Animationen** |

#### Board Settings - **ESP32 D1 Mini** (Lolin/WeMos, passt in selben PCB-Sockel)

| Setting | Value |
|---|---|
| Board | `WEMOS D1 MINI ESP32` oder `ESP32 Dev Module` |
| Upload Speed | `921600` |
| CPU Frequency | `240MHz (WiFi/BT)` |
| Flash Frequency | `80MHz` |
| Flash Mode | `QIO` |
| Flash Size | `4MB (32Mb)` |
| Partition Scheme | `Default 4MB with spiffs` (SPIFFS wird nicht genutzt) |
| PSRAM | `Disabled` |
| Erase All Flash | `Disabled` |

#### Board Settings - ESP32-S3 (separate Build, andere Hardware)

Nur falls du den S3-Zero-Standalone-Sketch nutzt — siehe `Huyang_Remote_Control_S3Zero_V1/`.

### Step 2: Install Libraries

Open **Tools** -> **Manage Libraries** and install each of the following:

| Search for | Install | Notes |
|---|---|---|
| `ESPAsyncWebServer` | ESPAsyncWebServer by lacamera | bzw. ESP32Async-Fork |
| `ESPAsyncTCP` (ESP8266) / `AsyncTCP` (ESP32) | Standard | wird automatisch als Dependency vorgeschlagen |
| `PWM Servo Driver` | Adafruit PWM Servo Driver Library | |
| `Adafruit NeoPixel` | Adafruit NeoPixel | |
| `Arduino GFX Library` | GFX Library for Arduino by Moon On Our Nation | |
| `DFRobotDFPlayerMini` | DFRobotDFPlayerMini | |
| `ArduinoJson` | ArduinoJson by Benoit Blanchon | **v7.x** erforderlich |
| `PubSubClient` | PubSubClient by Nick O'Leary | **nur** wenn `HUYANG_MQTT_ENABLED=true` in config.h |

### Step 3: Configure

Edit `config.h` to set your default values (used on first boot or after factory reset):

1. WiFi SSID + Passwort (oder leer lassen für AP-Mode-Fallback "HuyangWifiControl")
2. Choose WiFi mode: `1` = STA (connect to network), `0` = AP (hotspot)
3. Set hotspot name/password if using AP mode
4. Enable or disable hardware features to match your build

After first upload, all settings can be changed via Serial CLI or Web-Settings and are stored in EEPROM.

### Step 4: Upload — **EIN Schritt**

Ab v2.9 wird **nur noch** der Sketch hochgeladen. Web-UI-Assets sind in PROGMEM eingebettet, Custom-Sequence wird in EEPROM gespeichert. Kein LittleFS-Plugin nötig.

1. Board in Arduino IDE auswählen (Pins werden automatisch über `pins.h` gesetzt)
2. Settings oben anwenden (besonders MMU + lwIP + SSL!)
3. **Upload** (`Ctrl+U`)
4. Serial Monitor öffnen (115200 baud)
5. Browser → `http://huyang.local`

Fertig. Kein zweiter Upload, kein Plugin, keine Verwirrung.

**Wenn du `data/`-Files veränderst**, dann vor dem Compile:
```powershell
powershell -ExecutionPolicy Bypass -File gen-webassets.ps1
```
Das regeneriert `WebAssets.h` aus dem `data/`-Verzeichnis. Dann Sketch neu compilen + uploaden.

### Project File Structure

```
Huyang_Remote_Control_v2/
├── Huyang_Remote_Control_v2.ino  # Main program
├── pins.h                        # Board-specific pin definitions (auto-select)
├── config.h                      # Default configuration (compile-time)
├── HuyangConfig.h/.cpp           # EEPROM configuration system (CRC-32)
├── HuyangWifi.h/.cpp             # WiFi management (STA + AP)
├── HuyangCLI.h/.cpp              # Serial command interface
├── HuyangFace.h/.cpp             # TFT eye animations (6 moods)
├── HuyangNeck.h/.cpp             # Head/neck servo control (easing)
├── HuyangBody.h/.cpp             # Body servo control
├── HuyangAudio.h/.cpp            # DFPlayer Mini audio
├── EasingServo.h/.cpp            # Servo easing functions
├── WebServer.h/.cpp              # Web interface + trigger endpoints
├── HuyangSequence.h/.cpp         # Animation-Sequencer (Eyes+Body+Neck+Audio synchron)
├── HuyangMqtt.h/.cpp             # MQTT-Integration (optional via #define)
├── WebAssets.h                   # PROGMEM-embedded UI (auto-generiert)
├── gen-webassets.ps1             # Generator-Skript fuer WebAssets.h
├── data/                         # Source-Files fuer UI (Input fuer gen-webassets.ps1)
├── changelog.md                  # Version history
└── README.md                     # This file
```

**Hinweis:** `data/` wird nicht mehr auf den ESP hochgeladen. Der Inhalt ist via `gen-webassets.ps1` in `WebAssets.h` als PROGMEM-Konstanten eingebettet und wird mit dem Sketch geflasht. Falls du HTML/CSS/JS aenderst → Generator-Skript ausfuehren, dann Sketch neu uploaden.

---

## Control Interfaces

### 1. Web Interface

**Responsive web UI accessible from any browser**

#### Access
- **Direct IP**: `http://[ESP_IP_ADDRESS]` (shown in Serial Monitor)
- **mDNS**: `http://huyang.local`
- **AP Mode**: `http://192.168.10.1`

#### Features
- Eye expression control (open, closed, blink, focus, sad, angry)
- Joystick for head movement (rotation + tilt)
- Slider for sideways neck tilt
- Body movement controls
- Automatic animation toggle
- Configurable UI (only shows enabled features)

![Preview of 1.9](img/1_9.png)

### 2. Serial CLI (115200 baud)

Connect via Serial Monitor. Type `help` for all commands.

#### System Commands

```
help                    Show all available commands
status                  Show WiFi, memory, and uptime
config                  Show full configuration
save                    Save current config to EEPROM
reset confirm           Reset config to defaults (from config.h)
reboot                  Restart ESP
```

#### WiFi Commands

```
wifi status             Show WiFi connection info
wifi ssid <name>        Set WiFi network name
wifi password <pass>    Set WiFi password
wifi mode <ap|sta>      Set WiFi mode
wifi reconnect          Reconnect with current settings
```

After changing WiFi settings, use `save` then `wifi reconnect`.

#### Movement Commands

```
neck rotate <-100..100>     Rotate head left/right
neck tilt <-100..100>       Tilt neck forward/backward
neck sideways <-100..100>   Tilt neck left/right
body rotate <-100..100>     Rotate body left/right
body tilt <-100..100>       Tilt body forward/backward
body sideways <-100..100>   Tilt body left/right
```

#### Eye Commands

```
eyes open               Open both eyes
eyes close              Close both eyes
eyes blink              Blink both eyes
eyes focus              Focus expression
eyes sad                Sad expression
eyes angry              Angry expression
eyes auto               Enable automatic animations
```

#### Audio Commands

```
audio volume <0..30>    Set playback volume
audio play <track>      Play track by number
audio stop              Stop playback
```

#### Calibration Commands

```
cal show                    Show calibration offsets
cal neck rotation <val>     Set neck rotation offset
cal neck tiltforward <val>  Set neck tilt forward offset
cal neck tiltsideways <val> Set neck sideways offset
```

Use `save` to persist calibration changes.

#### Animation Control

```
auto on                 Enable automatic animations
auto off                Disable automatic animations
```

### 3. Voice Assistant Control (Google Home, Alexa, Siri)

HTTP GET endpoints for integration with Google Home, Alexa (via IFTTT), Siri (via Shortcuts), or any HTTP client.

#### Available Triggers

| Endpoint | Action |
|---|---|
| `/trigger/wakeup` | Open eyes, enable automatic mode |
| `/trigger/sleep` | Close eyes, center head, disable automatic |
| `/trigger/nod` | Tilt head forward (nod) |
| `/trigger/shake` | Rotate head left (head shake) |
| `/trigger/look` | Focus eyes |
| `/trigger/sad` | Sad eyes + head tilt down |
| `/trigger/angry` | Angry eyes |
| `/trigger/blink` | Blink eyes |
| `/trigger/random` | Enable automatic random animations |
| `/trigger/reset` | Reset all to default positions |
| `/trigger/sound?id=<n>` | Play audio track number n |

All endpoints return JSON: `{"status":"ok","action":"<name>"}`

#### IFTTT Setup (Google Home / Alexa)

1. Create account at https://ifttt.com
2. Create Applet: **If This** -> Google Assistant / Alexa -> "Say a simple phrase"
3. Trigger phrase: `Huyang wake up`
4. **Then That** -> Webhooks -> Make a web request
5. URL: `http://YOUR_HUYANG_IP/trigger/wakeup`, Method: GET
6. Save and test: "Hey Google, Huyang wake up"

#### Siri Shortcuts (iPhone/iPad)

1. Open **Shortcuts** app
2. Tap **+** -> **Add Action** -> search "Get Contents of URL"
3. Enter URL: `http://YOUR_HUYANG_IP/trigger/wakeup`
4. Name shortcut: "Huyang wake up"
5. Tap (i) -> **Add to Siri** -> record phrase
6. Test: "Hey Siri, Huyang wake up"

#### Quick Setup URLs

Replace `192.168.1.100` with your Huyang's IP address:

```
http://192.168.1.100/trigger/wakeup
http://192.168.1.100/trigger/sleep
http://192.168.1.100/trigger/nod
http://192.168.1.100/trigger/shake
http://192.168.1.100/trigger/look
http://192.168.1.100/trigger/sad
http://192.168.1.100/trigger/angry
http://192.168.1.100/trigger/blink
http://192.168.1.100/trigger/random
http://192.168.1.100/trigger/reset
http://192.168.1.100/trigger/sound?id=1
```

#### Testing

You can test triggers directly in your browser or with curl:
```
curl http://huyang.local/trigger/wakeup
curl http://192.168.1.100/trigger/sad
curl http://192.168.10.1/trigger/sound?id=3
```

**Notes:**
- Triggers work on local network without additional hardware
- For external access, use port forwarding or VPN
- IFTTT free plan supports unlimited applets
- Siri Shortcuts work locally without cloud latency

---

## Troubleshooting

| Problem | Solution |
|---------|----------|
| Web interface shows blank page | `WebAssets.h` muss existieren — `gen-webassets.ps1` ausfuehren und Sketch neu uploaden |
| IRAM overflow (`section .iram1 will not fit`) | MMU auf `16KB cache + 48KB IRAM` umstellen, lwIP auf `v2 Lower Memory`, SSL auf `Basic` |
| Custom-Sequence verschwindet nach Reboot | EEPROM-Migration. Bei v2.9-Erst-Flash wird alte EEPROM-Config verworfen. Sequenz neu speichern. |
| Cannot connect to WiFi | Check SSID/password via CLI: `wifi ssid YourNetwork`, `wifi password YourPass`, `save`, `wifi reconnect` |
| Servos not moving | Verify PCA9685 wiring and I2C address (0x40). Check feature flags with `config` |
| Serial Monitor shows gibberish | Set baud rate to 115200 |
| EEPROM config lost | Use `reset confirm` to restore defaults, then `save` |
| mDNS not working | Not all networks support mDNS. Use IP address instead |
| WiFi keeps disconnecting | Huyang auto-reconnects every 30s in STA mode. Check signal strength |
| Eyes not displaying | Check SPI wiring. See pin table above for your board |
| ESP32-S3 displays not working | Verify Display SCL/SDA on GPIO 10/11 (SPI), not GPIO 8/9 (I2C)! |
| Wrong pins after board switch | Pins are auto-selected via `pins.h`. Verify correct board is selected in Arduino IDE |

---

## Credits

Huyang Droid Remote Control v2.9 by [Printed-Droid.com](https://printed-droid.com)
Original v1.x by Jeanette Mueller
