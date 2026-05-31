#include "DisplayManager.h"

void DisplayManager::begin() {
    _tft.init();
    _tft.setRotation(1); // Landscape
    _tft.fillScreen(COLOR_BACKGROUND);
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // ESP32 supports PWM on most pins for backlight control
    // but if it is, we would use analogWrite here.
    // Assuming for now it's just on or we'll add PWM later.
}
