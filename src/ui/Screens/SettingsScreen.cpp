#include "SettingsScreen.h"
#include "HomeScreen.h"
#include "../ScreenManager.h"

void SettingsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Settings", 20, 20, 4);

    tft.drawFastHLine(20, 50, 440, DisplayManager::COLOR_TEXT_DIMMED);

    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.drawString("Wi-Fi Setup ->", 20, 80, 4);
    tft.drawString("Receiver Setup ->", 20, 140, 4);
    tft.drawString("Display Settings ->", 20, 200, 4);

    // Back button [ X ]
    tft.setTextDatum(TR_DATUM);
    tft.drawString("X", 460, 20, 4);
}

void SettingsScreen::update() {
    // Nothing to poll here usually
}

void SettingsScreen::handleTouch(TS_Point p) {
    // Close settings (return to home) if X is pressed
    if (p.x > 400 && p.y < 60) {
        ScreenManager::getInstance().setScreen(new HomeScreen());
    }
}
