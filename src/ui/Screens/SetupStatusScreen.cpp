#include "SetupStatusScreen.h"
#include "HomeScreen.h"
#include "KeyboardScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../DisplayManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"
#include "../../network/WiFiManager.h"
#include "../../storage/ConfigStore.h"

SetupStatusScreen::SetupStatusScreen(const String& ssid, const String& password, ScreenReturnTarget returnTarget)
    : _ssid(ssid), _password(password), _returnTarget(returnTarget) {}

void SetupStatusScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawText(tft, "Wi-Fi Setup", 240, 10, MaterialStyle::TextRole::ScreenTitle, TC_DATUM);

    if (!_isConnecting && !_failed) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Unavailable,
                                       "Connecting to",
                                       MaterialStyle::truncateToWidth(tft, _ssid, 380, 4),
                                       Icons::WIFI, _progressFrame);
        WiFiManager::getInstance().startConnect(_ssid, _password);
        _isConnecting = true;
        _progressVisible = false;
        _startTime = millis();
        _lastProgressAtMs = _startTime;
    } else if (_failed) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Error,
                                       _failureMessage, "Check credentials and try again.",
                                       Icons::FAILURE);
        MaterialStyle::drawStandardButton(tft, 140, MaterialStyle::BottomActionY, 200,
                                          MaterialStyle::ButtonHeight,
                                          Icons::RETRY, "Retry");
    } else {
        bool showProgress = millis() - _startTime >= MaterialStyle::ProgressThresholdMs;
        _progressVisible = showProgress;
        if (showProgress) {
            MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Loading,
                                           "Connecting...",
                                           MaterialStyle::truncateToWidth(tft, _ssid, 380, 2),
                                           Icons::WIFI, _progressFrame);
        } else {
            MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Unavailable,
                                           "Connecting...",
                                           MaterialStyle::truncateToWidth(tft, _ssid, 380, 2),
                                           Icons::WIFI);
        }
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
        } else {
            uint32_t now = millis();
            bool shouldShowProgress = now - _startTime >= MaterialStyle::ProgressThresholdMs;
            if (shouldShowProgress && !_progressVisible) {
                _progressVisible = true;
                draw();
                return;
            }

            if (shouldShowProgress &&
                now - _lastProgressAtMs >= MaterialStyle::ProgressFrameMs) {
                _lastProgressAtMs = now;
                _progressFrame++;
                TFT_eSPI& tft = DisplayManager::getInstance().getTft();
                MaterialStyle::clearProgressBar(tft, MaterialStyle::StatusBlockProgressX,
                                                MaterialStyle::StatusBlockProgressY,
                                                MaterialStyle::StatusBlockProgressW);
                MaterialStyle::drawProgressBar(tft, MaterialStyle::StatusBlockProgressX,
                                               MaterialStyle::StatusBlockProgressY,
                                               MaterialStyle::StatusBlockProgressW,
                                               _progressFrame);
            }
        }
    }
}

void SetupStatusScreen::handleTouch(TS_Point p) {
    if (_failed && p.y >= MaterialStyle::BottomActionY &&
        p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        ScreenManager::getInstance().setScreen(new KeyboardScreen(_ssid, _returnTarget));
    }
}
