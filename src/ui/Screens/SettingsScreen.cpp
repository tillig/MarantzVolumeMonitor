#include "SettingsScreen.h"
#include "CalibrationScreen.h"
#include "HomeScreen.h"
#include "NetworkListScreen.h"
#include "ReceiverListScreen.h"
#include "../ScreenManager.h"

void SettingsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Settings", 20, 20, 4);

    tft.drawFastHLine(20, 50, 440, DisplayManager::COLOR_TEXT_DIMMED);

    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.drawString("Touch Calibration ->", 20, 80, 4);
    tft.drawString("Wi-Fi Setup ->", 20, 140, 4);
    tft.drawString("Receiver Setup ->", 20, 200, 4);

    tft.fillRoundRect(360, 270, 96, 38, 8, DisplayManager::COLOR_PANEL);
    tft.drawRoundRect(360, 270, 96, 38, 8, DisplayManager::COLOR_BAR_BG);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("OK", 408, 289, 2);
}

void SettingsScreen::update() {
    // Nothing to poll here usually
}

void SettingsScreen::handleTouch(TS_Point p) {
    if (p.x >= 360 && p.x <= 456 && p.y >= 270 && p.y <= 308) {
        ScreenManager::getInstance().setScreen(new HomeScreen());
    } else if (p.y >= 132 && p.y <= 176) {
        ScreenManager::getInstance().setScreen(new NetworkListScreen(ScreenReturnTarget::Settings));
    } else if (p.y >= 192 && p.y <= 236) {
        ScreenManager::getInstance().setScreen(new ReceiverListScreen(ScreenReturnTarget::Settings));
    } else if (p.y >= 72 && p.y <= 116) {
        ScreenManager::getInstance().setScreen(new CalibrationScreen(ScreenReturnTarget::Settings));
    }
}
