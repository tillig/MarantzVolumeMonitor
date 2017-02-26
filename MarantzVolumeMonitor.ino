#include "StatDisplay.h"
#include "DFRobotLCDShield.h"
#include <LiquidCrystal.h>

LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

bool receiverOn;

void setup()
{
    pinMode(LCD_BACKLIGHT, OUTPUT);
    lcd.begin(16, 2);
    StatDisplay.init(&lcd, LCD_BACKLIGHT);
    receiverOn = false;
}

void loop()
{
    receiverOn = getReceiverPower();
    if (receiverOn)
    {
        if (!StatDisplay.isEnabled())
        {
            StatDisplay.enable();
        }

        // The receiver is on, so get the updated
        // values to display and refresh.
        String input = getInput();
        String vol = getVolume();
        String surround = getSurround();

        StatDisplay.show(vol, input, surround);

        // While the receiver is on, refresh
        // twice a second.
        delay(500);
    }
    else
    {
        if (StatDisplay.isEnabled())
        {
            StatDisplay.disable();
        }

        // If the receiver is off, only check
        // every five seconds to see if it's turned
        // back on.
        delay(5000);
    }
}

int getButton()
{
    int adc_key_in = analogRead(0);
    if (adc_key_in > 1000) {
        return BUTTON_NONE;
    }

    if (adc_key_in < BUTTON_RIGHT_TOLERANCE)
    {
        return BUTTON_RIGHT;
    }

    if (adc_key_in < BUTTON_UP_TOLERANCE)
    {
        return BUTTON_UP;
    }

    if (adc_key_in < BUTTON_DOWN_TOLERANCE)
    {
        return BUTTON_DOWN;
    }

    if (adc_key_in < BUTTON_LEFT_TOLERANCE)
    {
        return BUTTON_LEFT;
    }

    if (adc_key_in < BUTTON_SELECT_TOLERANCE)
    {
        return BUTTON_SELECT;
    }

    return BUTTON_NONE;
}

// STUBS!
// Getting the input and volume will probably be
// a single HTTP call to the receiver API.

bool getReceiverPower()
{
    return random(1, 100) > 20;
}

String getInput()
{
    if (random(1, 100) % 2 == 0)
    {
        return "Roku";
    }

    return "XboxOneSuperLong";
}

String getSurround()
{
    if (random(1, 100) % 2 == 0)
    {
        return "Stereo";
    }

    return "Multi Ch Input 7.1";
}

String getVolume()
{
    if (random(1, 100) % 5 == 0)
    {
        return "MUTE";
    }

    float roundedVol = float(int((random(0, 1000) / 10.0) * 10) / 10.0);
    return String(roundedVol, 1);
}