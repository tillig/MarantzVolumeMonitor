#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

struct TouchCalibrationConfig {
    bool isPresent;
    uint8_t version;
    float xFromRawX;
    float xFromRawY;
    float xOffset;
    float yFromRawX;
    float yFromRawY;
    float yOffset;

    TouchCalibrationConfig()
        : isPresent(false),
          version(1),
          xFromRawX(0.0f),
          xFromRawY(0.0f),
          xOffset(0.0f),
          yFromRawX(0.0f),
          yFromRawY(0.0f),
          yOffset(0.0f) {}
};

struct DeviceConfig {
    String wifiSsid;
    String wifiPassword;
    String receiverIp;
    int brightness;
    bool useDbScale;
    TouchCalibrationConfig touchCalibration;

    DeviceConfig()
        : wifiSsid(""),
          wifiPassword(""),
          receiverIp(""),
          brightness(255),
          useDbScale(false) {}
};

class ConfigStore {
public:
    enum class ResetTarget {
        Wifi,
        Receiver,
        Calibration
    };

    static ConfigStore& getInstance() {
        static ConfigStore instance;
        return instance;
    }

    bool begin();
    bool loadConfig(DeviceConfig& config);
    bool saveConfig(const DeviceConfig& config);
    bool resetToDefaults(ResetTarget target);

private:
    ConfigStore() {}
    bool ensureReady();
    bool loadDocument(JsonDocument& doc);

    const char* CONFIG_FILE = "/config.json";
    bool _isReady = false;
};

#endif
