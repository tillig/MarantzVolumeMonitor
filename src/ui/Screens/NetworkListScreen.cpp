#include "NetworkListScreen.h"
#include "KeyboardScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"
#include "../DisplayManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"
#include "../../storage/ConfigStore.h"

NetworkListScreen::NetworkListScreen(ScreenReturnTarget returnTarget)
    : _returnTarget(returnTarget) {
    DeviceConfig config;
    ConfigStore::getInstance().loadConfig(config);
    _canCancel = _returnTarget == ScreenReturnTarget::Settings && config.wifiSsid.length() > 0;
}

void NetworkListScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    if (!_isScanning && !_hasScanned) {
        WiFiManager::getInstance().startScan();
        _isScanning = true;
        _progressFrame = 0;
        _lastProgressAtMs = millis();
    }

    MaterialStyle::drawPageHeader(tft, Icons::WIFI, "Wi-Fi Setup", "Select a Wi-Fi network");

    if (_isScanning) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Loading,
                                       "Scanning for networks...",
                                       "", Icons::SCAN, _progressFrame);
    } else if (_networks.empty()) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Empty, "No networks found",
                                       "Use Rescan or enter a network manually.", Icons::WARNING);
    } else {
        drawList();
    }

    drawActions(tft);
}

void NetworkListScreen::update() {
    if (_isScanning) {
        int16_t status = WiFiManager::getInstance().getScanStatus();
        if (status >= 0) {
            _networks = WiFiManager::getInstance().getScanResults();
            _isScanning = false;
            _hasScanned = true;
            draw();
        } else {
            uint32_t now = millis();
            if (now - _lastProgressAtMs >= MaterialStyle::ProgressFrameMs) {
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

void NetworkListScreen::handleTouch(TS_Point p) {
    // 1. Bottom Buttons (Manual / Rescan)
    if (p.y >= MaterialStyle::BottomActionY &&
        p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        if (_canCancel) {
            if (p.x >= 20 && p.x <= 150) {
                ScreenManager::getInstance().setScreen(new KeyboardScreen("", _returnTarget));
            } else if (p.x >= 175 && p.x <= 305) {
                _networks.clear();
                _hasScanned = false;
                _isScanning = false;
                _scrollOffset = 0;
                draw();
            } else if (p.x >= 330 && p.x <= 460) {
                ScreenManager::getInstance().setScreen(new SettingsScreen());
            }
        } else {
            if (p.x < 240) {
                ScreenManager::getInstance().setScreen(new KeyboardScreen("", _returnTarget));
            } else {
                _networks.clear();
                _hasScanned = false;
                _isScanning = false;
                _scrollOffset = 0;
                draw();
            }
        }
        return;
    }

    // 2. Pagination Controls
    if (p.y >= 230 && p.y <= 266) {
        if (p.x < 120 && _scrollOffset > 0) {
            _scrollOffset -= 3;
            draw();
        } else if (p.x > 360 && (_scrollOffset + 3) < _networks.size()) {
            _scrollOffset += 3;
            draw();
        }
        return;
    }

    // 3. Network list items
    if (p.y >= MaterialStyle::SetupListTopY && p.y < 240) {
        int index = _scrollOffset + ((p.y - MaterialStyle::SetupListTopY) /
                                     (MaterialStyle::ListRowHeight + MaterialStyle::RowGap));
        if (index < _networks.size()) {
            ScreenManager::getInstance().setScreen(new KeyboardScreen(_networks[index].ssid, _returnTarget));
        }
    }
}

void NetworkListScreen::drawList() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    int itemsToShow = 3;
    for (int i = 0; i < itemsToShow; ++i) {
        int index = _scrollOffset + i;
        int y = MaterialStyle::SetupListTopY + (i * (MaterialStyle::ListRowHeight + MaterialStyle::RowGap));

        if (index < _networks.size()) {
            MaterialStyle::drawListRow(tft, {
                20, y, 440, MaterialStyle::ListRowHeight,
                nullptr,
                WiFiManager::signalLevelForRssi(_networks[index].rssi),
                _networks[index].ssid,
                "",
                String(index + 1),
                MaterialStyle::ComponentState::Normal
            });
        }
    }

    // Draw Pagination Status
    String pageInfo = "Page " + String((_scrollOffset / itemsToShow) + 1) + " of " +
                      String((_networks.size() + itemsToShow - 1) / itemsToShow);
    MaterialStyle::drawText(tft, pageInfo, 240, 246,
                            MaterialStyle::TextRole::CompactMetadata, MC_DATUM);

    if (_scrollOffset > 0) {
        MaterialStyle::drawTextButton(tft, 20, 230, 96, 32, "PREV");
    }
    if ((_scrollOffset + itemsToShow) < _networks.size()) {
        MaterialStyle::drawTextButton(tft, 364, 230, 96, 32, "NEXT");
    }
}

void NetworkListScreen::drawActions(TFT_eSPI& tft) {
    if (_canCancel) {
        const char* labels[] = {"Manual", "Rescan", "Cancel"};
        for (int i = 0; i < 3; ++i) {
            int x = 20 + i * 155;
            const Icons::IconBitmap* icon = i == 0 ? &Icons::MANUAL_ENTRY : i == 1 ? &Icons::RETRY : &Icons::KEYBOARD_CANCEL;
            MaterialStyle::ComponentState state = i == 2 ? MaterialStyle::ComponentState::Error
                                                         : MaterialStyle::ComponentState::Normal;
            MaterialStyle::drawStandardButton(tft, x, MaterialStyle::BottomActionY, 130,
                                              MaterialStyle::ButtonHeight, *icon, labels[i], state);
        }
        return;
    }

    MaterialStyle::drawStandardButton(tft, 20, MaterialStyle::BottomActionY, 200, MaterialStyle::ButtonHeight,
                                      Icons::MANUAL_ENTRY, "Manual Entry");
    MaterialStyle::drawStandardButton(tft, 260, MaterialStyle::BottomActionY, 200, MaterialStyle::ButtonHeight,
                                      Icons::RETRY, "Rescan");
}
