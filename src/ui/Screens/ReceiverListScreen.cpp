#include "ReceiverListScreen.h"
#include "HomeScreen.h"
#include "ReceiverIpScreen.h"
#include "ReceiverStatusScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"

ReceiverListScreen::ReceiverListScreen(ScreenReturnTarget returnTarget)
    : _returnTarget(returnTarget) {
    startDiscovery();
}

void ReceiverListScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
    drawHeader(tft);

    ReceiverDiscovery& discovery = ReceiverDiscovery::getInstance();
    const std::vector<ReceiverCandidate>& candidates = discovery.getCandidates();
    ReceiverDiscoveryState state = discovery.getState();

    tft.setTextDatum(MC_DATUM);
    if (state == ReceiverDiscoveryState::Searching) {
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
        tft.drawString("Searching for receivers...", 240, 84, 2);
    } else if (candidates.empty()) {
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
        tft.drawString("No receivers found", 240, 92, 4);
        tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
        tft.drawString("Retry discovery or enter an IP address.", 240, 130, 2);
    } else {
        for (size_t i = 0; i < candidates.size() && i < 3; ++i) {
            drawCandidateRow(tft, candidates[i], i, 72 + (int)i * 56);
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

    if (p.y >= 248 && p.y <= 302) {
        if (p.x >= 24 && p.x <= 152) {
            startDiscovery();
            draw();
        } else if (p.x >= 176 && p.x <= 304) {
            ReceiverDiscovery::getInstance().stop();
            ScreenManager::getInstance().setScreen(new ReceiverIpScreen(_returnTarget));
        } else if (p.x >= 328 && p.x <= 456) {
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
    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString("Receiver Setup", 20, 16, 4);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Select a discovered receiver", 20, 46, 2);
}

void ReceiverListScreen::drawActions(TFT_eSPI& tft) {
    const char* labels[] = {"Retry", "Manual", "Cancel"};
    for (int i = 0; i < 3; ++i) {
        int x = 24 + i * 152;
        tft.fillRoundRect(x, 248, 128, 44, 6, DisplayManager::COLOR_PANEL);
        tft.drawRoundRect(x, 248, 128, 44, 6, DisplayManager::COLOR_BAR_BG);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
        tft.drawString(labels[i], x + 64, 270, 2);
    }
}

void ReceiverListScreen::drawCandidateRow(TFT_eSPI& tft, const ReceiverCandidate& candidate, int index, int y) {
    int x = 24;
    tft.fillRoundRect(x, y, 432, 46, 6, DisplayManager::COLOR_PANEL);
    tft.drawRoundRect(x, y, 432, 46, 6, DisplayManager::COLOR_BAR_BG);

    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString(candidate.name, x + 14, y + 15, 2);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.drawString(candidate.ipAddress, x + 14, y + 34, 2);

    tft.setTextDatum(MR_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawNumber(index + 1, x + 410, y + 23, 4);
}

void ReceiverListScreen::startDiscovery() {
    ReceiverDiscovery::getInstance().start();
    _lastState = ReceiverDiscoveryState::Idle;
    _lastCandidateCount = 0;
}

int ReceiverListScreen::touchedCandidateIndex(TS_Point p) const {
    if (p.x < 24 || p.x > 456) {
        return -1;
    }

    for (int i = 0; i < 3; ++i) {
        int y = 72 + i * 56;
        if (p.y >= y && p.y <= y + 46) {
            return i;
        }
    }

    return -1;
}
