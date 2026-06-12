#include "ReceiverStatusScreen.h"
#include "HomeScreen.h"
#include "ReceiverIpScreen.h"
#include "ReceiverListScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"
#include "../../network/MarantzClient.h"
#include "../../network/WiFiManager.h"
#include "../../storage/ConfigStore.h"

ReceiverStatusScreen::ReceiverStatusScreen(const ReceiverCandidate& candidate, ScreenReturnTarget returnTarget)
    : _candidate(candidate), _returnTarget(returnTarget) {}

void ReceiverStatusScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawText(tft, "Receiver Setup", 240, 14, MaterialStyle::TextRole::ScreenTitle, TC_DATUM);

    if (_complete && _result.success) {
        MaterialStyle::drawStatusBlock(
            tft, MaterialStyle::StatusKind::Success, "Receiver saved", "Tap OK to continue.", Icons::SUCCESS);
        MaterialStyle::drawStandardButton(tft,
                                          182,
                                          MaterialStyle::BottomActionY,
                                          116,
                                          MaterialStyle::ButtonHeight,
                                          Icons::KEYBOARD_OK,
                                          "OK",
                                          MaterialStyle::ComponentState::Success);
        return;
    }

    if (!_complete) {
        bool showProgress = _started && millis() - _startedAtMs >= MaterialStyle::ProgressThresholdMs;
        MaterialStyle::drawStatusBlock(tft,
                                       showProgress ? MaterialStyle::StatusKind::Loading
                                                    : MaterialStyle::StatusKind::Unavailable,
                                       "Verifying...",
                                       MaterialStyle::truncateToWidth(tft, _candidate.name, 340, 2),
                                       Icons::SCAN,
                                       _progressFrame);
        return;
    }

    MaterialStyle::drawStatusBlock(
        tft, MaterialStyle::StatusKind::Error, failureText(), "Choose a recovery action.", Icons::FAILURE);
    MaterialStyle::drawStandardButton(
        tft, 20, MaterialStyle::BottomActionY, 130, MaterialStyle::ButtonHeight, Icons::RETRY, "Retry");
    MaterialStyle::drawStandardButton(
        tft, 175, MaterialStyle::BottomActionY, 130, MaterialStyle::ButtonHeight, Icons::MANUAL_ENTRY, "Manual");
    MaterialStyle::drawStandardButton(
        tft, 330, MaterialStyle::BottomActionY, 130, MaterialStyle::ButtonHeight, Icons::SCAN, "Discover");
}

void ReceiverStatusScreen::update() {
    if (!_started) {
        _started = true;
        _startedAtMs = millis();
        _lastProgressAtMs = _startedAtMs;
        verify();
        draw();
    }

    if (!_complete && millis() - _startedAtMs >= MaterialStyle::ProgressThresholdMs &&
        millis() - _lastProgressAtMs >= MaterialStyle::ProgressFrameMs) {
        _lastProgressAtMs = millis();
        _progressFrame++;
        draw();
    }
}

void ReceiverStatusScreen::handleTouch(TS_Point p) {
    if (!_complete) {
        return;
    }

    if (p.y >= MaterialStyle::BottomActionY && p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        if (_result.success && p.x >= 182 && p.x <= 298) {
            if (_returnTarget == ScreenReturnTarget::Settings) {
                ScreenManager::getInstance().setScreen(new SettingsScreen());
            } else {
                ScreenManager::getInstance().setScreen(new HomeScreen());
            }
            return;
        }

        if (_result.success) {
            return;
        }

        if (p.x >= 20 && p.x <= 150) {
            _started = false;
            _complete = false;
            _result = ReceiverVerificationResult();
            draw();
        } else if (p.x >= 175 && p.x <= 305) {
            ScreenManager::getInstance().setScreen(new ReceiverIpScreen(_returnTarget));
        } else if (p.x >= 330 && p.x <= 460) {
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
