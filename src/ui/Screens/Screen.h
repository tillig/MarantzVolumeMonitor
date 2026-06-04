#ifndef SCREEN_H
#define SCREEN_H

#include "../DisplayManager.h"
#include "../TouchManager.h"

enum class ScreenReturnTarget {
    Home,
    Settings
};

class Screen {
public:
    virtual ~Screen() {}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleTouch(TS_Point p) = 0;
};

#endif
