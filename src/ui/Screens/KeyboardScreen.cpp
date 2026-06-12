#include "KeyboardScreen.h"
#include "NetworkListScreen.h"
#include "SetupStatusScreen.h"
#include "../IconRenderer.h"
#include "../ScreenManager.h"
#include "../DisplayManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"

// Explicit Control Chars to avoid ASCII collisions
#define KEY_SHIFT '\x01'
#define KEY_MODE '\x02'
#define KEY_BACKSPACE '\x03'
#define KEY_OK '\x04'
#define KEY_CANCEL '\x05'
#define KEY_EYE '\x06'
#define KEY_SPACE ' '

KeyboardScreen::KeyboardScreen(const String& ssid, ScreenReturnTarget returnTarget)
    : _ssid(ssid), _returnTarget(returnTarget) {
    _currentMode = Mode::Lowercase;
    _capsLock = false;
    initKeys();
}

void KeyboardScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    String title = (_ssid == "") ? "Manual SSID Entry" : "Wi-Fi Password: " + _ssid;
    MaterialStyle::drawText(
        tft, MaterialStyle::truncateToWidth(tft, title, 440, 2), 240, 5, MaterialStyle::TextRole::Body, TC_DATUM);

    drawInputArea();
    drawKeys();
}

void KeyboardScreen::update() {}

void KeyboardScreen::handleTouch(TS_Point p) {
    // Calibration preparation: Remove arbitrary offsets.
    int16_t touchX = p.x;
    int16_t touchY = p.y;

    for (const auto& key : _keys) {
        if (touchX >= key.x && touchX < key.x + key.w && touchY >= key.y && touchY < key.y + key.h) {
            if (key.isFunction) {
                switch (key.label) {
                    case KEY_BACKSPACE:
                        if (_password.length() > 0)
                            _password.remove(_password.length() - 1);
                        break;
                    case KEY_OK:
                        if (_ssid == "") {
                            ScreenManager::getInstance().setScreen(new KeyboardScreen(_password, _returnTarget));
                        } else {
                            ScreenManager::getInstance().setScreen(
                                new SetupStatusScreen(_ssid, _password, _returnTarget));
                        }
                        return;
                    case KEY_CANCEL:
                        ScreenManager::getInstance().setScreen(new NetworkListScreen(_returnTarget));
                        return;
                    case KEY_SHIFT:
                        _capsLock = !_capsLock;
                        _currentMode = _capsLock ? Mode::Uppercase : Mode::Lowercase;
                        initKeys();
                        draw();
                        return;
                    case KEY_MODE:
                        if (_currentMode == Mode::Symbols) {
                            _currentMode = _capsLock ? Mode::Uppercase : Mode::Lowercase;
                        } else {
                            _currentMode = Mode::Symbols;
                        }
                        initKeys();
                        draw();
                        return;
                    case KEY_EYE:
                        _showPassword = !_showPassword;
                        draw();
                        return;
                    case KEY_SPACE:
                        _password += " ";
                        break;
                }
            } else {
                _password += key.label;
            }
            drawInputArea();
            delay(150);
            return;
        }
    }
}

void KeyboardScreen::initKeys() {
    _keys.clear();

    std::vector<std::vector<char>> layout;
    const bool isPasswordEntry = _ssid != "";
    if (_currentMode == Mode::Uppercase) {
        layout = {{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
                  {'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P'},
                  {'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L'},
                  {'Z', 'X', 'C', 'V', 'B', 'N', 'M', KEY_BACKSPACE}};
    } else if (_currentMode == Mode::Lowercase) {
        layout = {{'1', '2', '3', '4', '5', '6', '7', '8', '9', '0'},
                  {'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p'},
                  {'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l'},
                  {'z', 'x', 'c', 'v', 'b', 'n', 'm', KEY_BACKSPACE}};
    } else {
        layout = {{'!', '@', '#', '$', '%', '^', '&', '*', '(', ')'},
                  {'-', '_', '=', '+', '[', ']', '{', '}', ';', ':'},
                  {'\'', '"', ',', '.', '/', '?', '|', '\\', '`', '~'},
                  {'<', '>', KEY_BACKSPACE}};
    }
    if (isPasswordEntry) {
        layout.back().push_back(KEY_EYE);
    }

    int startY = 85;
    int keyH = MaterialStyle::KeyboardKeyHeight;
    int margin = MaterialStyle::KeyboardKeyGap;

    // 1. Draw Alphanumeric/Symbol Rows
    for (int r = 0; r < layout.size(); ++r) {
        int rowLen = layout[r].size();
        int keyW = 40;
        int rowWidth = (rowLen * (keyW + margin)) - margin;
        int startX = (480 - rowWidth) / 2;

        for (int i = 0; i < rowLen; ++i) {
            char l = layout[r][i];
            bool isFunc = (l < 32);
            _keys.push_back({l, startX + i * (keyW + margin), startY + r * (keyH + margin), keyW, keyH, isFunc});
        }
    }

    // 2. Bottom Function Row
    int funcW = 80;   // Fit 5 keys without overflowing screen
    int spaceW = 120; // Width for spacebar
    int bottomY = startY + 4 * (keyH + margin);

    int totalWidth =
        (_currentMode != Mode::Symbols) ? (funcW * 4 + spaceW + margin * 4) : (funcW * 3 + spaceW + margin * 3);
    int x = (480 - totalWidth) / 2;

    if (_currentMode != Mode::Symbols) {
        _keys.push_back({KEY_SHIFT, x, bottomY, funcW, keyH, true});
        x += funcW + margin;
    }

    _keys.push_back({KEY_MODE, x, bottomY, funcW, keyH, true});
    x += funcW + margin;
    _keys.push_back({KEY_SPACE, x, bottomY, spaceW, keyH, true});
    x += spaceW + margin;
    _keys.push_back({KEY_CANCEL, x, bottomY, funcW, keyH, true});
    x += funcW + margin;
    _keys.push_back({KEY_OK, x, bottomY, funcW, keyH, true});
}

void KeyboardScreen::drawKeys() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    for (const auto& key : _keys) {
        if (key.label == KEY_EYE) {
            const Icons::IconBitmap& icon = _showPassword ? Icons::KEYBOARD_VISIBILITY_OFF : Icons::KEYBOARD_VISIBILITY;
            MaterialStyle::drawKeyboardIconButton(tft, key.x, key.y, key.w, key.h, icon);
        } else if (key.label == KEY_BACKSPACE) {
            MaterialStyle::drawKeyboardIconButton(tft, key.x, key.y, key.w, key.h, Icons::KEYBOARD_BACKSPACE);
        } else if (key.label == KEY_OK) {
            MaterialStyle::drawKeyboardIconButton(
                tft, key.x, key.y, key.w, key.h, Icons::KEYBOARD_OK, MaterialStyle::ComponentState::Success);
        } else if (key.label == KEY_CANCEL) {
            MaterialStyle::drawKeyboardIconButton(
                tft, key.x, key.y, key.w, key.h, Icons::KEYBOARD_CANCEL, MaterialStyle::ComponentState::Error);
        } else if (key.label == KEY_SHIFT) {
            MaterialStyle::drawKeyboardIconButton(tft,
                                                  key.x,
                                                  key.y,
                                                  key.w,
                                                  key.h,
                                                  Icons::KEYBOARD_CAPS_LOCK,
                                                  _capsLock ? MaterialStyle::ComponentState::Selected
                                                            : MaterialStyle::ComponentState::Normal);
        } else {
            String label;
            if (key.label == KEY_MODE)
                label = (_currentMode == Mode::Symbols) ? "ABC" : "!@#$";
            else if (key.label == KEY_SPACE)
                label = "SPACE";
            else
                label = String(key.label);

            MaterialStyle::ComponentState state =
                key.isFunction ? MaterialStyle::ComponentState::Normal : MaterialStyle::ComponentState::Inactive;
            MaterialStyle::drawKeyboardTextButton(tft, key.x, key.y, key.w, key.h, label, state);
        }
    }
}

void KeyboardScreen::drawInputArea() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillRect(20, 35, 440, 40, DisplayManager::COLOR_BACKGROUND);
    String displayStr = "";
    if (_showPassword || _ssid == "") {
        displayStr = _password;
    } else {
        for (int i = 0; i < _password.length(); ++i)
            displayStr += "*";
    }
    displayStr += "_";

    MaterialStyle::drawInputField(tft, 20, 35, 440, 40, displayStr, MaterialStyle::TextRole::SectionLabel);
}
