#include "TouchManager.h"
#include <Arduino.h>
#include <math.h>

void TouchManager::begin() {
    _ts.begin();
    _ts.setRotation(1);
    _touchCandidateActive = false;
}

bool TouchManager::isTouched() {
    if (!_ts.touched()) {
        _touchCandidateActive = false;
        return false;
    }

    TS_Point point = _ts.getPoint();
    uint32_t now = millis();

    if (!_touchCandidateActive) {
        _touchCandidateActive = true;
        _touchCandidateStartedMs = now;
        _candidatePoint = point;
        return false;
    }

    _candidatePoint = point;
    if (now - _touchCandidateStartedMs < TOUCH_CONFIRM_MS) {
        return false;
    }

    _lastPoint = _candidatePoint;
    return true;
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
