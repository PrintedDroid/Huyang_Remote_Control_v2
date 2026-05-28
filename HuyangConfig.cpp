#include "HuyangConfig.h"
#include "config.h"

void HuyangConfig::setup()
{
	EEPROM.begin(HUYANG_EEPROM_SIZE);
	load();
}

void HuyangConfig::load()
{
	EEPROM.get(0, data);

	if (data.magic != HUYANG_EEPROM_MAGIC || data.version != HUYANG_CONFIG_VERSION)
	{
		Serial.println(F("No valid config found, loading defaults..."));
		resetToDefaults();
		return;
	}

	// Verify checksum
	uint32_t storedChecksum = data.checksum;
	data.checksum = 0;
	uint32_t calculated = calculateChecksum();

	if (storedChecksum != calculated)
	{
		Serial.println(F("Config checksum mismatch, loading defaults..."));
		resetToDefaults();
		return;
	}

	data.checksum = storedChecksum;

	// Belt-and-suspenders: ensure all string fields are null-terminated
	data.wifiSSID[sizeof(data.wifiSSID) - 1] = '\0';
	data.wifiPassword[sizeof(data.wifiPassword) - 1] = '\0';
	data.apSSID[sizeof(data.apSSID) - 1] = '\0';
	data.apPassword[sizeof(data.apPassword) - 1] = '\0';

	memcpy(&_lastSaved, &data, sizeof(data));
	Serial.printf("Config loaded (writes: %lu)\n", (unsigned long)data.writeCount);
}

void HuyangConfig::save()
{
	// Only write bytes that changed (wear leveling)
	if (memcmp(&data, &_lastSaved, sizeof(data) - sizeof(uint32_t)) == 0)
	{
		Serial.println(F("Nothing changed."));
		return;
	}

	data.writeCount++;
	data.checksum = 0;
	data.checksum = calculateChecksum();

	uint8_t *configBytes = (uint8_t *)&data;
	uint8_t *lastBytes = (uint8_t *)&_lastSaved;
	size_t bytesWritten = 0;

	for (size_t i = 0; i < sizeof(data); i++)
	{
		if (configBytes[i] != lastBytes[i])
		{
			EEPROM.write(i, configBytes[i]);
			bytesWritten++;
		}
	}

	EEPROM.commit();
	memcpy(&_lastSaved, &data, sizeof(data));
	Serial.printf("EEPROM: %u/%u bytes written (total writes: %lu)\n",
		bytesWritten, sizeof(data), (unsigned long)data.writeCount);
}

void HuyangConfig::resetToDefaults()
{
	// Only preserve writeCount if existing data had valid magic (not first boot garbage)
	uint32_t previousWriteCount = (data.magic == HUYANG_EEPROM_MAGIC) ? data.writeCount : 0;
	setDefaults();
	data.writeCount = previousWriteCount;
	data.checksum = 0;
	data.checksum = calculateChecksum();
	EEPROM.put(0, data);
	EEPROM.commit();
	memcpy(&_lastSaved, &data, sizeof(data));
	Serial.println(F("Defaults saved to EEPROM"));
}

void HuyangConfig::setDefaults()
{
	memset(&data, 0, sizeof(data));
	data.magic = HUYANG_EEPROM_MAGIC;
	data.version = HUYANG_CONFIG_VERSION;

	// WiFi defaults from config.h
	strncpy(data.wifiSSID, WifiSsidConnectTo, sizeof(data.wifiSSID) - 1);
	strncpy(data.wifiPassword, WifiPasswordConnectTo, sizeof(data.wifiPassword) - 1);
	strncpy(data.apSSID, WifiSsidOfHotspot, sizeof(data.apSSID) - 1);
	strncpy(data.apPassword, WifiPasswordHotspot, sizeof(data.apPassword) - 1);
	data.wifiMode = DefaultWifiMode;

	// Feature flags from config.h
	data.enableEyes = DefaultEnableEyes;
	data.enableMonacle = DefaultEnableMonacle;
	data.enableNeckMovement = DefaultEnableNeckMovement;
	data.enableHeadRotation = DefaultEnableHeadRotation;
	data.enableBodyMovement = DefaultEnableBodyMovement;
	data.enableBodyRotation = DefaultEnableBodyRotation;
	data.enableTorsoLights = DefaultEnableTorsoLights;

	// Calibration
	data.calNeckRotation = 0;
	data.calNeckTiltForward = 0;
	data.calNeckTiltSideways = 0;

	// Audio
	data.volume = 20;
}

void HuyangConfig::printConfig()
{
	Serial.println(F("\n=== Huyang Configuration ==="));
	Serial.printf("EEPROM writes: %lu\n", (unsigned long)data.writeCount);
	Serial.printf("WiFi Mode: %s\n", data.wifiMode == 1 ? "STA (connect to network)" : "AP (hotspot)");
	Serial.printf("WiFi SSID: %s\n", data.wifiSSID);
	Serial.printf("AP SSID: %s\n", data.apSSID);
	Serial.println(F("--- Features ---"));
	Serial.printf("Eyes: %s\n", data.enableEyes ? "on" : "off");
	Serial.printf("Monacle: %s\n", data.enableMonacle ? "on" : "off");
	Serial.printf("Neck Movement: %s\n", data.enableNeckMovement ? "on" : "off");
	Serial.printf("Head Rotation: %s\n", data.enableHeadRotation ? "on" : "off");
	Serial.printf("Body Movement: %s\n", data.enableBodyMovement ? "on" : "off");
	Serial.printf("Body Rotation: %s\n", data.enableBodyRotation ? "on" : "off");
	Serial.printf("Torso Lights: %s\n", data.enableTorsoLights ? "on" : "off");
	Serial.println(F("--- Calibration ---"));
	Serial.printf("Neck Rotation: %d\n", data.calNeckRotation);
	Serial.printf("Neck Tilt Forward: %d\n", data.calNeckTiltForward);
	Serial.printf("Neck Tilt Sideways: %d\n", data.calNeckTiltSideways);
	Serial.printf("Volume: %d\n", data.volume);
	Serial.println(F("============================\n"));
}

uint32_t HuyangConfig::calculateChecksum()
{
	// CRC-32 (IEEE 802.3 polynomial)
	uint32_t crc = 0xFFFFFFFF;
	uint8_t *bytes = (uint8_t *)&data;
	size_t len = sizeof(data) - sizeof(uint32_t); // exclude checksum field

	for (size_t i = 0; i < len; i++)
	{
		crc ^= bytes[i];
		for (int j = 0; j < 8; j++)
		{
			crc = (crc >> 1) ^ (0xEDB88320 & -(crc & 1));
		}
	}
	return ~crc;
}
