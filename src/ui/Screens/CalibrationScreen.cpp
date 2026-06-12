#include "CalibrationScreen.h"

#include <Arduino.h>
#include <math.h>

#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "../DisplayManager.h"
#include "../IconRenderer.h"
#include "../MaterialStyle.h"
#include "../ScreenManager.h"
#include "../TouchManager.h"
#include "../../storage/ConfigStore.h"

namespace {
constexpr int CancelButtonX = 172;
constexpr int CancelButtonWidth = 136;
constexpr int CancelButtonY = 190;

bool solve3x3(float matrix[3][4], float out[3]) {
    for (int pivot = 0; pivot < 3; ++pivot) {
        int bestRow = pivot;
        float bestAbs = fabsf(matrix[pivot][pivot]);
        for (int row = pivot + 1; row < 3; ++row) {
            float candidateAbs = fabsf(matrix[row][pivot]);
            if (candidateAbs > bestAbs) {
                bestAbs = candidateAbs;
                bestRow = row;
            }
        }

        if (bestAbs < 0.000001f) {
            return false;
        }

        if (bestRow != pivot) {
            for (int column = pivot; column < 4; ++column) {
                float swap = matrix[pivot][column];
                matrix[pivot][column] = matrix[bestRow][column];
                matrix[bestRow][column] = swap;
            }
        }

        float pivotValue = matrix[pivot][pivot];
        for (int column = pivot; column < 4; ++column) {
            matrix[pivot][column] /= pivotValue;
        }

        for (int row = 0; row < 3; ++row) {
            if (row == pivot) {
                continue;
            }

            float factor = matrix[row][pivot];
            for (int column = pivot; column < 4; ++column) {
                matrix[row][column] -= factor * matrix[pivot][column];
            }
        }
    }

    out[0] = matrix[0][3];
    out[1] = matrix[1][3];
    out[2] = matrix[2][3];
    return true;
}
}

CalibrationScreen::CalibrationScreen(ScreenReturnTarget returnTarget)
    : _returnTarget(returnTarget) {}

void CalibrationScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    if (_sessionState == SessionState::Complete) {
        MaterialStyle::drawPageHeader(tft, Icons::TOUCH_CALIBRATION, "Touch Calibration",
                                      "Calibration saved and applied");
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Success,
                                       "Alignment updated",
                                       "Press OK to return to navigation.",
                                       Icons::SUCCESS);
        MaterialStyle::drawStandardButton(tft, 182, MaterialStyle::BottomActionY, 116,
                                          MaterialStyle::ButtonHeight,
                                          Icons::KEYBOARD_OK, "OK",
                                          MaterialStyle::ComponentState::Success);
        return;
    }

    if (_sessionState == SessionState::RetryableFailure) {
        MaterialStyle::drawPageHeader(tft, Icons::TOUCH_CALIBRATION, "Touch Calibration",
                                      "Retry the full calibration");
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Error,
                                       _statusTitle, _statusMessage, Icons::WARNING);
        MaterialStyle::drawText(tft, "Tap anywhere to retry, or Cancel to leave.", 240, 220,
                                MaterialStyle::TextRole::Body, TC_DATUM);
        MaterialStyle::drawStandardButton(tft, 172, MaterialStyle::BottomActionY,
                                          136, MaterialStyle::ButtonHeight,
                                          Icons::KEYBOARD_CANCEL, "Cancel",
                                          MaterialStyle::ComponentState::Error);
        return;
    } else {
        const CalibrationPoint& point = _points[_step];
        MaterialStyle::drawText(tft, "Touch Calibration", 240, 12,
                                MaterialStyle::TextRole::ScreenTitle, TC_DATUM);
        MaterialStyle::drawText(tft, "Tap each crosshair directly.", 240, 78,
                                MaterialStyle::TextRole::Body, TC_DATUM);

        if (_statusTitle.length() > 0) {
            String status = _statusTitle;
            if (_statusMessage.length() > 0) {
                status += ": " + _statusMessage;
            }
            MaterialStyle::drawText(tft, status, 240, 174,
                                    MaterialStyle::TextRole::CompactMetadata, TC_DATUM, _statusState);
        }

        tft.drawLine(point.x - 14, point.y, point.x + 14, point.y, TFT_RED);
        tft.drawLine(point.x, point.y - 14, point.x, point.y + 14, TFT_RED);
        tft.drawCircle(point.x, point.y, 8, TFT_RED);
        tft.drawCircle(point.x, point.y, TargetRadiusPx, DisplayManager::COLOR_BAR_BG);
    }

    MaterialStyle::drawStandardButton(tft, CancelButtonX, CancelButtonY,
                                      CancelButtonWidth, MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_CANCEL, "Cancel",
                                      MaterialStyle::ComponentState::Error);
}

void CalibrationScreen::update() {
    if (_awaitingRelease && !TouchManager::getInstance().isTouchContactPresent()) {
        _awaitingRelease = false;
    }
}

void CalibrationScreen::handleTouch(TS_Point p) {
    if (_sessionState == SessionState::Complete) {
        if (isOkPressed(p)) {
            exitScreen();
        }
        return;
    }

    if (isCancelPressed(p)) {
        exitScreen();
        return;
    }

    if (_sessionState == SessionState::RetryableFailure) {
        resetSession();
        draw();
        return;
    }

    uint32_t now = millis();
    if (_awaitingRelease || now < _nextCaptureAllowedAtMs) {
        return;
    }

    if (!isWithinActiveTarget(p)) {
        setStatus("Tap the highlighted point",
                  "Stay inside the target ring before releasing.",
                  MaterialStyle::ComponentState::Warning);
        _nextCaptureAllowedAtMs = now + CaptureCooldownMs;
        draw();
        return;
    }

    TS_Point raw = TouchManager::getInstance().getRawPoint();
    if (isSuspiciousDuplicate(raw)) {
        setStatus("Sample rejected",
                  "That tap was too close to an earlier point. Try again.",
                  MaterialStyle::ComponentState::Warning);
        _nextCaptureAllowedAtMs = now + CaptureCooldownMs;
        draw();
        return;
    }

    CalibrationSample& sample = _samples[_step];
    sample.rawX = static_cast<uint16_t>(raw.x);
    sample.rawY = static_cast<uint16_t>(raw.y);
    sample.rawZ = static_cast<uint16_t>(raw.z);
    sample.mappedX = p.x;
    sample.mappedY = p.y;
    _awaitingRelease = true;
    _nextCaptureAllowedAtMs = now + CaptureCooldownMs;
    _statusTitle = "";
    _statusMessage = "";
    _statusState = MaterialStyle::ComponentState::Normal;
    _step++;

    if (_step >= PointCount) {
        TouchManager::CalibrationProfile profile;
        if (!computeCalibrationProfile(profile) || !validateCalibrationProfile(profile) ||
            !saveCalibrationProfile(profile)) {
            _sessionState = SessionState::RetryableFailure;
            if (_statusTitle.length() == 0) {
                setStatus("Calibration failed",
                          "The captured points were not usable. Retry the full calibration.",
                          MaterialStyle::ComponentState::Error);
            }
        } else {
            _sessionState = SessionState::Complete;
        }
    }

    draw();
}

void CalibrationScreen::resetSession() {
    _step = 0;
    _awaitingRelease = false;
    _nextCaptureAllowedAtMs = 0;
    _sessionState = SessionState::Collecting;
    _statusTitle = "";
    _statusMessage = "";
    _statusState = MaterialStyle::ComponentState::Normal;
    for (uint8_t i = 0; i < PointCount; ++i) {
        _samples[i] = CalibrationSample();
    }
}

void CalibrationScreen::setStatus(const String& title, const String& message,
                                  MaterialStyle::ComponentState state) {
    _statusTitle = title;
    _statusMessage = message;
    _statusState = state;
}

bool CalibrationScreen::isCancelPressed(TS_Point p) const {
    return p.x >= CancelButtonX && p.x <= CancelButtonX + CancelButtonWidth &&
           p.y >= CancelButtonY &&
           p.y <= CancelButtonY + MaterialStyle::ButtonHeight;
}

bool CalibrationScreen::isOkPressed(TS_Point p) const {
    return p.x >= 182 && p.x <= 298 &&
           p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool CalibrationScreen::isWithinActiveTarget(const TS_Point& mappedPoint) const {
    const CalibrationPoint& point = _points[_step];
    int32_t dx = mappedPoint.x - point.x;
    int32_t dy = mappedPoint.y - point.y;
    return (dx * dx) + (dy * dy) <= (TargetRadiusPx * TargetRadiusPx);
}

bool CalibrationScreen::isSuspiciousDuplicate(const TS_Point& rawPoint) const {
    int32_t thresholdSquared = DuplicateRawDistanceThreshold * DuplicateRawDistanceThreshold;
    for (uint8_t i = 0; i < _step; ++i) {
        int32_t dx = rawPoint.x - _samples[i].rawX;
        int32_t dy = rawPoint.y - _samples[i].rawY;
        if ((dx * dx) + (dy * dy) <= thresholdSquared) {
            return true;
        }
    }
    return false;
}

bool CalibrationScreen::computeCalibrationProfile(TouchManager::CalibrationProfile& profile) const {
    float rawX[PointCount];
    float rawY[PointCount];
    float targetX[PointCount];
    float targetY[PointCount];
    for (uint8_t i = 0; i < PointCount; ++i) {
        rawX[i] = static_cast<float>(_samples[i].rawX);
        rawY[i] = static_cast<float>(_samples[i].rawY);
        targetX[i] = static_cast<float>(_points[i].x);
        targetY[i] = static_cast<float>(_points[i].y);
    }

    profile = TouchManager::CalibrationProfile();
    profile.version = 1;
    profile.isDefault = false;
    return solveLeastSquaresAxis(rawX, rawY, targetX, PointCount,
                                 profile.xFromRawX, profile.xFromRawY, profile.xOffset) &&
           solveLeastSquaresAxis(rawX, rawY, targetY, PointCount,
                                 profile.yFromRawX, profile.yFromRawY, profile.yOffset);
}

bool CalibrationScreen::solveLeastSquaresAxis(const float* rawX, const float* rawY,
                                              const float* targets, size_t count,
                                              float& fromRawX, float& fromRawY,
                                              float& offset) const {
    float sumXX = 0.0f;
    float sumXY = 0.0f;
    float sumYY = 0.0f;
    float sumX = 0.0f;
    float sumY = 0.0f;
    float sumTarget = 0.0f;
    float sumXTarget = 0.0f;
    float sumYTarget = 0.0f;

    for (size_t i = 0; i < count; ++i) {
        sumXX += rawX[i] * rawX[i];
        sumXY += rawX[i] * rawY[i];
        sumYY += rawY[i] * rawY[i];
        sumX += rawX[i];
        sumY += rawY[i];
        sumTarget += targets[i];
        sumXTarget += rawX[i] * targets[i];
        sumYTarget += rawY[i] * targets[i];
    }

    float matrix[3][4] = {
        {sumXX, sumXY, sumX, sumXTarget},
        {sumXY, sumYY, sumY, sumYTarget},
        {sumX, sumY, static_cast<float>(count), sumTarget}
    };
    float solution[3];
    if (!solve3x3(matrix, solution)) {
        return false;
    }

    fromRawX = solution[0];
    fromRawY = solution[1];
    offset = solution[2];
    return true;
}

bool CalibrationScreen::validateCalibrationProfile(const TouchManager::CalibrationProfile& profile) {
    if (!TouchManager::isCalibrationProfileUsable(profile)) {
        setStatus("Calibration failed",
                  "The fitted profile was not numerically stable.",
                  MaterialStyle::ComponentState::Error);
        return false;
    }

    for (uint8_t i = 0; i < PointCount; ++i) {
        TS_Point raw(_samples[i].rawX, _samples[i].rawY, _samples[i].rawZ);
        TS_Point mapped = TouchManager::getInstance().mapRawPoint(raw);
        float fittedX = (profile.xFromRawX * raw.x) + (profile.xFromRawY * raw.y) + profile.xOffset;
        float fittedY = (profile.yFromRawX * raw.x) + (profile.yFromRawY * raw.y) + profile.yOffset;
        if (fittedX < 0.0f || fittedX > 479.0f || fittedY < 0.0f || fittedY > 319.0f) {
            setStatus("Calibration failed",
                      "The solved profile mapped points outside the screen.",
                      MaterialStyle::ComponentState::Error);
            return false;
        }

        float dx = fittedX - _points[i].x;
        float dy = fittedY - _points[i].y;
        if (sqrtf((dx * dx) + (dy * dy)) > FitErrorThresholdPx) {
            setStatus("Calibration failed",
                      "The captured points were not consistent enough. Retry.",
                      MaterialStyle::ComponentState::Error);
            return false;
        }

        (void)mapped;
    }

    return true;
}

bool CalibrationScreen::saveCalibrationProfile(const TouchManager::CalibrationProfile& profile) {
    DeviceConfig config;
    ConfigStore::getInstance().loadConfig(config);
    config.touchCalibration.isPresent = true;
    config.touchCalibration.version = profile.version;
    config.touchCalibration.xFromRawX = profile.xFromRawX;
    config.touchCalibration.xFromRawY = profile.xFromRawY;
    config.touchCalibration.xOffset = profile.xOffset;
    config.touchCalibration.yFromRawX = profile.yFromRawX;
    config.touchCalibration.yFromRawY = profile.yFromRawY;
    config.touchCalibration.yOffset = profile.yOffset;

    if (!ConfigStore::getInstance().saveConfig(config)) {
        setStatus("Save failed",
                  "The new calibration could not be stored.",
                  MaterialStyle::ComponentState::Error);
        return false;
    }

    return TouchManager::getInstance().applyCalibrationProfile(profile);
}

void CalibrationScreen::exitScreen() const {
    if (_returnTarget == ScreenReturnTarget::Settings) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    } else {
        ScreenManager::getInstance().setScreen(new HomeScreen());
    }
}
