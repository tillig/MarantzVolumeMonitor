#include "NetworkListScreen.h"
#include "KeyboardScreen.h"
#include "../ScreenManager.h"
#include "../DisplayManager.h"

void NetworkListScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("Select Wi-Fi Network", 240, 10, 4);

    if (!_isScanning && _networks.empty()) {
        tft.drawString("Scanning...", 240, 140, 4);
        WiFiManager::getInstance().startScan();
        _isScanning = true;
    } else {
        drawList();
    }

    // Manual Entry Button
    tft.fillRoundRect(20, 275, 200, 35, 17, DisplayManager::COLOR_BAR_BG);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Manual Entry", 120, 292, 2);

    // Scan/Refresh Button
    tft.fillRoundRect(260, 275, 200, 35, 17, DisplayManager::COLOR_BAR_BG);
    tft.drawString("Rescan", 360, 292, 2);
}

void NetworkListScreen::update() {
    if (_isScanning) {
        int16_t status = WiFiManager::getInstance().getScanStatus();
        if (status >= 0) {
            _networks = WiFiManager::getInstance().getScanResults();
            _isScanning = false;
            draw();
        }
    }
}

void NetworkListScreen::handleTouch(TS_Point p) {
    // 1. Bottom Buttons (Manual / Rescan) — drawn at y=275, height=35
    if (p.y > 270) {
        if (p.x < 240) {
            ScreenManager::getInstance().setScreen(new KeyboardScreen(""));
        } else {
            _networks.clear();
            _isScanning = false;
            _scrollOffset = 0;
            draw();
        }
        return;
    }

    // 2. Pagination Controls — drawn around y=230–270
    if (p.y > 230 && p.y < 270) {
        if (p.x < 120 && _scrollOffset > 0) {
            _scrollOffset -= 5;
            draw();
        } else if (p.x > 360 && (_scrollOffset + 5) < _networks.size()) {
            _scrollOffset += 5;
            draw();
        }
        return;
    }

    // 3. Network list items — drawn at y = 50 + i*36, height=32
    if (p.y > 50 && p.y < 230) {
        int index = _scrollOffset + ((p.y - 50) / 36);
        if (index < _networks.size()) {
            ScreenManager::getInstance().setScreen(new KeyboardScreen(_networks[index].ssid));
        }
    }
}

void NetworkListScreen::drawList() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    int itemsToShow = 5;
    for (int i = 0; i < itemsToShow; ++i) {
        int index = _scrollOffset + i;
        int y = 50 + (i * 36);

        if (index < _networks.size()) {
            tft.fillRoundRect(10, y, 460, 32, 6, DisplayManager::COLOR_BAR_BG);
            tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
            tft.setTextDatum(ML_DATUM);

            String ssid = _networks[index].ssid;
            if (ssid.length() > 25) ssid = ssid.substring(0, 22) + "...";
            tft.drawString(ssid, 25, y + 16, 2);

            // RSSI indicator
            int32_t rssi = _networks[index].rssi;
            uint16_t color = TFT_GREEN;
            if (rssi < -80) color = TFT_RED;
            else if (rssi < -70) color = TFT_YELLOW;
            tft.fillCircle(445, y + 16, 6, color);
        }
    }

    // Draw Pagination Status
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(MC_DATUM);
    String pageInfo = "Page " + String((_scrollOffset / 5) + 1) + " of " + String((_networks.size() + 4) / 5);
    tft.drawString(pageInfo, 240, 245, 1);

    if (_scrollOffset > 0) tft.drawString("< PREV", 60, 245, 2);
    if ((_scrollOffset + 5) < _networks.size()) tft.drawString("NEXT >", 420, 245, 2);
}
