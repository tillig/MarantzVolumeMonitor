#include "DisplayManager.h"

void DisplayManager::begin() {
    _tft.init();
    _tft.setRotation(1); // Landscape
    _tft.fillScreen(COLOR_BACKGROUND);
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // NodeMCU Mini doesn't always have a backlight pin wired to PWM by default
    // but if it is, we would use analogWrite here.
    // Assuming for now it's just on or we'll add PWM later.
}
