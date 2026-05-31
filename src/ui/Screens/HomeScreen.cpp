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

    // Default placeholder status for prototyping
    _lastStatus.volume = -45.0;
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

    // 1. Arc Gauge around the center
    drawVolumeArc(240, 110, 90, _lastStatus.volume);

    // 2. Large Volume Number (Font 8)
    tft.setTextColor(TFT_WHITE, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0", 240, 110, 8);
    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.drawString("dB", 240, 180, 4);

    // 3. Labels below (Classic placement, no prefixes, Font 4)
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("Blu-ray", 30, 210, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("Dolby TrueHD", 450, 210, 4);

    // 4. Tiles (Modern pills with gray background)
    String families[] = {"Dolby", "DTS", "PCM", "Other"};
    for(int i=0; i<4; i++) {
        int tx = 20 + (i*115);
        int ty = 250;
        tft.fillRoundRect(tx, ty, 105, 45, 22, DisplayManager::COLOR_BAR_BG);
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
        tft.setTextDatum(MC_DATUM);
        tft.drawString(families[i], tx + 52, ty + 22, 2);
    }

    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Layout: UNIFIED (Tap Top to Cycle)", 240, 315, 2);
}

void HomeScreen::drawVolumeArc(int x, int y, int r, float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // Map volume (-80 to 18) to arc progress (0 to 270 degrees)
    float percent = (volume + 80) / 98.0;
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;

    int startAngle = 225; // Bottom leftish
    int sweepAngle = (int)(percent * 270);

    // Draw Background Shadow Arc
    tft.drawArc(x, y, r, r-15, 225, 495, DisplayManager::COLOR_BAR_BG, DisplayManager::COLOR_BACKGROUND);

    // Draw Value Arc with Color Gradient (simplified for now: Green -> Yellow -> Red)
    uint16_t arcColor = TFT_GREEN;
    if (percent > 0.5) arcColor = TFT_YELLOW;
    if (percent > 0.8) arcColor = TFT_RED;

    tft.drawArc(x, y, r, r-15, 225, 225 + sweepAngle, arcColor, DisplayManager::COLOR_BACKGROUND);
}

void HomeScreen::drawLayoutClassic() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // Large Volume
    tft.setTextColor(_textColors[_colorIndex], DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0 dB", 240, 80, 7);

    // Volume Bar
    tft.fillRoundRect(72, 140, 336, 24, 4, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(72, 140, 200, 24, 4, DisplayManager::COLOR_ACCENT);

    // Labels
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("SRC: Blu-ray", 20, 200, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("MODE: Dolby TrueHD", 460, 200, 4);

    // Tiles (boxes)
    for(int i=0; i<4; i++) {
        tft.drawRoundRect(20 + (i*115), 240, 100, 50, 4, DisplayManager::COLOR_BAR_BG);
    }

    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Layout: CLASSIC (Tap Top to Cycle)", 240, 310, 2);
}

void HomeScreen::drawLayoutModern() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // Centered Bar Focus
    tft.fillRoundRect(24, 150, 432, 40, 6, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(24, 150, 300, 40, 6, DisplayManager::COLOR_ACCENT);

    // Volume floating above bar
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("-45.0 dB", 240, 140, 7);

    // Labels at very top
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Blu-ray", 20, 10, 4);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("Dolby TrueHD", 460, 10, 4);

    // Tiles (pills)
    for(int i=0; i<4; i++) {
        tft.fillRoundRect(20 + (i*115), 230, 100, 40, 20, DisplayManager::COLOR_BAR_BG);
    }

    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Layout: MODERN (Tap Top to Cycle)", 240, 310, 2);
}

void HomeScreen::drawLayoutMinimal() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // Massive Volume Number
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0", 240, 160, 8); // Largest font

    // Thin bar at bottom
    tft.fillRect(0, 310, 480, 10, DisplayManager::COLOR_BAR_BG);
    tft.fillRect(0, 310, 320, 10, DisplayManager::COLOR_ACCENT);

    // Tiny labels
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("BLU-RAY", 10, 10, 2);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("DOLBY TRUEHD", 470, 10, 2);

    tft.setTextDatum(BC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.drawString("Layout: MINIMAL (Tap Top to Cycle)", 240, 280, 2);
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
