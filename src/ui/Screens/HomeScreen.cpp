#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"

HomeScreen::HomeScreen() {
    _colorIndex = 0;
    _textColors[0] = TFT_WHITE;
    _textColors[1] = TFT_GREEN;
    _textColors[2] = TFT_BLUE;
    _textColors[3] = TFT_RED;
    _textColors[4] = TFT_YELLOW;
    _textColors[5] = TFT_MAGENTA;
    _textColors[6] = TFT_CYAN;
}

void HomeScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    if (_lastStatus.isValid) {
        drawVolume(_lastStatus.volume);
        drawSource(_lastStatus.input);
        drawMode(_lastStatus.mode);
        drawTiles(_lastStatus.mode);
        drawSettingsButton();
    } else {
        drawSpikeText();
    }
}

void HomeScreen::update() {
    // If we aren't connected to a receiver, just keep showing spike text
    if (_lastStatus.isValid) {
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
}

void HomeScreen::handleTouch(TS_Point p) {
    if (!_lastStatus.isValid) {
        // Cycle colors if in spike mode
        _colorIndex = (_colorIndex + 1) % 7;
        drawSpikeText();
        Serial.print("HomeScreen Color Changed to index: ");
        Serial.println(_colorIndex);
        return;
    }

    // Check if settings button was pressed (bottom right gear)
    if (p.x > 400 && p.y > 240) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void HomeScreen::drawSpikeText() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(_textColors[_colorIndex], DisplayManager::COLOR_BACKGROUND);
    tft.drawString("Hello World", 240, 160, 4);
    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Architecture Ready", 240, 310, 2);
}

void HomeScreen::drawVolume(float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);

    char volStr[10];
    dtostrf(volume, 4, 1, volStr);
    String displayVol = String(volStr) + " dB";

    tft.drawString(displayVol, 240, 80, 7);

    int barWidth = 336;
    int barHeight = 24;
    int barX = (480 - barWidth) / 2;
    int barY = 140;

    tft.fillRoundRect(barX, barY, barWidth, barHeight, 4, DisplayManager::COLOR_BAR_BG);

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
}

void HomeScreen::drawSettingsButton() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(BR_DATUM);
    tft.drawString("SET", 470, 310, 4);
}
