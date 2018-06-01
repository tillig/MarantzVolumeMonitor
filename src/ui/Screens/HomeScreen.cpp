#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"

void HomeScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
    
    drawVolume(_lastStatus.volume);
    drawSource(_lastStatus.input);
    drawMode(_lastStatus.mode);
    drawTiles(_lastStatus.mode);
    drawSettingsButton();
}

void HomeScreen::update() {
    MarantzStatus status = MarantzClient::getInstance().getStatus();
    if (status.isValid) {
        if (status.volume != _lastStatus.volume) {
            drawVolume(status.volume);
        }
        if (status.input != _lastStatus.input) {
            drawSource(status.input);
        }
        if (status.mode != _lastStatus.mode) {
            drawMode(status.mode);
            drawTiles(status.mode);
        }
        _lastStatus = status;
    }
}

void HomeScreen::handleTouch(TS_Point p) {
    // Check if settings button was pressed (bottom right gear)
    if (p.x > 400 && p.y > 240) { 
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void HomeScreen::drawVolume(float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    
    char volStr[10];
    dtostrf(volume, 4, 1, volStr);
    String displayVol = String(volStr) + " dB";
    
    tft.drawString(displayVol, 240, 80, 7); // Using font 7 for large volume
    
    // Draw Volume Bar
    int barWidth = 336; // 70% of 480
    int barHeight = 24;
    int barX = (480 - barWidth) / 2;
    int barY = 140;
    
    tft.fillRoundRect(barX, barY, barWidth, barHeight, 4, DisplayManager::COLOR_BAR_BG);
    
    // Calculate fill width (mapping -80 to 18 dB to 0 to 100%)
    // But Marantz usually shows 0 to 98 or -80 to 18.
    float percent = (volume + 80) / 98.0; 
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    
    tft.fillRoundRect(barX, barY, (int)(barWidth * percent), barHeight, 4, DisplayManager::COLOR_ACCENT);
}

void HomeScreen::drawSource(const String& source) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("SRC: " + source, 20, 200, 4);
}

void HomeScreen::drawMode(const String& mode) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("MODE: " + mode, 460, 200, 4);
}

void HomeScreen::drawTiles(const String& mode) {
    // Implementation for audio tiles (Dolby, DTS, PCM, Other)
}

void HomeScreen::drawSettingsButton() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(BR_DATUM);
    tft.drawString("SET", 470, 310, 4); // Simplified gear icon for now
}
