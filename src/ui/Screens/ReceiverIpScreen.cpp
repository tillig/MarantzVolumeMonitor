#include "ReceiverIpScreen.h"
#include "ReceiverListScreen.h"
#include "ReceiverStatusScreen.h"
#include "../IconRenderer.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"
#include "../../network/ReceiverDiscovery.h"

ReceiverIpScreen::ReceiverIpScreen(ScreenReturnTarget returnTarget) : _returnTarget(returnTarget) {}

void ReceiverIpScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawPageHeader(tft, Icons::RECEIVER, "Manual Receiver IP", "IPv4 only");

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

    if (p.y >= MaterialStyle::BottomActionY && p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        if (p.x >= 44 && p.x <= 180) {
            ScreenManager::getInstance().setScreen(new ReceiverListScreen(_returnTarget));
        } else if (p.x >= 300 && p.x <= 436) {
            submit();
        }
    }
}

void ReceiverIpScreen::drawInput(TFT_eSPI& tft) {
    tft.fillRect(20, 66, 440, 28, DisplayManager::COLOR_BACKGROUND);
    String display = _ipAddress + "_";
    MaterialStyle::drawInputField(tft, 20, 66, 440, 28, display, MaterialStyle::TextRole::SectionLabel);

    tft.fillRect(20, 266, 440, 12, DisplayManager::COLOR_BACKGROUND);
    if (_errorMessage.length() > 0) {
        MaterialStyle::drawText(tft,
                                _errorMessage,
                                240,
                                272,
                                MaterialStyle::TextRole::CompactMetadata,
                                MC_DATUM,
                                MaterialStyle::ComponentState::Error);
    }
}

void ReceiverIpScreen::drawKeypad(TFT_eSPI& tft) {
    const char* labels[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", ".", "0", "<"};
    for (int i = 0; i < 12; ++i) {
        int col = i % 3;
        int row = i / 3;
        int x = 84 + col * 104;
        int y = 96 + row * 44;
        if (String(labels[i]) == "<") {
            MaterialStyle::drawKeyboardIconButton(tft, x, y, 88, 38, Icons::KEYBOARD_BACKSPACE);
        } else {
            MaterialStyle::drawKeyboardTextButton(tft, x, y, 88, 38, labels[i]);
        }
    }

    MaterialStyle::drawStandardButton(tft,
                                      44,
                                      MaterialStyle::BottomActionY,
                                      136,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_CANCEL,
                                      "Cancel",
                                      MaterialStyle::ComponentState::Error);
    MaterialStyle::drawStandardButton(tft,
                                      300,
                                      MaterialStyle::BottomActionY,
                                      136,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "Verify",
                                      MaterialStyle::ComponentState::Success);
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
