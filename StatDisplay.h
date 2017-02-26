// StatDisplay.h

#ifndef _STATDISPLAY_h
#define _STATDISPLAY_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <LiquidCrystal.h>

class StatDisplayClass
{
public:
    void init(LiquidCrystal* lcd, uint8_t backlight);
    bool isEnabled();
    void enable();
    void disable();
    void show(String volume, String input, String surround);
private:
    LiquidCrystal* _lcd;
    uint8_t _backlight;
    bool _enabled;
};

extern StatDisplayClass StatDisplay;

#endif

