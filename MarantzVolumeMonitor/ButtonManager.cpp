#include "ButtonManager.h"

ButtonManagerClass ButtonManager;

int ButtonManagerClass::readButton()
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

int ButtonManagerClass::waitForButtonPress()
{
    int buttonPressed = BUTTON_NONE;
    while ((buttonPressed = ButtonManager.readButton()) == BUTTON_NONE)
    {
        // Wait for button press.
    }

    while (ButtonManager.readButton() != BUTTON_NONE)
    {
        // Wait for button release.
    }

    return buttonPressed;
}
