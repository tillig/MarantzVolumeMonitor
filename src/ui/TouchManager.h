#ifndef TOUCH_MANAGER_H
#define TOUCH_MANAGER_H

#include <XPT2046_Touchscreen.h>
#include <SPI.h>

class TouchManager {
public:
    static TouchManager& getInstance() {
        static TouchManager instance;
        return instance;
    }

    void begin();
    bool isTouched();
    TS_Point getPoint();
    TS_Point getRawPoint();
    static constexpr float getXFromRawX() { return X_FROM_RAW_X; }
    static constexpr float getXFromRawY() { return X_FROM_RAW_Y; }
    static constexpr float getXOffset() { return X_OFFSET; }
    static constexpr float getYFromRawX() { return Y_FROM_RAW_X; }
    static constexpr float getYFromRawY() { return Y_FROM_RAW_Y; }
    static constexpr float getYOffset() { return Y_OFFSET; }

private:
    TouchManager() : _ts(TOUCH_CS) {}
    XPT2046_Touchscreen _ts;
    TS_Point _lastPoint;

    // 9-point affine fit based on measured hardware data captured on 2026-06-03.
    // Important: XPT2046_Touchscreen::setRotation(1) already rotates the raw
    // controller readings. This panel still exhibits slight cross-axis skew, so
    // simple per-axis min/max mapping plus manual offsets was not accurate
    // enough. Preserve the affine model unless fresh calibration data shows
    // otherwise.
    static constexpr float X_FROM_RAW_X = -0.1341911308f;
    static constexpr float X_FROM_RAW_Y = -0.0024769263f;
    static constexpr float X_OFFSET = 528.0057351119f;
    static constexpr float Y_FROM_RAW_X = -0.0029025493f;
    static constexpr float Y_FROM_RAW_Y = -0.0841345531f;
    static constexpr float Y_OFFSET = 337.3623694457f;

    static constexpr uint16_t SCREEN_WIDTH = 480;
    static constexpr uint16_t SCREEN_HEIGHT = 320;
};

#endif
