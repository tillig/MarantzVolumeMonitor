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
    TouchManager() : _ts(4) {} // D2 = GPIO 4
    XPT2046_Touchscreen _ts;
};

#endif
