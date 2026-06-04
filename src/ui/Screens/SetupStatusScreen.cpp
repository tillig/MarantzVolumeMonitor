#include "SetupStatusScreen.h"
#include "HomeScreen.h"
#include "KeyboardScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../DisplayManager.h"
#include "../../network/WiFiManager.h"
#include "../../storage/ConfigStore.h"

SetupStatusScreen::SetupStatusScreen(const String& ssid, const String& password, ScreenReturnTarget returnTarget)
    : _ssid(ssid), _password(password), _returnTarget(returnTarget) {}

void SetupStatusScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Wi-Fi Setup", 240, 10, 4);

    if (!_isConnecting && !_failed) {
        tft.drawString("Connecting to", 240, 100, 2);
        tft.drawString(_ssid, 240, 130, 4);
        WiFiManager::getInstance().startConnect(_ssid, _password);
        _isConnecting = true;
        _startTime = millis();
    } else if (_failed) {
        tft.setTextColor(TFT_RED);
        tft.drawString(_failureMessage, 240, 140, 4);

        tft.fillRoundRect(140, 240, 200, 40, 20, DisplayManager::COLOR_BAR_BG);
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
        tft.setTextDatum(MC_DATUM);
        tft.drawString("Retry", 240, 260, 2);
    } else {
        tft.drawString("Connecting...", 240, 180, 2);
    }
}

void SetupStatusScreen::update() {
    if (_isConnecting) {
        wl_status_t status = WiFiManager::getInstance().getConnectStatus();
        if (status == WL_CONNECTED) {
            _isConnecting = false;

            // Save Config
            DeviceConfig config;
            ConfigStore::getInstance().loadConfig(config);
            config.wifiSsid = _ssid;
            config.wifiPassword = _password;
            if (!ConfigStore::getInstance().saveConfig(config)) {
                _failed = true;
                _failureMessage = "Save Failed";
                Serial.println("WiFi connected, but saving credentials failed");
                draw();
                return;
            }

            Serial.println("WiFi Connected Successfully");
            if (_returnTarget == ScreenReturnTarget::Settings) {
                ScreenManager::getInstance().setScreen(new SettingsScreen());
            } else {
                ScreenManager::getInstance().setScreen(new HomeScreen());
            }
        } else if (status == WL_CONNECT_FAILED || status == WL_NO_SSID_AVAIL || (millis() - _startTime > 15000)) {
            _isConnecting = false;
            _failed = true;
            _failureMessage = "Connection Failed";
            draw();
        }
    }
}

void SetupStatusScreen::handleTouch(TS_Point p) {
    if (_failed && p.y > 220) {
        ScreenManager::getInstance().setScreen(new KeyboardScreen(_ssid, _returnTarget));
    }
}
