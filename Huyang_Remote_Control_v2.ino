/*
================================================================================
PROJECT:      Huyang Droid Remote Control
VERSION:      2.9 (Single-Upload, no LittleFS)
AUTHOR:       Printed-Droid.com
ORIGINAL:     Jeanette Mueller (v1.x)
DESCRIPTION:  ESP-based control system for the Huyang droid (DroidDivision)
              with animated TFT eyes, servo-driven neck/body, web interface,
              serial CLI, voice assistant trigger endpoints, OTA, MQTT.

HARDWARE:     ESP8266 D1 Mini or ESP32 D1 Mini on Printed-Droid PCB
              + PCA9685 PWM Driver + 2x GC9A01 Round TFTs + DFPlayer Mini
BAUD RATE:    115200

UI-Assets liegen in WebAssets.h (PROGMEM), Custom-Sequence in EEPROM.
KEIN LittleFS-Upload mehr noetig - nur Sketch flashen, fertig.

================================================================================
MULTI-BOARD SUPPORT:
================================================================================

Pin assignments are automatically selected based on the board chosen in
Arduino IDE. See pins.h for the full pin mapping.

Supported boards:
  - ESP8266 D1 Mini (default on Printed-Droid PCB)
  - ESP32 D1 Mini Lolin/WeMos (drop-in replacement on same PCB socket)
  - ESP32-S3 (separate build / experimental)
  - ESP32 generic Dev Module

================================================================================
ARDUINO IDE BOARD SETTINGS - ESP8266 D1 Mini (EMPFOHLEN fuer Printed-Droid PCB)
================================================================================

  Board:               "LOLIN(WEMOS) D1 mini (clone)"  oder  "Generic ESP8266"
  Upload Speed:        921600
  Debug port:          Disabled
  Debug Level:         None
  Flash Size:          "4MB (FS:none OTA:~1019KB)"   <- WICHTIG: FS:none!
  C++ Exceptions:      Disabled (new aborts on oom)
  Flash Frequency:     40MHz
  Flash Mode:          DOUT (compatible)
  lwIP Variant:        v2 Lower Memory                <- spart IRAM
  MMU:                 16KB cache + 48KB IRAM          <- WICHTIG fuer Feature-Set!
  Non-32-Bit Access:   Use pgm_read macros for IRAM/PROGMEM
  SSL Support:         Basic SSL ciphers (lower ROM use)
  Stack Protection:    Disabled
  VTables:             Flash
  Erase Flash:         Only Sketch
  CPU Frequency:       160 MHz

================================================================================
ARDUINO IDE BOARD SETTINGS - ESP32 D1 Mini (Lolin/WeMos auf Printed-Droid PCB)
================================================================================

  Board:               "WEMOS D1 MINI ESP32"  oder  "ESP32 Dev Module"
  Upload Speed:        921600
  CPU Frequency:       240MHz (WiFi/BT)
  Flash Frequency:     80MHz
  Flash Mode:          QIO
  Flash Size:          4MB (32Mb)
  Partition Scheme:    Default 4MB with spiffs       (SPIFFS wird nicht genutzt)
  PSRAM:               Disabled
  Erase All Flash:     Disabled

================================================================================
ARDUINO IDE BOARD SETTINGS - ESP32-S3 (separate Build, nicht diese PCB)
================================================================================

  Board:               "ESP32S3 Dev Module"
  Upload Speed:        921600
  USB CDC On Boot:     Enabled
  CPU Frequency:       240MHz
  Flash Size:          4MB (32Mb)
  Partition Scheme:    Default 4MB with spiffs

================================================================================
LIBRARIES (via Library Manager):
================================================================================

  - ESPAsyncWebServer  (by Iacamera - oder ESP32Async Fork)
  - ESPAsyncTCP        (ESP8266 only)  ODER  AsyncTCP (ESP32)
  - Adafruit PWM Servo Driver Library
  - Adafruit NeoPixel
  - GFX Library for Arduino  (Moon On Our Nation)
  - DFRobotDFPlayerMini
  - ArduinoJson  (v7.x)
  - PubSubClient  (NUR wenn HUYANG_MQTT_ENABLED=true in config.h)

================================================================================
CONFIGURATION:
================================================================================

1. Select your board in Arduino IDE (Settings siehe oben!)
2. Edit config.h for default WiFi credentials and feature flags
3. Upload sketch - das war's. KEIN LittleFS-Upload mehr noetig.
4. Open Serial Monitor at 115200 baud
5. Browser auf http://huyang.local oder die im Serial ausgegebene IP
6. Use Serial CLI to configure at runtime (type 'help')
7. Settings are stored in EEPROM and survive reboots

Falls data/-Files geaendert wurden:
   powershell -ExecutionPolicy Bypass -File gen-webassets.ps1
   Erzeugt WebAssets.h neu - dann Sketch neu kompilieren + uploaden.

================================================================================
*/

#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_PWMServoDriver.h>
#include <Arduino_GFX_Library.h>
#include <ArduinoOTA.h>

#include "pins.h"
#include "config.h"

#include "HuyangConfig.h"
#include "HuyangWifi.h"

#include "HuyangFace.h"
#include "HuyangBody.h"
#include "HuyangNeck.h"
#include "HuyangAudio.h"
#include "HuyangSequence.h"
#include "HuyangMqtt.h"

#include "WebServer.h"
#include "HuyangCLI.h"

// =============================================================================
// Hardware Init — board-specific SPI bus for TFT displays
// =============================================================================

#ifdef ESP32
// ESP32/ESP32-S3: Use Arduino_ESP32SPI with explicit pin assignment
Arduino_DataBus *leftBus = new Arduino_ESP32SPI(
    PIN_TFT_DC, PIN_TFT_CS_LEFT, PIN_SPI_SCK, PIN_SPI_MOSI);
Arduino_GFX *leftGfx = new Arduino_GC9A01(leftBus, PIN_TFT_RST);

Arduino_DataBus *rightBus = new Arduino_ESP32SPI(
    PIN_TFT_DC, PIN_TFT_CS_RIGHT, PIN_SPI_SCK, PIN_SPI_MOSI);
Arduino_GFX *rightGfx = new Arduino_GC9A01(rightBus, PIN_TFT_RST);
#else
// ESP8266: Use Arduino_HWSPI with default SPI pins (SCK=14, MOSI=13)
Arduino_DataBus *leftBus = new Arduino_HWSPI(PIN_TFT_DC, PIN_TFT_CS_LEFT);
Arduino_GFX *leftGfx = new Arduino_GC9A01(leftBus, PIN_TFT_RST);

Arduino_DataBus *rightBus = new Arduino_HWSPI(PIN_TFT_DC, PIN_TFT_CS_RIGHT);
Arduino_GFX *rightGfx = new Arduino_GC9A01(rightBus, PIN_TFT_RST);
#endif

Adafruit_PWMServoDriver *pwm = new Adafruit_PWMServoDriver(0x40);

// Configuration & WiFi
HuyangConfig huyangConfig;
HuyangWifi huyangWifi(&huyangConfig);

// Droid components
HuyangFace *huyangFace = new HuyangFace(leftGfx, rightGfx);
HuyangBody *huyangBody = new HuyangBody(pwm);
HuyangNeck *huyangNeck = new HuyangNeck(pwm);
HuyangAudio *huyangAudio = new HuyangAudio();
HuyangSequence *huyangSequence = new HuyangSequence(huyangFace, huyangNeck, huyangBody, huyangAudio);
HuyangMqtt *huyangMqtt = new HuyangMqtt(huyangFace, huyangNeck, huyangBody, huyangAudio, huyangSequence);

// Web & CLI
WebServer *webserver = new WebServer(WebServerPort);
HuyangCLI huyangCLI(&huyangConfig, &huyangWifi,
                     huyangFace, huyangNeck,
                     huyangBody, huyangAudio,
                     webserver);

// Timing
unsigned long currentMillis = 0;
unsigned long previousMillisIP = 0;

// Status-LED Tracking: aktueller WiFi-Mode, fuer Erkennung von Mode-Wechseln in loop()
String lastWifiMode = "";

void setup()
{
    Serial.begin(115200);
    Serial.println(F("Huyang! v2.9 by Printed-Droid.com (Single-Upload, no LittleFS)"));

#ifdef ESP32
    Serial.println(F("Board: ESP32"));
#else
    Serial.println(F("Board: ESP8266"));
#endif

    // 1. Load config from EEPROM (falls back to config.h defaults)
    huyangConfig.setup();

    // 2. I2C + PWM servo driver (vorgezogen damit Body+NeoPixel verfuegbar sind als Status-LEDs)
#ifdef ESP32
    Wire.begin(PIN_I2C_SDA, PIN_I2C_SCL);
#endif
    pwm->begin();
    pwm->setOscillatorFrequency(27000000);
    pwm->setPWMFreq(60); // Bewusst 60 Hz - SERVOMIN/SERVOMAX in HuyangBody.h/HuyangNeck.h sind darauf kalibriert

    // 3. Body zuerst initialisieren - dadurch werden NeoPixel als Status-LEDs nutzbar
    huyangBody->setup();
    huyangBody->setStatus(HuyangBody::StatusBoot); // rot = Boot in progress
    huyangBody->updateChestLights();

    // 4. WiFi - mit Status-Feedback
    huyangBody->setStatus(HuyangBody::StatusConnectingSTA); // gelb
    huyangBody->updateChestLights();
    huyangWifi.setup();
    if (huyangWifi.getMode() == "STA")
    {
        huyangBody->setStatus(HuyangBody::StatusConnectedSTA); // gruen (2s, dann Default)
    }
    else
    {
        huyangBody->setStatus(HuyangBody::StatusAPMode); // magenta - bleibt bis STA-Retry erfolgreich
    }
    huyangBody->updateChestLights();

    // 5. Restliche Droid-Komponenten
    huyangFace->setup();
    huyangNeck->setup();
    huyangAudio->setup();

    // 6. Web server with feature flags from EEPROM config
    webserver->setComponents(huyangFace, huyangNeck, huyangBody, huyangAudio);
    webserver->setSequence(huyangSequence);
    webserver->setStatusSources(&huyangWifi, huyangMqtt);
    webserver->setConfig(&huyangConfig);
    webserver->setAuth(HUYANG_WEB_AUTH_ENABLED, HUYANG_WEB_USER, HUYANG_WEB_PASS);
    webserver->setup(huyangConfig.data.enableEyes,
                     huyangConfig.data.enableMonacle,
                     huyangConfig.data.enableNeckMovement,
                     huyangConfig.data.enableHeadRotation,
                     huyangConfig.data.enableBodyMovement,
                     huyangConfig.data.enableBodyRotation,
                     huyangConfig.data.enableTorsoLights);
    webserver->start();

    // 7. Serial CLI
    huyangCLI.setup();

    // 8. OTA Updates - funktioniert in beiden Modi (STA + AP)
    ArduinoOTA.setHostname("huyang");
    // OTA-Passwort optional - kann ueber HuyangConfig erweitert werden
    // ArduinoOTA.setPassword("huyang-ota");
    ArduinoOTA.onStart([]() {
        Serial.println(F("OTA Update start"));
        huyangBody->setStatus(HuyangBody::StatusError); // rot blinkend waehrend Flash
        huyangBody->updateChestLights();
    });
    ArduinoOTA.onEnd([]() {
        Serial.println(F("OTA Update done - reboot"));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)         Serial.println(F("Auth Failed"));
        else if (error == OTA_BEGIN_ERROR)   Serial.println(F("Begin Failed"));
        else if (error == OTA_CONNECT_ERROR) Serial.println(F("Connect Failed"));
        else if (error == OTA_RECEIVE_ERROR) Serial.println(F("Receive Failed"));
        else if (error == OTA_END_ERROR)     Serial.println(F("End Failed"));
    });
    ArduinoOTA.begin();
    Serial.println(F("OTA bereit (Hostname: huyang)"));

    // 9. MQTT (no-op wenn HUYANG_MQTT_ENABLED=false in config.h)
    huyangMqtt->setup(HUYANG_MQTT_HOST, HUYANG_MQTT_PORT,
                      HUYANG_MQTT_USER, HUYANG_MQTT_PASS,
                      HUYANG_MQTT_BASETOPIC);

    Serial.println(F("Setup done."));
}

void loop()
{
    currentMillis = millis();

    // WiFi maintenance (reconnect if needed)
    huyangWifi.loop();

    // OTA Update Handle
    ArduinoOTA.handle();

    // Status-LED auf WiFi-Mode-Wechsel reagieren (z.B. AP -> STA via Auto-Retry)
    String currentMode = huyangWifi.getMode();
    if (currentMode != lastWifiMode)
    {
        if (currentMode == "STA" && huyangWifi.isConnected())
        {
            huyangBody->setStatus(HuyangBody::StatusConnectedSTA);
        }
        else if (currentMode == "AP")
        {
            huyangBody->setStatus(HuyangBody::StatusAPMode);
        }
        lastWifiMode = currentMode;
    }

    // Process serial commands
    huyangCLI.loop();

    // Periodic IP address display
    if (currentMillis - previousMillisIP > 5000)
    {
        previousMillisIP = currentMillis;

        if (huyangWifi.isConnected())
        {
            Serial.print(F("http://"));
            Serial.print(huyangWifi.getIP());
            if (WebServerPort != 80)
            {
                Serial.print(F(":"));
                Serial.print(WebServerPort);
            }
            Serial.println();
        }
    }

    // Face / Eyes
    huyangFace->automatic = webserver->automaticAnimations;

    if (huyangFace->automatic == false)
    {
        if (webserver->allEyes != 0)
        {
            HuyangFace::EyeState newState = huyangFace->getStateFrom(webserver->allEyes);
            huyangFace->setEyesTo(newState);

            if (newState == HuyangFace::EyeState::Blink)
            {
                webserver->allEyes = HuyangFace::EyeState::Open;
            }
        }
        else
        {
            if (webserver->leftEye != 0)
            {
                HuyangFace::EyeState newState = huyangFace->getStateFrom(webserver->leftEye);
                huyangFace->setLeftEyeTo(newState);

                if (newState == HuyangFace::EyeState::Blink)
                {
                    webserver->leftEye = HuyangFace::EyeState::Open;
                }
            }

            if (webserver->rightEye != 0)
            {
                HuyangFace::EyeState newState = huyangFace->getStateFrom(webserver->rightEye);
                huyangFace->setRightEyeTo(newState);

                if (newState == HuyangFace::EyeState::Blink)
                {
                    webserver->rightEye = HuyangFace::EyeState::Open;
                }
            }
        }
    }

    huyangFace->loop();

    // Neck
    huyangNeck->automatic = webserver->automaticAnimations;
    if (huyangNeck->automatic == false)
    {
        huyangNeck->rotateHead(webserver->neckRotate);
        huyangNeck->tiltNeckForward(webserver->neckTiltForward);
        huyangNeck->tiltNeckSideways(webserver->neckTiltSideways);
    }
    huyangNeck->loop();

    // Body
    huyangBody->automatic = webserver->automaticAnimations;
    if (huyangBody->automatic == false)
    {
        huyangBody->rotateBody(webserver->bodyRotate);
        huyangBody->tiltBodyForward(webserver->bodyTiltForward);
        huyangBody->tiltBodySideways(webserver->bodyTiltSideways);
    }
    huyangBody->loop();

    huyangAudio->loop();

    huyangSequence->loop();

    huyangMqtt->loop();
}
