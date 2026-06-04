#ifndef KEYBOARD_SCREEN_H
#define KEYBOARD_SCREEN_H

#include "Screen.h"
#include <vector>

class KeyboardScreen : public Screen {
public:
    enum class Mode {
        Lowercase,
        Uppercase,
        Symbols
    };

    KeyboardScreen(const String& ssid, ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    String _ssid;
    String _password;
    ScreenReturnTarget _returnTarget;
    bool _showPassword = false;
    Mode _currentMode = Mode::Lowercase;
    bool _capsLock = false;

    struct Key {
        char label;
        int x, y, w, h;
        bool isFunction;
    };
    std::vector<Key> _keys;

    void initKeys();
    void drawKeys();
    void drawInputArea();
};

#endif
