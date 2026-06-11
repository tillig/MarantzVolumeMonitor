#include "DisplayManager.h"

#include <Arduino.h>

void DisplayManager::begin() {
    setBacklightEnabled(true);
    _tft.init();
    _tft.setRotation(1); // Landscape
    _tft.fillScreen(COLOR_BACKGROUND);
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // ESP32 supports PWM on most pins for backlight control
    // but if it is, we would use analogWrite here.
    // Assuming for now it's just on or we'll add PWM later.
}

void DisplayManager::setBacklightEnabled(bool enabled) {
    _backlightEnabled = enabled;

#ifdef BACKLIGHT_CONTROL_PIN
    pinMode(BACKLIGHT_CONTROL_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_CONTROL_PIN, enabled ? HIGH : LOW);
#endif
}
