#include "ScreenManager.h"
#include "DisplayManager.h"

void ScreenManager::setScreen(Screen* screen) {
    if (_currentScreen != nullptr) {
        delete _currentScreen;
    }
    _currentScreen = screen;
    if (_currentScreen != nullptr) {
        DisplayManager::getInstance().setBacklightEnabled(true);
        _currentScreen->draw();
    }
}

void ScreenManager::update() {
    if (_currentScreen != nullptr) {
        _currentScreen->update();
    }
}

void ScreenManager::handleTouch(TS_Point p) {
    if (_currentScreen != nullptr) {
        _currentScreen->handleTouch(p);
    }
}
