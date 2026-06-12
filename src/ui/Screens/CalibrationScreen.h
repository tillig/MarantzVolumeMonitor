#ifndef CALIBRATION_SCREEN_H
#define CALIBRATION_SCREEN_H

#include "Screen.h"
#include "../MaterialStyle.h"

class CalibrationScreen : public Screen {
public:
    CalibrationScreen(ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    static constexpr uint8_t PointCount = 9;
    static constexpr int16_t TargetRadiusPx = 28;
    static constexpr uint16_t DuplicateRawDistanceThreshold = 140;
    static constexpr uint16_t FitErrorThresholdPx = 18;
    static constexpr uint32_t CaptureCooldownMs = 260;

    struct CalibrationPoint {
        int16_t x;
        int16_t y;
        const char* label;
    };

    struct CalibrationSample {
        uint16_t rawX = 0;
        uint16_t rawY = 0;
        uint16_t rawZ = 0;
        int16_t mappedX = 0;
        int16_t mappedY = 0;
    };

    enum class SessionState { Collecting, RetryableFailure, Complete };

    uint8_t _step = 0;
    bool _awaitingRelease = false;
    uint32_t _nextCaptureAllowedAtMs = 0;
    ScreenReturnTarget _returnTarget;
    SessionState _sessionState = SessionState::Collecting;
    String _statusTitle;
    String _statusMessage;
    MaterialStyle::ComponentState _statusState = MaterialStyle::ComponentState::Normal;

    CalibrationPoint _points[PointCount] = {{48, 40, "Top Left"},
                                            {240, 40, "Top Center"},
                                            {432, 40, "Top Right"},
                                            {48, 144, "Mid Left"},
                                            {240, 144, "Center"},
                                            {432, 144, "Mid Right"},
                                            {48, 276, "Bottom Left"},
                                            {240, 276, "Bottom Center"},
                                            {432, 276, "Bottom Right"}};
    CalibrationSample _samples[PointCount];

    void resetSession();
    void setStatus(const String& title, const String& message, MaterialStyle::ComponentState state);
    bool isCancelPressed(TS_Point p) const;
    bool isOkPressed(TS_Point p) const;
    bool isWithinActiveTarget(const TS_Point& mappedPoint) const;
    bool isSuspiciousDuplicate(const TS_Point& rawPoint) const;
    bool computeCalibrationProfile(TouchManager::CalibrationProfile& profile) const;
    bool solveLeastSquaresAxis(const float* rawX,
                               const float* rawY,
                               const float* targets,
                               size_t count,
                               float& fromRawX,
                               float& fromRawY,
                               float& offset) const;
    bool validateCalibrationProfile(const TouchManager::CalibrationProfile& profile);
    bool saveCalibrationProfile(const TouchManager::CalibrationProfile& profile);
    void exitScreen() const;
};

#endif
