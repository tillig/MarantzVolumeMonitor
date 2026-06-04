#include "ReceiverStatusScreen.h"
#include "HomeScreen.h"
#include "ReceiverIpScreen.h"
#include "ReceiverListScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../../network/MarantzClient.h"
#include "../../network/WiFiManager.h"
#include "../../storage/ConfigStore.h"

ReceiverStatusScreen::ReceiverStatusScreen(const ReceiverCandidate& candidate, ScreenReturnTarget returnTarget)
    : _candidate(candidate), _returnTarget(returnTarget) {}

void ReceiverStatusScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextDatum(TC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString("Verify Receiver", 240, 14, 4);

    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.drawString(_candidate.name, 240, 78, 2);
    tft.drawString(_candidate.ipAddress, 240, 104, 4);

    if (!_complete) {
        tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
        tft.drawString("Requesting live receiver status...", 240, 166, 2);
        return;
    }

    if (_result.success) {
        tft.setTextColor(TFT_GREEN);
        tft.drawString("Receiver saved", 240, 166, 4);
        tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
        String returnText = _returnTarget == ScreenReturnTarget::Settings ? "Returning to Settings"
                                                                          : "Returning to Home";
        tft.drawString(returnText, 240, 204, 2);
    } else {
        tft.setTextColor(TFT_RED);
        tft.drawString(failureText(), 240, 156, 2);

        tft.fillRoundRect(24, 246, 128, 44, 6, DisplayManager::COLOR_PANEL);
        tft.fillRoundRect(176, 246, 128, 44, 6, DisplayManager::COLOR_PANEL);
        tft.fillRoundRect(328, 246, 128, 44, 6, DisplayManager::COLOR_PANEL);
        tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
        tft.drawString("Retry", 88, 268, 2);
        tft.drawString("Manual", 240, 268, 2);
        tft.drawString("Discover", 392, 268, 2);
    }
}

void ReceiverStatusScreen::update() {
    if (!_started) {
        _started = true;
        _startedAtMs = millis();
        verify();
        draw();
    }

    if (_complete && _result.success && millis() - _startedAtMs >= 1200) {
        if (_returnTarget == ScreenReturnTarget::Settings) {
            ScreenManager::getInstance().setScreen(new SettingsScreen());
        } else {
            ScreenManager::getInstance().setScreen(new HomeScreen());
        }
    }
}

void ReceiverStatusScreen::handleTouch(TS_Point p) {
    if (!_complete || _result.success) {
        return;
    }

    if (p.y >= 246 && p.y <= 290) {
        if (p.x >= 24 && p.x <= 152) {
            _started = false;
            _complete = false;
            _result = ReceiverVerificationResult();
            draw();
        } else if (p.x >= 176 && p.x <= 304) {
            ScreenManager::getInstance().setScreen(new ReceiverIpScreen(_returnTarget));
        } else if (p.x >= 328 && p.x <= 456) {
            ScreenManager::getInstance().setScreen(new ReceiverListScreen(_returnTarget));
        }
    }
}

void ReceiverStatusScreen::verify() {
    _result.ipAddress = _candidate.ipAddress;
    if (!WiFiManager::getInstance().isConnected()) {
        _result.failureReason = ReceiverFailureReason::NetworkUnavailable;
        _complete = true;
        return;
    }

    if (!ReceiverDiscovery::isValidIpv4(_candidate.ipAddress)) {
        _result.failureReason = ReceiverFailureReason::InvalidAddress;
        _complete = true;
        return;
    }

    MarantzStatus status;
    _result.statusValid = MarantzClient::getInstance().verifyReceiver(_candidate.ipAddress, &status);
    _result.success = _result.statusValid;
    _result.failureReason = _result.success ? ReceiverFailureReason::None : ReceiverFailureReason::NoStatus;

    if (_result.success) {
        saveVerifiedReceiver();
    }

    _complete = true;
}

void ReceiverStatusScreen::saveVerifiedReceiver() {
    DeviceConfig config;
    ConfigStore::getInstance().loadConfig(config);
    config.receiverIp = _candidate.ipAddress;
    if (!ConfigStore::getInstance().saveConfig(config)) {
        _result.success = false;
        _result.failureReason = ReceiverFailureReason::SaveFailed;
    }
}

String ReceiverStatusScreen::failureText() const {
    switch (_result.failureReason) {
        case ReceiverFailureReason::InvalidAddress:
            return "Invalid receiver address";
        case ReceiverFailureReason::NetworkUnavailable:
            return "Wi-Fi is not connected";
        case ReceiverFailureReason::SaveFailed:
            return "Receiver verified, save failed";
        case ReceiverFailureReason::Timeout:
        case ReceiverFailureReason::NoStatus:
        default:
            return "Receiver status unavailable";
    }
}
