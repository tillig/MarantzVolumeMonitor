#include "ConfigStore.h"

#include <math.h>

namespace {
bool readFiniteFloat(JsonVariantConst value, float& target) {
    if (value.isNull()) {
        return false;
    }

    float candidate = value.as<float>();
    if (!isfinite(candidate)) {
        return false;
    }

    target = candidate;
    return true;
}
}

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

bool ConfigStore::loadDocument(JsonDocument& doc) {
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

    DeserializationError error = deserializeJson(doc, configFile);
    configFile.close();

    if (error) {
        Serial.println("Failed to parse config file");
        return false;
    }

    return true;
}

bool ConfigStore::loadConfig(DeviceConfig& config) {
    JsonDocument doc;
    if (!loadDocument(doc)) {
        return false;
    }

    config.wifiSsid = doc["wifiSsid"] | "";
    config.wifiPassword = doc["wifiPassword"] | "";
    config.receiverIp = doc["receiverIp"] | "";
    config.brightness = doc["brightness"] | 255;
    config.useDbScale = doc["useDbScale"] | false;
    config.touchCalibration = TouchCalibrationConfig();

    JsonVariantConst calibration = doc["touchCalibration"];
    if (calibration.is<JsonObjectConst>()) {
        TouchCalibrationConfig savedCalibration;
        savedCalibration.version = calibration["version"] | 1;
        if (readFiniteFloat(calibration["xFromRawX"], savedCalibration.xFromRawX) &&
            readFiniteFloat(calibration["xFromRawY"], savedCalibration.xFromRawY) &&
            readFiniteFloat(calibration["xOffset"], savedCalibration.xOffset) &&
            readFiniteFloat(calibration["yFromRawX"], savedCalibration.yFromRawX) &&
            readFiniteFloat(calibration["yFromRawY"], savedCalibration.yFromRawY) &&
            readFiniteFloat(calibration["yOffset"], savedCalibration.yOffset)) {
            savedCalibration.isPresent = true;
            config.touchCalibration = savedCalibration;
        }
    }

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
    if (config.touchCalibration.isPresent) {
        JsonObject calibration = doc["touchCalibration"].to<JsonObject>();
        calibration["version"] = config.touchCalibration.version;
        calibration["xFromRawX"] = config.touchCalibration.xFromRawX;
        calibration["xFromRawY"] = config.touchCalibration.xFromRawY;
        calibration["xOffset"] = config.touchCalibration.xOffset;
        calibration["yFromRawX"] = config.touchCalibration.yFromRawX;
        calibration["yFromRawY"] = config.touchCalibration.yFromRawY;
        calibration["yOffset"] = config.touchCalibration.yOffset;
    } else {
        doc.remove("touchCalibration");
    }

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

bool ConfigStore::resetToDefaults(ResetTarget target) {
    DeviceConfig config;
    loadConfig(config);

    switch (target) {
        case ResetTarget::Wifi:
            config.wifiSsid = "";
            config.wifiPassword = "";
            break;
        case ResetTarget::Receiver:
            config.receiverIp = "";
            break;
        case ResetTarget::Calibration:
            config.touchCalibration = TouchCalibrationConfig();
            break;
        default:
            break;
    }

    return saveConfig(config);
}
