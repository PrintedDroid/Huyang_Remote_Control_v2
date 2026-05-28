#ifndef HuyangConfig_h
#define HuyangConfig_h

#include <Arduino.h>
#include <EEPROM.h>

#define HUYANG_EEPROM_SIZE 1024
#define HUYANG_EEPROM_MAGIC 0x4855 // "HU"
#define HUYANG_CONFIG_VERSION 3    // Bumped: customSeqBlob hinzu (Custom-Sequence in EEPROM)
#define HUYANG_CUSTOM_SEQ_MAX_BYTES 224 // 32 Steps * 7 Bytes (SequenceStep packed)

struct __attribute__((packed)) HuyangConfigData
{
	// System identification
	uint16_t magic;
	uint8_t version;
	uint32_t writeCount;

	// WiFi configuration
	char wifiSSID[32];
	char wifiPassword[64];
	char apSSID[32];
	char apPassword[64];
	uint8_t wifiMode; // 0 = AP, 1 = STA

	// Feature flags
	bool enableEyes;
	bool enableMonacle;
	bool enableNeckMovement;
	bool enableHeadRotation;
	bool enableBodyMovement;
	bool enableBodyRotation;
	bool enableTorsoLights;

	// Calibration offsets
	int8_t calNeckRotation;
	int8_t calNeckTiltForward;
	int8_t calNeckTiltSideways;

	// Audio
	uint8_t volume;

	// Custom-Sequence (binaere SequenceStep[]-Blob)
	uint8_t customSeqCount;                          // Anzahl belegter Steps (0..32)
	uint8_t customSeqBlob[HUYANG_CUSTOM_SEQ_MAX_BYTES]; // 32 Steps * 7 Bytes

	// Data integrity (MUST be the last field)
	uint32_t checksum;
};

// Compile-time safety checks
static_assert(sizeof(HuyangConfigData) <= HUYANG_EEPROM_SIZE,
	"HuyangConfigData exceeds EEPROM allocation");
static_assert(offsetof(HuyangConfigData, checksum) == sizeof(HuyangConfigData) - sizeof(uint32_t),
	"checksum must be the last field in HuyangConfigData");

class HuyangConfig
{
public:
	HuyangConfigData data;

	void setup();
	void load();
	void save();
	void resetToDefaults();
	void printConfig();

private:
	HuyangConfigData _lastSaved;

	void setDefaults();
	uint32_t calculateChecksum();
};

#endif
