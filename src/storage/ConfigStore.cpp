#include "ConfigStore.h"

bool ConfigStore::ensureReady() {
    return _isReady || begin();
}

bool ConfigStore::begin() {
    if (_isReady) {
        return true;
    }

    if (!LittleFS.begin(true)) {
        Serial.println("An Error has occurred while mounting LittleFS");
        return false;
    }

    _isReady = true;
    return true;
}

bool ConfigStore::loadConfig(DeviceConfig& config) {
    if (!ensureReady()) {
        return false;
    }

    if (!LittleFS.exists(CONFIG_FILE)) {
        return false;
    }

    File configFile = LittleFS.open(CONFIG_FILE, "r");
    if (!configFile) {
        Serial.println("Failed to open config file");
        return false;
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();

    if (error) {
        Serial.println("Failed to parse config file");
        return false;
    }

    config.wifiSsid = doc["wifiSsid"] | "";
    config.wifiPassword = doc["wifiPassword"] | "";
    config.receiverIp = doc["receiverIp"] | "";
    config.brightness = doc["brightness"] | 255;
    config.useDbScale = doc["useDbScale"] | false;

    return true;
}

bool ConfigStore::saveConfig(const DeviceConfig& config) {
    if (!ensureReady()) {
        return false;
    }

    JsonDocument doc;
    doc["wifiSsid"] = config.wifiSsid;
    doc["wifiPassword"] = config.wifiPassword;
    doc["receiverIp"] = config.receiverIp;
    doc["brightness"] = config.brightness;
    doc["useDbScale"] = config.useDbScale;

    File configFile = LittleFS.open(CONFIG_FILE, "w");
    if (!configFile) {
        Serial.println("Failed to open config file for writing");
        return false;
    }

    if (serializeJson(doc, configFile) == 0) {
        Serial.println("Failed to write to file");
        configFile.close();
        return false;
    }

    configFile.close();
    return true;
}
