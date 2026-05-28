//
// Huyang Default Configuration
// These values are used as fallback when EEPROM is empty or corrupted.
// Once configured via CLI, changes are stored in EEPROM.
//

// WiFi Mode: 0 = AP (hotspot), 1 = STA (connect to network)
#define DefaultWifiMode 1

// Hotspot settings (AP mode)
#define WifiSsidOfHotspot "HuyangWifiControl"
#define WifiPasswordHotspot ""

// Network to connect to (STA mode)
// Configure via CLI: wifi ssid <name>, wifi password <pass>, save
#define WifiSsidConnectTo ""
#define WifiPasswordConnectTo ""

// Webserver Port
#define WebServerPort 80

// Web-Interface Authentication (HTTP Basic Auth)
// Auf true setzen um Login-Schutz zu aktivieren. Standard-Credentials unten anpassen!
#define HUYANG_WEB_AUTH_ENABLED false
#define HUYANG_WEB_USER "admin"
#define HUYANG_WEB_PASS "huyang"

// MQTT-Integration (optional, fuer Home-Assistant etc.)
// Wenn true: PubSubClient-Library muss installiert sein!
// Bei false (default) kompiliert HuyangMqtt zu nichts.
#define HUYANG_MQTT_ENABLED false
#define HUYANG_MQTT_HOST "192.168.1.10"
#define HUYANG_MQTT_PORT 1883
#define HUYANG_MQTT_USER ""
#define HUYANG_MQTT_PASS ""
#define HUYANG_MQTT_BASETOPIC "huyang"

// Feature defaults
#define DefaultEnableEyes true
#define DefaultEnableMonacle true
#define DefaultEnableNeckMovement true
#define DefaultEnableHeadRotation true
#define DefaultEnableBodyMovement true
#define DefaultEnableBodyRotation true
#define DefaultEnableTorsoLights true
