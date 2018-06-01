#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <TFT_eSPI.h>

class DisplayManager {
public:
    static DisplayManager& getInstance() {
        static DisplayManager instance;
        return instance;
    }

    void begin();
    void setBrightness(uint8_t brightness);
    TFT_eSPI& getTft() { return _tft; }

    // Constants for colors from UIPlan.md
    static const uint16_t COLOR_BACKGROUND = 0x0841; // #101010
    static const uint16_t COLOR_PANEL = 0x1082;      // #181818
    static const uint16_t COLOR_TEXT_PRIMARY = TFT_WHITE;
    static const uint16_t COLOR_TEXT_SECONDARY = 0xA514; // #A0A0A0
    static const uint16_t COLOR_TEXT_DIMMED = 0x632C;    // #606060
    static const uint16_t COLOR_ACCENT = 0x3D7F;         // #3DAEFF
    static const uint16_t COLOR_BAR_BG = 0x3186;         // #303030

private:
    DisplayManager() : _tft(TFT_eSPI()) {}
    TFT_eSPI _tft;
};

#endif
