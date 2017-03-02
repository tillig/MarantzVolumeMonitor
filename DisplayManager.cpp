// 
// 
// 

#include "DisplayManager.h"

DisplayManagerClass DisplayManager;

void DisplayManagerClass::init(LiquidCrystal* lcd, uint8_t backlight)
{
    _lcd = lcd;
    _backlight = backlight;
    enableDisplay();
}

bool DisplayManagerClass::isDisplayEnabled()
{
    return _enabled;
}

void DisplayManagerClass::enableDisplay()
{
    _enabled = true;
    digitalWrite(_backlight, HIGH);
    _lcd->clear();
    _lcd->display();
}

void DisplayManagerClass::disableDisplay()
{
    _enabled = false;
    digitalWrite(_backlight, LOW);
    _lcd->clear();
    _lcd->noDisplay();
}

void DisplayManagerClass::resetLine(uint8_t line)
{
    _lcd->setCursor(0, line);
    _lcd->print("                ");
    _lcd->setCursor(0, line);
}

void DisplayManagerClass::showStats(String volume, String input, String surround)
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

void DisplayManagerClass::showMessage(String line)
{
    if (!this->isDisplayEnabled())
    {
        this->enableDisplay();
    }

    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print(line);
}

void DisplayManagerClass::showMessage(String line1, String line2)
{
    if (!this->isDisplayEnabled())
    {
        this->enableDisplay();
    }

    _lcd->clear();
    _lcd->setCursor(0, 0);
    _lcd->print(line1);
    _lcd->setCursor(0, 1);
    _lcd->print(line2);
}
