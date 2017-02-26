// 
// 
// 

#include "StatDisplay.h"

StatDisplayClass StatDisplay;

void StatDisplayClass::init(LiquidCrystal* lcd, uint8_t backlight)
{
    _lcd = lcd;
    _backlight = backlight;
    disable();
}

bool StatDisplayClass::isEnabled()
{
    return _enabled;
}

void StatDisplayClass::enable()
{
    _enabled = true;
    digitalWrite(_backlight, HIGH);
    _lcd->clear();
    _lcd->display();
}

void StatDisplayClass::disable()
{
    _enabled = false;
    digitalWrite(_backlight, LOW);
    _lcd->clear();
    _lcd->noDisplay();
}

void StatDisplayClass::show(String volume, String input, String surround)
{
    // Display is
    //
    // {input} [{vol}]
    // {surround}
    //
    // like
    //
    // Xbox One  [10.5]
    // Multi Ch Input 7
    //
    // input truncated to 9
    // volume gets padded to 4
    // surround truncated to 16

    // Clear the display because the values
    // may be different lengths and you don't
    // want overlaps.
    _lcd->clear();

    // Display the input info.
    _lcd->setCursor(0, 0);
    if (input.length() > 9)
    {
        input.remove(9, input.length() - 9);
    }
    _lcd->print(input);
    _lcd->setCursor(10, 0);

    // Display the volume info.
    _lcd->print("[");
    if (volume.length() < 4)
    {
        // Pad short values on the left.
        _lcd->print(" ");
    }
    _lcd->print(volume);
    _lcd->print("]");

    _lcd->setCursor(0, 1);
    _lcd->print(surround);
}
