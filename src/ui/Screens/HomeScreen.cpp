#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "../ScreenManager.h"

HomeScreen::HomeScreen() {
    _colorIndex = 0;
    _currentLayout = Layout::Unified;
    _textColors[0] = TFT_WHITE;
    _textColors[1] = TFT_GREEN;
    _textColors[2] = TFT_BLUE;
    _textColors[3] = TFT_RED;
    _textColors[4] = TFT_YELLOW;
    _textColors[5] = TFT_MAGENTA;
    _textColors[6] = TFT_CYAN;

    // Default placeholder status for prototyping (Set to 50 for 0-100 scale)
    _lastStatus.volume = 50.0;
    _lastStatus.input = "Blu-ray";
    _lastStatus.mode = "Dolby TrueHD";
    _lastStatus.isValid = false;
}

void HomeScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    switch (_currentLayout) {
        case Layout::Classic: drawLayoutClassic(); break;
        case Layout::Modern:  drawLayoutModern();  break;
        case Layout::Minimal: drawLayoutMinimal(); break;
        case Layout::Unified: drawLayoutUnified(); break;
    }
}

void HomeScreen::update() {
}

void HomeScreen::handleTouch(TS_Point p) {
    if (!_lastStatus.isValid) {
        // Cycle layouts on top half, colors on bottom half
        if (p.y < 160) {
            _currentLayout = (Layout)(((int)_currentLayout + 1) % 4);
            Serial.print("HomeScreen Layout Changed to: ");
            Serial.println((int)_currentLayout);
        } else {
            _colorIndex = (_colorIndex + 1) % 7;
            Serial.print("HomeScreen Color Changed to index: ");
            Serial.println(_colorIndex);
        }
        draw();
        return;
    }
}

void HomeScreen::drawLayoutUnified() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // 1. Arc Gauge (Center 240, 140, Radius 125, Thickness 16)
    tft.drawArc(240, 140, 125, 109, 60, 300, DisplayManager::COLOR_BAR_BG, DisplayManager::COLOR_BACKGROUND);
    drawVolumeArc(240, 140, 125, _lastStatus.volume);

    // 2. Large Volume Number (Font 8)
    // Moved up 5px to Y=125 for final visual centering
    tft.setTextColor(TFT_WHITE, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawNumber((int)_lastStatus.volume, 240, 125, 8);

    // "VOLUME" label moved up to Y=185
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("VOLUME", 240, 185, 2);

    // 3. Labels (Moved to Y=255 to clear the gauge)
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("Blu-ray", 40, 255, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("Dolby TrueHD", 440, 255, 4);

    // 4. Tiles (10px margin from bottom: 320 - 32 - 10 = 278)
    String families[] = {"Dolby", "DTS", "PCM", "Other"};
    for(int i=0; i<4; i++) {
        int tx = 25 + (i*112);
        int ty = 278;
        tft.fillRoundRect(tx, ty, 102, 32, 16, DisplayManager::COLOR_BAR_BG);
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(families[i], tx + 51, ty + 16, 2);
    }
}

void HomeScreen::drawVolumeArc(int x, int y, int r, float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    float percent = volume / 100.0;
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;

    int startAngle = 60;
    int maxSweep = 240;
    int currentSweep = (int)(percent * maxSweep);
    int endAngle = startAngle + currentSweep;

    // Draw the Gradient Fill
    for (int i = 0; i < currentSweep; i += 8) {
        float p = (float)i / maxSweep;
        uint8_t red, green;
        if (p < 0.5) {
            red = (uint8_t)(p * 2 * 255);
            green = 255;
        } else {
            red = 255;
            green = (uint8_t)((1.0 - (p - 0.5) * 2) * 255);
        }
        uint16_t color = tft.color565(red, green, 0);

        int segStart = startAngle + i;
        int segEnd = segStart + 8;
        if (segEnd > endAngle) segEnd = endAngle;
        tft.drawArc(x, y, r, r-16, segStart, segEnd, color, DisplayManager::COLOR_BACKGROUND);
    }

    // Add Rounded Ends (Pills)
    // To align with Arc (0=Bottom), Math Angle = Arc Angle + 90
    float r_mid = r - 8;

    // 1. Start cap (8:00 - Arc 60)
    float a1 = (60 + 90) * PI / 180.0;
    tft.fillCircle(x + r_mid * cos(a1), y + r_mid * sin(a1), 8, tft.color565(0, 255, 0));

    // 2. Track End cap (4:00 - Arc 300)
    float a3 = (300 + 90) * PI / 180.0;
    tft.fillCircle(x + r_mid * cos(a3), y + r_mid * sin(a3), 8, DisplayManager::COLOR_BAR_BG);

    // 3. Current volume cap (Gradient color)
    if (currentSweep > 0) {
        float a2 = (endAngle + 90) * PI / 180.0;
        float p = (float)currentSweep / maxSweep;
        uint8_t red, green;
        if (p < 0.5) { red = (uint8_t)(p * 2 * 255); green = 255; }
        else { red = 255; green = (uint8_t)((1.0 - (p - 0.5) * 2) * 255); }
        tft.fillCircle(x + r_mid * cos(a2), y + r_mid * sin(a2), 8, tft.color565(red, green, 0));
    }
}

void HomeScreen::drawLayoutClassic() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(_textColors[_colorIndex], DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0 dB", 240, 80, 7);
    tft.fillRoundRect(72, 140, 336, 24, 4, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(72, 140, 200, 24, 4, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("SRC: Blu-ray", 20, 200, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("MODE: Dolby TrueHD", 460, 200, 4);
    for(int i=0; i<4; i++) {
        tft.drawRoundRect(20 + (i*115), 240, 100, 50, 4, DisplayManager::COLOR_BAR_BG);
    }
}

void HomeScreen::drawLayoutModern() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillRoundRect(24, 150, 432, 40, 6, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(24, 150, 300, 40, 6, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("-45.0 dB", 240, 140, 7);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Blu-ray", 20, 10, 4);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("Dolby TrueHD", 460, 10, 4);
    for(int i=0; i<4; i++) {
        tft.fillRoundRect(20 + (i*115), 230, 100, 40, 20, DisplayManager::COLOR_BAR_BG);
    }
}

void HomeScreen::drawLayoutMinimal() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0", 240, 160, 8);
    tft.fillRect(0, 310, 480, 10, DisplayManager::COLOR_BAR_BG);
    tft.fillRect(0, 310, 320, 10, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("BLU-RAY", 10, 10, 2);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("DOLBY TRUEHD", 470, 10, 2);
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
