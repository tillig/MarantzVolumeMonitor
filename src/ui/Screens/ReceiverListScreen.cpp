#include "ReceiverListScreen.h"
#include "HomeScreen.h"
#include "ReceiverIpScreen.h"
#include "ReceiverStatusScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"

ReceiverListScreen::ReceiverListScreen(ScreenReturnTarget returnTarget) : _returnTarget(returnTarget) {
    startDiscovery();
}

void ReceiverListScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
    drawHeader(tft);

    ReceiverDiscovery& discovery = ReceiverDiscovery::getInstance();
    const std::vector<ReceiverCandidate>& candidates = discovery.getCandidates();
    ReceiverDiscoveryState state = discovery.getState();

    if (state == ReceiverDiscoveryState::Searching) {
        MaterialStyle::drawStatusBlock(
            tft, MaterialStyle::StatusKind::Loading, "Searching for receivers...", "", Icons::SCAN, _progressFrame);
    } else if (candidates.empty()) {
        MaterialStyle::drawStatusBlock(tft,
                                       MaterialStyle::StatusKind::Empty,
                                       "No receivers found",
                                       "Rescan or enter an IP address.",
                                       Icons::WARNING);
    } else {
        for (size_t i = 0; i < candidates.size() && i < 3; ++i) {
            drawCandidateRow(tft,
                             candidates[i],
                             i,
                             MaterialStyle::SetupListTopY +
                                 (int)i * (MaterialStyle::ListRowHeight + MaterialStyle::RowGap));
        }
    }

    drawActions(tft);
    _lastState = state;
    _lastCandidateCount = candidates.size();
}

void ReceiverListScreen::update() {
    ReceiverDiscovery& discovery = ReceiverDiscovery::getInstance();
    discovery.update();

    ReceiverDiscoveryState state = discovery.getState();
    size_t candidateCount = discovery.getCandidates().size();
    if (state != _lastState || candidateCount != _lastCandidateCount) {
        draw();
    } else if (state == ReceiverDiscoveryState::Searching) {
        uint32_t now = millis();
        if (now - _lastProgressAtMs >= MaterialStyle::ProgressFrameMs) {
            _lastProgressAtMs = now;
            _progressFrame++;
            TFT_eSPI& tft = DisplayManager::getInstance().getTft();
            MaterialStyle::clearProgressBar(tft,
                                            MaterialStyle::StatusBlockProgressX,
                                            MaterialStyle::StatusBlockProgressY,
                                            MaterialStyle::StatusBlockProgressW);
            MaterialStyle::drawProgressBar(tft,
                                           MaterialStyle::StatusBlockProgressX,
                                           MaterialStyle::StatusBlockProgressY,
                                           MaterialStyle::StatusBlockProgressW,
                                           _progressFrame);
        }
    }
}

void ReceiverListScreen::handleTouch(TS_Point p) {
    int selectedIndex = touchedCandidateIndex(p);
    const std::vector<ReceiverCandidate>& candidates = ReceiverDiscovery::getInstance().getCandidates();
    if (selectedIndex >= 0 && selectedIndex < (int)candidates.size()) {
        ReceiverDiscovery::getInstance().stop();
        ScreenManager::getInstance().setScreen(new ReceiverStatusScreen(candidates[selectedIndex], _returnTarget));
        return;
    }

    if (p.y >= MaterialStyle::BottomActionY && p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        if (p.x >= 20 && p.x <= 150) {
            ReceiverDiscovery::getInstance().stop();
            ScreenManager::getInstance().setScreen(new ReceiverIpScreen(_returnTarget));
        } else if (p.x >= 175 && p.x <= 305) {
            startDiscovery();
            draw();
        } else if (p.x >= 330 && p.x <= 460) {
            ReceiverDiscovery::getInstance().stop();
            if (_returnTarget == ScreenReturnTarget::Settings) {
                ScreenManager::getInstance().setScreen(new SettingsScreen());
            } else {
                ScreenManager::getInstance().setScreen(new HomeScreen());
            }
        }
    }
}

void ReceiverListScreen::drawHeader(TFT_eSPI& tft) {
    MaterialStyle::drawPageHeader(tft, Icons::RECEIVER, "Receiver Setup", "Select a discovered receiver");
}

void ReceiverListScreen::drawActions(TFT_eSPI& tft) {
    const char* labels[] = {"Manual", "Rescan", "Cancel"};
    for (int i = 0; i < 3; ++i) {
        int x = 20 + i * 155;
        const Icons::IconBitmap* icon = i == 0   ? &Icons::MANUAL_ENTRY
                                        : i == 1 ? &Icons::RETRY
                                                 : &Icons::KEYBOARD_CANCEL;
        MaterialStyle::ComponentState state =
            i == 2 ? MaterialStyle::ComponentState::Error : MaterialStyle::ComponentState::Normal;
        MaterialStyle::drawStandardButton(
            tft, x, MaterialStyle::BottomActionY, 130, MaterialStyle::ButtonHeight, *icon, labels[i], state);
    }
}

void ReceiverListScreen::drawCandidateRow(TFT_eSPI& tft, const ReceiverCandidate& candidate, int index, int y) {
    MaterialStyle::drawListRow(tft,
                               {24,
                                y,
                                432,
                                MaterialStyle::ListRowHeight,
                                &Icons::RECEIVER,
                                0,
                                candidate.name,
                                candidate.ipAddress,
                                String(index + 1),
                                MaterialStyle::ComponentState::Normal});
}

void ReceiverListScreen::startDiscovery() {
    ReceiverDiscovery::getInstance().start();
    _lastState = ReceiverDiscoveryState::Idle;
    _lastCandidateCount = 0;
    _progressFrame = 0;
    _lastProgressAtMs = millis();
}

int ReceiverListScreen::touchedCandidateIndex(TS_Point p) const {
    if (p.x < 24 || p.x > 456) {
        return -1;
    }

    for (int i = 0; i < 3; ++i) {
        int y = MaterialStyle::SetupListTopY + i * (MaterialStyle::ListRowHeight + MaterialStyle::RowGap);
        if (p.y >= y && p.y <= y + MaterialStyle::ListRowHeight) {
            return i;
        }
    }

    return -1;
}
