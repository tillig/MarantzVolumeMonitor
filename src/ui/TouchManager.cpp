#include "TouchManager.h"
#include <Arduino.h>
#include <math.h>

void TouchManager::begin() {
    _ts.begin();
    _ts.setRotation(1);
}

bool TouchManager::isTouched() {
    if (_ts.touched()) {
        _lastPoint = _ts.getPoint();
        return true;
    }
    return false;
}

TS_Point TouchManager::getRawPoint() {
    return _lastPoint;
}

TS_Point TouchManager::getPoint() {
    // Apply the measured affine transform instead of per-axis min/max mapping.
    float mappedX = (X_FROM_RAW_X * _lastPoint.x) + (X_FROM_RAW_Y * _lastPoint.y) + X_OFFSET;
    float mappedY = (Y_FROM_RAW_X * _lastPoint.x) + (Y_FROM_RAW_Y * _lastPoint.y) + Y_OFFSET;

    int16_t x = static_cast<int16_t>(lroundf(mappedX));
    int16_t y = static_cast<int16_t>(lroundf(mappedY));

    x = constrain(x, 0, SCREEN_WIDTH - 1);
    y = constrain(y, 0, SCREEN_HEIGHT - 1);

    return TS_Point(x, y, _lastPoint.z);
}
