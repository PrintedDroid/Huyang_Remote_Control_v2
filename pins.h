#ifndef HuyangPins_h
#define HuyangPins_h

//
// Huyang Pin Configuration
// Automatically selects the correct pins based on the board selected in Arduino IDE.
//
// Supported boards:
//   - ESP32-S3 (Waveshare ESP32-S3 Zero, Lolin S3 Mini, etc.)
//   - ESP32 generic (ESP32 Dev Module, Waveshare ESP32-Zero, etc.)
//   - ESP8266 (NodeMCU, Wemos D1 Mini, Generic ESP8266 Module, etc.)
//

// =============================================================================
// ESP32-S3 (Waveshare ESP32-S3 Zero, Lolin S3 Mini)
// Pins GPIO 1-13 are used — available on both boards.
// =============================================================================
#if defined(CONFIG_IDF_TARGET_ESP32S3)

    // TFT Display Pins (SPI)
    #define PIN_TFT_DC        4
    #define PIN_TFT_CS_LEFT   5
    #define PIN_TFT_CS_RIGHT  3
    #define PIN_TFT_RST       6
    #define PIN_SPI_SCK       10
    #define PIN_SPI_MOSI      11

    // I2C (PCA9685 Servo Shield)
    #define PIN_I2C_SDA       8
    #define PIN_I2C_SCL       9

    // DFPlayer Mini (HardwareSerial on ESP32)
    #define PIN_AUDIO_RX      1    // ESP RX <- DFPlayer TX
    #define PIN_AUDIO_TX      7    // ESP TX -> DFPlayer RX

    // NeoPixel Chest Lights
    #define PIN_NEOPIXEL      2

    // Sensors (optional, ESP32-S3 only)
    #define PIN_IR_RECEIVER   12
    #define PIN_PIR_SENSOR    13
    #define HUYANG_HAS_IR     1
    #define HUYANG_HAS_PIR    1

// =============================================================================
// ESP32 generic - default: Lolin/WeMos ESP32 D1 Mini on Printed-Droid PCB
// (PCB sockets D0..D8 = ESP32 GPIO 26/22/21/17/16/18/19/23/5)
// =============================================================================
#elif defined(ESP32)

    // TFT Display Pins (SPI) - PCB sockets: DC=D4, CS_L=D0, CS_R=D8, SCK=D5, MOSI=D7
    #define PIN_TFT_DC        16   // D4
    #define PIN_TFT_CS_LEFT   26   // D0
    #define PIN_TFT_CS_RIGHT  5    // D8
    #define PIN_TFT_RST       -1   // Display RST tied to ESP RST on this PCB, no GPIO needed
    #define PIN_SPI_SCK       18   // D5 (= ESP32 HSPI default)
    #define PIN_SPI_MOSI      23   // D7 (= ESP32 HSPI default)

    // I2C (PCA9685 Servo Shield) - PCB sockets: SDA=D2, SCL=D1
    #define PIN_I2C_SDA       21   // D2 (= ESP32 default I2C SDA)
    #define PIN_I2C_SCL       22   // D1 (= ESP32 default I2C SCL)

    // DFPlayer Mini (HardwareSerial on ESP32) - PCB sockets: D6 = ESP TX -> DFPlayer RX
    // No return line wired on the PCB, so RX = -1
    #define PIN_AUDIO_TX      19   // D6 -> DFPlayer RX
    #define PIN_AUDIO_RX      -1   // no feedback channel

    // NeoPixel Chest Lights - PCB sockets: D3
    #define PIN_NEOPIXEL      17   // D3

    // No sensors on Printed-Droid PCB by default
    #define HUYANG_HAS_IR     0
    #define HUYANG_HAS_PIR    0

// =============================================================================
// ESP8266 - default: Wemos D1 Mini / NodeMCU on Printed-Droid PCB
// (PCB sockets D0..D8 = ESP8266 GPIO 16/5/4/0/2/14/12/13/15)
// =============================================================================
#elif defined(ESP8266)

    // TFT Display Pins (SPI) - PCB sockets: DC=D4, CS_L=D0, CS_R=D8
    // SPI fix: SCK=GPIO 14 (D5), MOSI=GPIO 13 (D7) via Arduino_HWSPI
    #define PIN_TFT_DC        2    // D4
    #define PIN_TFT_CS_LEFT   16   // D0
    #define PIN_TFT_CS_RIGHT  15   // D8
    #define PIN_TFT_RST       -1   // Display RST tied to ESP RST on this PCB
                                   // (was 0 in v1.9 - collided with NeoPixel on D3!)

    // I2C (PCA9685) - PCB sockets: SDA=D2, SCL=D1 (= ESP8266 Wire defaults)
    #define PIN_I2C_SDA       4    // D2
    #define PIN_I2C_SCL       5    // D1

    // DFPlayer Mini (SoftwareSerial on ESP8266) - PCB sockets: D6 = ESP TX -> DFPlayer RX
    // No return line wired on the PCB, so RX = -1
    // (was RX=12, TX=-1 in v1.9 - wrong direction, DFPlayer never received commands!)
    #define PIN_AUDIO_TX      12   // D6 -> DFPlayer RX
    #define PIN_AUDIO_RX      -1   // no feedback channel

    // NeoPixel Chest Lights - PCB sockets: D3
    #define PIN_NEOPIXEL      0    // D3 (boot-strap pin - PCB should provide a pullup)

    // No sensors on Printed-Droid PCB
    #define HUYANG_HAS_IR     0
    #define HUYANG_HAS_PIR    0

#else
    #error "Unsupported board! Please select ESP8266, ESP32, or ESP32-S3 in Arduino IDE."
#endif

#endif
