#include "DisplayManager.h"

#include <Arduino.h>

void DisplayManager::begin() {
    setBacklightEnabled(true, "DisplayManager::begin");
    _tft.init();
    _tft.setRotation(1); // Landscape
    _tft.fillScreen(COLOR_BACKGROUND);
}

void DisplayManager::setBrightness(uint8_t brightness) {
    // ESP32 supports PWM on most pins for backlight control
    // but if it is, we would use analogWrite here.
    // Assuming for now it's just on or we'll add PWM later.
}

void DisplayManager::setBacklightEnabled(bool enabled, const char* reason) {
    bool previous = _backlightEnabled;
    _backlightEnabled = enabled;

    if (previous != enabled || reason != nullptr) {
        Serial.print("BACKLIGHT ");
        Serial.print(previous ? "ON" : "OFF");
        Serial.print(" -> ");
        Serial.print(enabled ? "ON" : "OFF");
        if (reason != nullptr) {
            Serial.print(" reason=");
            Serial.print(reason);
        }
        Serial.print(" millis=");
        Serial.println(millis());
    }

#ifdef BACKLIGHT_CONTROL_PIN
    pinMode(BACKLIGHT_CONTROL_PIN, OUTPUT);
    digitalWrite(BACKLIGHT_CONTROL_PIN, enabled ? HIGH : LOW);
#endif
}
