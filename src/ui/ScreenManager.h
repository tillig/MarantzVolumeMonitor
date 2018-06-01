#ifndef SCREEN_MANAGER_H
#define SCREEN_MANAGER_H

#include "Screens/Screen.h"

class ScreenManager {
public:
    static ScreenManager& getInstance() {
        static ScreenManager instance;
        return instance;
    }

    void setScreen(Screen* screen);
    void update();
    void handleTouch(TS_Point p);

private:
    ScreenManager() : _currentScreen(nullptr) {}
    Screen* _currentScreen;
};

#endif
