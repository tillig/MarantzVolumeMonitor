#include "TouchManager.h"

void TouchManager::begin() {
    _ts.begin();
    _ts.setRotation(1);
}

bool TouchManager::isTouched() {
    if (_ts.touched()) {
        TS_Point p = _ts.getPoint();
        Serial.print("Touch Data: x=");
        Serial.print(p.x);
        Serial.print(", y=");
        Serial.print(p.y);
        Serial.print(", z=");
        Serial.println(p.z);
        return true;
    }
    return false;
}

TS_Point TouchManager::getPoint() {
    TS_Point p = _ts.getPoint();

    // Map raw coordinates to screen pixels
    // Using the user-provided corners:
    // Left side raw X is ~3750, Right side raw X is ~350 (Inverted)
    // Top side raw Y is ~3750, Bottom side raw Y is ~400 (Inverted)
    int16_t x = map(p.x, TS_MAX_X, TS_MIN_X, 0, SCREEN_WIDTH - 1);
    int16_t y = map(p.y, TS_MAX_Y, TS_MIN_Y, 0, SCREEN_HEIGHT - 1);

    // Constrain to screen bounds
    x = constrain(x, 0, SCREEN_WIDTH - 1);
    y = constrain(y, 0, SCREEN_HEIGHT - 1);

    return TS_Point(x, y, p.z);
}
