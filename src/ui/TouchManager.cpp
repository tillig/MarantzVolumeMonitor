#include "TouchManager.h"

#include <Arduino.h>
#include <math.h>

void TouchManager::begin() {
    _ts.begin();
    _ts.setRotation(1);
    _touchCandidateActive = false;
    _touchStableSampleCount = 0;
    applyDefaultCalibration();
}

bool TouchManager::isTouched() {
    if (!_ts.touched()) {
        _touchCandidateActive = false;
        _touchStableSampleCount = 0;
        return false;
    }

    TS_Point point = _ts.getPoint();
    uint32_t now = millis();

    if (!_touchCandidateActive) {
        _touchCandidateActive = true;
        _touchCandidateStartedMs = now;
        _candidatePoint = point;
        _touchStableSampleCount = 1;
        return false;
    }

    if (!isStableSample(point, _candidatePoint)) {
        _touchCandidateStartedMs = now;
        _candidatePoint = point;
        _touchStableSampleCount = 1;
        return false;
    }

    _candidatePoint = point;
    if (_touchStableSampleCount < 255) {
        _touchStableSampleCount++;
    }

    if (_touchStableSampleCount < TOUCH_CONFIRM_SAMPLES) {
        return false;
    }

    if (now - _touchCandidateStartedMs < TOUCH_CONFIRM_MS) {
        return false;
    }

    _lastPoint = _candidatePoint;
    return true;
}

bool TouchManager::isTouchContactPresent() {
    return _ts.touched();
}

TS_Point TouchManager::getRawPoint() {
    return _lastPoint;
}

TS_Point TouchManager::getPoint() {
    return mapRawPoint(_lastPoint);
}

TS_Point TouchManager::mapRawPoint(const TS_Point& rawPoint) const {
    float mappedX =
        (_activeProfile.xFromRawX * rawPoint.x) + (_activeProfile.xFromRawY * rawPoint.y) + _activeProfile.xOffset;
    float mappedY =
        (_activeProfile.yFromRawX * rawPoint.x) + (_activeProfile.yFromRawY * rawPoint.y) + _activeProfile.yOffset;
    int16_t x = static_cast<int16_t>(lroundf(mappedX));
    int16_t y = static_cast<int16_t>(lroundf(mappedY));

    x = constrain(x, 0, SCREEN_WIDTH - 1);
    y = constrain(y, 0, SCREEN_HEIGHT - 1);

    return TS_Point(x, y, rawPoint.z);
}

bool TouchManager::applyCalibrationProfile(const CalibrationProfile& profile) {
    if (!isCalibrationProfileUsable(profile)) {
        return false;
    }

    _activeProfile = profile;
    return true;
}

void TouchManager::applyDefaultCalibration() {
    _activeProfile = defaultCalibrationProfile();
}

TouchManager::CalibrationProfile TouchManager::activeCalibrationProfile() const {
    return _activeProfile;
}

TouchManager::CalibrationProfile TouchManager::defaultCalibrationProfile() {
    CalibrationProfile profile;
    profile.version = 1;
    profile.xFromRawX = DEFAULT_X_FROM_RAW_X;
    profile.xFromRawY = DEFAULT_X_FROM_RAW_Y;
    profile.xOffset = DEFAULT_X_OFFSET;
    profile.yFromRawX = DEFAULT_Y_FROM_RAW_X;
    profile.yFromRawY = DEFAULT_Y_FROM_RAW_Y;
    profile.yOffset = DEFAULT_Y_OFFSET;
    profile.isDefault = true;
    return profile;
}

bool TouchManager::isCalibrationProfileUsable(const CalibrationProfile& profile) {
    if (!isfinite(profile.xFromRawX) || !isfinite(profile.xFromRawY) || !isfinite(profile.xOffset) ||
        !isfinite(profile.yFromRawX) || !isfinite(profile.yFromRawY) || !isfinite(profile.yOffset)) {
        return false;
    }

    float determinant = (profile.xFromRawX * profile.yFromRawY) - (profile.xFromRawY * profile.yFromRawX);
    return fabsf(determinant) > 0.0001f;
}

bool TouchManager::isStableSample(const TS_Point& left, const TS_Point& right) {
    int32_t dx = left.x - right.x;
    int32_t dy = left.y - right.y;
    int32_t radiusSquared = TOUCH_STABILITY_RADIUS * TOUCH_STABILITY_RADIUS;
    return (dx * dx) + (dy * dy) <= radiusSquared;
}
