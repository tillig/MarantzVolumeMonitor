#include "StatDisplay.h"
#include <LiquidCrystal.h>

/*
DFRobot LCD Shield
https://www.dfrobot.com/product-51.html
D4-D7    -> LCD Data transmission
D8       -> Register Select
D9       -> Enable pin
D10      -> Backlight control
*/
// Constants for pins
#define LCD_DB4 4
#define LCD_DB5 5
#define LCD_DB6 6
#define LCD_DB7 7
#define LCD_RS 8
#define LCD_ENABLE 9
#define LCD_BACKLIGHT 10
#define NUM_BUTTONS 6
#define BUTTON_PIN 0
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);

bool receiverOn;

void setup() {
    pinMode(LCD_BACKLIGHT, OUTPUT);
    lcd.begin(16, 2);
    StatDisplay.init(&lcd, LCD_BACKLIGHT);
    receiverOn = false;
}

void loop() {
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
        delay(1000);
    }
    else
    {
        if (StatDisplay.isEnabled())
        {
            StatDisplay.disable();
        }

        delay(5000);
    }
}

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