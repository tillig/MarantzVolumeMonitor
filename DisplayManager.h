// DisplayManager.h

#ifndef _DISPLAYMANAGER_h
#define _DISPLAYMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <LiquidCrystal.h>

class DisplayManagerClass
{
public:
    void init(LiquidCrystal* lcd, uint8_t backlight);
    bool isDisplayEnabled();
    void enableDisplay();
    void disableDisplay();
    void resetLine(uint8_t line);
    void showStats(String volume, String input, String surround);
    void showMessage(String line);
    void showMessage(String line1, String line2);
private:
    LiquidCrystal* _lcd;
    uint8_t _backlight;
    bool _enabled;
};

extern DisplayManagerClass DisplayManager;

#endif

