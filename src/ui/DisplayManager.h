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
    void setBacklightEnabled(bool enabled, const char* reason = nullptr);
    bool isBacklightEnabled() const { return _backlightEnabled; }
    TFT_eSPI& getTft() { return _tft; }

    // Constants for colors from UIPlan.md
    static const uint16_t COLOR_BACKGROUND = 0x0841; // #101010
    static const uint16_t COLOR_PANEL = 0x1082;      // #181818
    static const uint16_t COLOR_TEXT_PRIMARY = TFT_WHITE;
    static const uint16_t COLOR_TEXT_SECONDARY = 0xA514; // #A0A0A0
    static const uint16_t COLOR_TEXT_DIMMED = 0x632C;    // #606060
    static const uint16_t COLOR_ACCENT = 0x3D7F;         // #3DAEFF
    static const uint16_t COLOR_BAR_BG = 0x3186;         // #303030
    static const uint16_t COLOR_TEXT_DARK = TFT_BLACK;   // For active tiles
    static const uint16_t COLOR_ICON_ACTIVE = 0x368D;    // #35D06E
    static const uint16_t COLOR_ICON_INACTIVE = COLOR_BAR_BG;
    static const uint16_t COLOR_WARNING = TFT_YELLOW;
    static const uint16_t COLOR_ERROR = TFT_RED;

private:
    DisplayManager() : _tft(TFT_eSPI()) {}
    TFT_eSPI _tft;
    bool _backlightEnabled = true;
};

#endif
