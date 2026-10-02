#include "DiagnosticsScreen.h"

#include "SettingsScreen.h"
#include "../MaterialStyle.h"
#include "../ScreenManager.h"
#include "../assets/IconBitmaps.h"
#include "../../diagnostics/DiagnosticLog.h"
#include "../../network/WiFiManager.h"

namespace {
constexpr int LogX = 20;
constexpr int LogTopY = 72;
constexpr int LogWidth = 440;
constexpr int LogLineHeight = 17;
constexpr int LogLineCount = 11;

String formatUptime(uint32_t uptimeMs) {
    uint32_t totalSeconds = uptimeMs / 1000;
    char buffer[16];
    snprintf(buffer,
             sizeof(buffer),
             "%lu:%02lu:%02lu",
             static_cast<unsigned long>(totalSeconds / 3600),
             static_cast<unsigned long>((totalSeconds / 60) % 60),
             static_cast<unsigned long>(totalSeconds % 60));
    return buffer;
}
} // namespace

void DiagnosticsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    _drawnSubtitle = subtitleText();
    MaterialStyle::drawPageHeader(tft, Icons::SCAN, "Diagnostics", _drawnSubtitle);
    drawLog(tft);

    MaterialStyle::drawStandardButton(tft,
                                      182,
                                      MaterialStyle::BottomActionY,
                                      116,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "OK",
                                      MaterialStyle::ComponentState::Success);
    _lastRefreshAtMs = millis();
}

void DiagnosticsScreen::update() {
    if (millis() - _lastRefreshAtMs < RefreshIntervalMs) {
        return;
    }

    _lastRefreshAtMs = millis();
    if (subtitleText() != _drawnSubtitle || DiagnosticLog::getInstance().totalCount() != _drawnLogCount) {
        draw();
    }
}

void DiagnosticsScreen::handleTouch(TS_Point p) {
    if (isOkPressed(p)) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void DiagnosticsScreen::drawLog(TFT_eSPI& tft) {
    _drawnLogCount = DiagnosticLog::getInstance().totalCount();
    std::vector<DiagnosticLog::Entry> entries = DiagnosticLog::getInstance().entries();

    // Newest first, so the most relevant events are visible without scrolling.
    int line = 0;
    for (auto it = entries.rbegin(); it != entries.rend() && line < LogLineCount; ++it, ++line) {
        String text = formatUptime(it->uptimeMs) + "  " + it->message;
        MaterialStyle::drawText(tft,
                                MaterialStyle::truncateToWidth(tft, text, LogWidth, 2),
                                LogX,
                                LogTopY + (line * LogLineHeight),
                                MaterialStyle::TextRole::Body,
                                TL_DATUM);
    }
}

String DiagnosticsScreen::subtitleText() const {
    if (!WiFiManager::getInstance().isConnected()) {
        return "Wi-Fi offline; web details unavailable";
    }
    return "Full details at http://" + WiFiManager::getInstance().getIPAddress() + "/";
}

bool DiagnosticsScreen::isOkPressed(TS_Point p) const {
    return p.x >= 182 && p.x <= 298 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}
