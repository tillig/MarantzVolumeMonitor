#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <XPT2046_Touchscreen.h>
#include <SPI.h>

class TouchManager {
public:
    struct CalibrationProfile {
        uint8_t version = 1;
        float xFromRawX = 0.0f;
        float xFromRawY = 0.0f;
        float xOffset = 0.0f;
        float yFromRawX = 0.0f;
        float yFromRawY = 0.0f;
        float yOffset = 0.0f;
        bool isDefault = true;
    };

    static TouchManager& getInstance() {
        static TouchManager instance;
        return instance;
    }

    void begin();
    bool isTouched();
    bool isTouchContactPresent();
    TS_Point getPoint();
    TS_Point getRawPoint();
    TS_Point mapRawPoint(const TS_Point& rawPoint) const;
    bool applyCalibrationProfile(const CalibrationProfile& profile);
    void applyDefaultCalibration();
    CalibrationProfile activeCalibrationProfile() const;
    static CalibrationProfile defaultCalibrationProfile();
    static bool isCalibrationProfileUsable(const CalibrationProfile& profile);

private:
    TouchManager() : _ts(TOUCH_CS) {}
    XPT2046_Touchscreen _ts;
    TS_Point _lastPoint;
    TS_Point _candidatePoint;
    uint32_t _touchCandidateStartedMs = 0;
    uint8_t _touchStableSampleCount = 0;
    bool _touchCandidateActive = false;

    // 9-point affine fit based on measured hardware data captured on 2026-06-03.
    // Important: XPT2046_Touchscreen::setRotation(1) already rotates the raw
    // controller readings. This panel still exhibits slight cross-axis skew, so
    // simple per-axis min/max mapping plus manual offsets was not accurate
    // enough. Preserve the affine model unless fresh calibration data shows
    // otherwise.
    static constexpr float DEFAULT_X_FROM_RAW_X = -0.1341911308f;
    static constexpr float DEFAULT_X_FROM_RAW_Y = -0.0024769263f;
    static constexpr float DEFAULT_X_OFFSET = 528.0057351119f;
    static constexpr float DEFAULT_Y_FROM_RAW_X = -0.0029025493f;
    static constexpr float DEFAULT_Y_FROM_RAW_Y = -0.0841345531f;
    static constexpr float DEFAULT_Y_OFFSET = 337.3623694457f;

    static constexpr uint16_t SCREEN_WIDTH = 480;
    static constexpr uint16_t SCREEN_HEIGHT = 320;
    static constexpr uint32_t TOUCH_CONFIRM_MS = 50;
    static constexpr uint8_t TOUCH_CONFIRM_SAMPLES = 4;
    static constexpr uint16_t TOUCH_STABILITY_RADIUS = 120;
    CalibrationProfile _activeProfile = defaultCalibrationProfile();

    static bool isStableSample(const TS_Point& left, const TS_Point& right);
};

#endif
