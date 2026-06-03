#ifndef CONFIG_STORE_H
#define CONFIG_STORE_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

struct DeviceConfig {
    String wifiSsid;
    String wifiPassword;
    String receiverIp;
    int brightness;
    bool useDbScale;

    DeviceConfig() : wifiSsid(""), wifiPassword(""), receiverIp(""), brightness(255), useDbScale(false) {}
};

class ConfigStore {
public:
    static ConfigStore& getInstance() {
        static ConfigStore instance;
        return instance;
    }

    bool begin();
    bool loadConfig(DeviceConfig& config);
    bool saveConfig(const DeviceConfig& config);

private:
    ConfigStore() {}
    bool ensureReady();

    const char* CONFIG_FILE = "/config.json";
    bool _isReady = false;
};

#endif
