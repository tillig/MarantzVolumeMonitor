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

private:
    TouchManager() : _ts(TOUCH_CS) {}
    XPT2046_Touchscreen _ts;

    // Calibration constants derived from raw hardware data
    static constexpr uint16_t TS_MIN_X = 350;
    static constexpr uint16_t TS_MAX_X = 3750;
    static constexpr uint16_t TS_MIN_Y = 400;
    static constexpr uint16_t TS_MAX_Y = 3750;

    static constexpr uint16_t SCREEN_WIDTH = 480;
    static constexpr uint16_t SCREEN_HEIGHT = 320;
};

#endif
