#include "ConfigurationInterface.h"
/*
 * PUBLIC METHODS
 */

void ConfigurationInterfaceClass::init(DisplayManagerClass* displayManager)
{
    _displayManager = displayManager;
}

IPAddress ConfigurationInterfaceClass::readConfigurationFromUser(IPAddress start)
{
    _displayManager->showMessage("Receiver IP:");
    IPAddress address = this->runUserInputLoop(start);
    return address;
}

/*
 * PROTECTED METHODS
 */

IPAddress ConfigurationInterfaceClass::incrementSegment(byte cursorPosition, IPAddress address)
{
    if (cursorPosition < 1)
    {
        return address;
    }

    int amount = this->getAmountToChangeSegment(cursorPosition);
    return this->changeSegment(cursorPosition, address, amount);
}

IPAddress ConfigurationInterfaceClass::decrementSegment(byte cursorPosition, IPAddress address)
{
    if (cursorPosition < 1)
    {
        return address;
    }

    int amount = this->getAmountToChangeSegment(cursorPosition);
    return this->changeSegment(cursorPosition, address, -amount);
}

/*
 * PRIVATE METHODS
 */

IPAddress ConfigurationInterfaceClass::changeSegment(byte cursorPosition, IPAddress address, int quantity)
{
    if (cursorPosition < 1)
    {
        return address;
    }

    int ipindex = cursorPosition / 4;
    int currentValue = address[ipindex];
    Serial.print("Changing value ");
    Serial.print(currentValue);
    Serial.print(" at address index ");
    Serial.print(ipindex);
    Serial.print(" by ");
    Serial.println(quantity);

    if (currentValue + quantity <= 255 && currentValue + quantity >= 0)
    {
        currentValue = currentValue + quantity;
    }

    address[ipindex] = currentValue;

    Serial.print("Updated address is ");
    Serial.println(IPAddressConverter.toString(address));
    this->updateAddressEntryLine(cursorPosition, address);

    return address;
}

int ConfigurationInterfaceClass::getAmountToChangeSegment(byte cursorPosition)
{
    if (cursorPosition < 1)
    {
        return;
    }

    // Figure out how much to add based on
    // cursor position
    // " 000.000.000.000"
    //  0123456789012345
    int quantity = 0;
    switch (cursorPosition % 4)
    {
    case 1:
        quantity = 100;
        break;
    case 2:
        quantity = 10;
        break;
    case 3:
        quantity = 1;
        break;
    default:
        // Skip the dots.
        break;
    }

    return quantity;
}

IPAddress ConfigurationInterfaceClass::runUserInputLoop(IPAddress start)
{
    Serial.print("Reading IP address from console starting at: ");
    Serial.println(IPAddressConverter.toString(start));
    IPAddress address = start;
    byte cursorPosition = 1;
    this->updateAddressEntryLine(cursorPosition, address);

    // Setup occurs on the bottom line of the display and looks like
    // " 000.000.000.000"
    //  0123456789012345 (cursor positioning)
    // We skip the leading space, then the user can move the cursor
    // to increment/decrement individual positions. Hitting select
    // ends the process and returns the entered address.
    int buttonPressed = BUTTON_NONE;
    while (buttonPressed != BUTTON_SELECT || address[0] == 0)
    {
        buttonPressed = ButtonManager.waitForButtonPress();

        switch (buttonPressed)
        {
        case BUTTON_UP:
            address = incrementSegment(cursorPosition, address);
            break;
        case BUTTON_DOWN:
            address = decrementSegment(cursorPosition, address);
            break;
        case BUTTON_LEFT:
            if (cursorPosition > 1)
            {
                cursorPosition--;
                if (cursorPosition % 4 == 0)
                {
                    // Skip the dots.
                    cursorPosition--;
                }

                _displayManager->moveCursor(cursorPosition, 1);
            }
            break;
        case BUTTON_RIGHT:
            if (cursorPosition < 15)
            {
                cursorPosition++;
                if (cursorPosition % 4 == 0)
                {
                    // Skip the dots.
                    cursorPosition++;
                }

                _displayManager->moveCursor(cursorPosition, 1);
            }
            break;
        default:
            break;
        }
    };

    _displayManager->lcd()->noCursor();

    return address;
}

void ConfigurationInterfaceClass::updateAddressEntryLine(byte cursorPosition, IPAddress address)
{
    _displayManager->lcd()->noCursor();
    _displayManager->lcd()->setCursor(0, 1);
    _displayManager->lcd()->print(" ");
    _displayManager->lcd()->print(IPAddressConverter.toPaddedString(address));
    _displayManager->lcd()->setCursor(cursorPosition, 1);
    _displayManager->lcd()->cursor();
}

ConfigurationInterfaceClass ConfigurationInterface;

