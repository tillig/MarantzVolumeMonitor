#include "ReceiverIpScreen.h"
#include "ReceiverListScreen.h"
#include "ReceiverStatusScreen.h"
#include "../ScreenManager.h"
#include "../../network/ReceiverDiscovery.h"

ReceiverIpScreen::ReceiverIpScreen(ScreenReturnTarget returnTarget)
    : _returnTarget(returnTarget) {}

void ReceiverIpScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextDatum(TL_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString("Manual Receiver IP", 20, 14, 4);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("IPv4 only", 20, 44, 2);

    drawInput(tft);
    drawKeypad(tft);
}

void ReceiverIpScreen::update() {}

void ReceiverIpScreen::handleTouch(TS_Point p) {
    const char* labels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "<"};

    for (int i = 0; i < 12; ++i) {
        int col = i % 3;
        int row = i / 3;
        int x = 84 + col * 104;
        int y = 96 + row * 44;
        if (p.x >= x && p.x <= x + 88 && p.y >= y && p.y <= y + 38) {
            String label = labels[i];
            if (label == "<") {
                if (_ipAddress.length() > 0) {
                    _ipAddress.remove(_ipAddress.length() - 1);
                }
            } else if (_ipAddress.length() < 15) {
                _ipAddress += label;
            }
            _errorMessage = "";
            drawInput(DisplayManager::getInstance().getTft());
            return;
        }
    }

    if (p.y >= 280 && p.y <= 318) {
        if (p.x >= 44 && p.x <= 180) {
            ScreenManager::getInstance().setScreen(new ReceiverListScreen(_returnTarget));
        } else if (p.x >= 300 && p.x <= 436) {
            submit();
        }
    }
}

void ReceiverIpScreen::drawInput(TFT_eSPI& tft) {
    tft.fillRect(20, 66, 440, 28, DisplayManager::COLOR_BACKGROUND);
    tft.fillRoundRect(20, 66, 440, 28, 4, DisplayManager::COLOR_PANEL);
    tft.drawRoundRect(20, 66, 440, 28, 4, DisplayManager::COLOR_BAR_BG);
    tft.setTextDatum(ML_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    String display = _ipAddress + "_";
    tft.drawString(display, 32, 80, 4);

    tft.fillRect(20, 266, 440, 12, DisplayManager::COLOR_BACKGROUND);
    if (_errorMessage.length() > 0) {
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(TFT_RED);
        tft.drawString(_errorMessage, 240, 272, 1);
    }
}

void ReceiverIpScreen::drawKeypad(TFT_eSPI& tft) {
    const char* labels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "<"};
    for (int i = 0; i < 12; ++i) {
        int col = i % 3;
        int row = i / 3;
        int x = 84 + col * 104;
        int y = 96 + row * 44;
        tft.fillRoundRect(x, y, 88, 38, 6, DisplayManager::COLOR_PANEL);
        tft.drawRoundRect(x, y, 88, 38, 6, DisplayManager::COLOR_BAR_BG);
        tft.setTextDatum(MC_DATUM);
        tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
        tft.drawString(labels[i], x + 44, y + 19, 4);
    }

    tft.fillRoundRect(44, 280, 136, 36, 6, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(300, 280, 136, 36, 6, TFT_DARKGREEN);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString("Cancel", 112, 298, 2);
    tft.drawString("Verify", 368, 298, 2);
}

void ReceiverIpScreen::submit() {
    if (!ReceiverDiscovery::isValidIpv4(_ipAddress)) {
        _errorMessage = "Enter a valid IPv4 address";
        drawInput(DisplayManager::getInstance().getTft());
        return;
    }

    ReceiverCandidate candidate("Manual Receiver", _ipAddress, ReceiverSource::Manual, millis());
    ScreenManager::getInstance().setScreen(new ReceiverStatusScreen(candidate, _returnTarget));
}
