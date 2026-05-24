#ifndef SCREEN_H
#define SCREEN_H

#include "../DisplayManager.h"
#include "../TouchManager.h"

class Screen {
public:
    virtual ~Screen() {}
    virtual void draw() = 0;
    virtual void update() = 0;
    virtual void handleTouch(TS_Point p) = 0;
};

#endif
