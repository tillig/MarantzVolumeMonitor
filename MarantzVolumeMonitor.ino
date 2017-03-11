#include "ButtonManager.h"
#include "DFRobotLCDShield.h"
#include "DisplayManager.h"
#include <LiquidCrystal.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>

byte mac[] = {
    0x00, 0xAB, 0xBA, 0xC1, 0xDD, 0x03
};
IPAddress receiverAddress;
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
bool receiverOn;

void setup()
{
    // Initialize application state
    receiverOn = false;

    // Serial used for debug messages
    Serial.begin(9600);

    // Initialize display
    pinMode(LCD_BACKLIGHT, OUTPUT);
    lcd.begin(16, 2);
    DisplayManager.init(&lcd, LCD_BACKLIGHT);

    DisplayManager.showMessage("DHCP starting...");
    Serial.println("DHCP starting...");

    // Initialize Ethernet connection
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("No DHCP lease.");
        DisplayManager.showMessage("No DHCP lease.", "Reset to retry.");
        while (true)
        {
            // Block until reset to try again.
        }
    }

    Serial.println("DHCP initialization success");
    Serial.println(getIPAddressString());
    DisplayManager.showMessage("IP address:", getIPAddressString());
    delay(2000);

    bool setupShouldRun = false;
    DisplayManager.showMessage("Push SELECT to", "run setup");
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < 5000)
    {
        if (ButtonManager.readButton() == BUTTON_SELECT)
        {
            Serial.println("User pushed SELECT to start setup");
            setupShouldRun = true;
            break;
        }
    }

    if (!setupShouldRun)
    {
        // TODO: Read saved IP address of Marantz receiver into receiverAddress
        // TODO: If there is no saved IP address, initiate setup.
    }

    if (setupShouldRun)
    {
        Serial.println("Initiating setup");
        DisplayManager.showMessage("Running Setup");
        delay(1000);
        receiverAddress = configureReceiver(receiverAddress);
    }
    else
    {
        Serial.println("Skipping setup");
    }

    Serial.print("Receiver IP address: ");
    Serial.println(getIPAddressString(receiverAddress));
}

IPAddress configureReceiver(IPAddress start)
{
    DisplayManager.showMessage("Receiver IP:");
    IPAddress address = readIPAddressFromConsole(start);
    Serial.print("Updating receiver IP address to: ");
    Serial.println(getIPAddressString(address));
    // TODO: Store IP address in memory
    return address;
}

IPAddress readIPAddressFromConsole(IPAddress start)
{
    Serial.print("Reading IP address from console starting at: ");
    Serial.println(getIPAddressString(start));
    lcd.setCursor(0, 1);
    lcd.print(" ");

    IPAddress address = start;
    lcd.print(getPaddedIPAddressString(address));

    byte cursorPosition = 1;
    lcd.setCursor(cursorPosition, 1);
    lcd.cursor();

    // Setup occurs on the bottom line of the display and looks like
    // " 000.000.000.000"
    //  0123456789012345 (cursor positioning)
    // We skip the leading space, then the user can move the cursor
    // to increment/decrement individual positions. Hitting select
    // ends the process and returns the entered address.
    int buttonPressed = BUTTON_NONE;
    while (buttonPressed != BUTTON_SELECT)
    {
        // TODO: Handle button movements and changing the updated address.
        buttonPressed = ButtonManager.waitForButtonPress();

        switch (buttonPressed)
        {
        case BUTTON_UP:
            Serial.println("Increasing value at cursor.");
            incrementSegment(cursorPosition, address);
            break;
        case BUTTON_DOWN:
            Serial.println("Decreasing value at cursor.");
            break;
        case BUTTON_LEFT:
            if(cursorPosition > 1)
            {
                Serial.println("Moving left.");
                cursorPosition--;
                if (cursorPosition % 4 == 0)
                {
                    // Skip the dots.
                    cursorPosition--;
                }

                moveUnderscore(cursorPosition);
            }
            break;
        case BUTTON_RIGHT:
            if (cursorPosition < 15)
            {
                Serial.println("Moving right.");
                cursorPosition++;
                if (cursorPosition % 4 == 0)
                {
                    // Skip the dots.
                    cursorPosition++;
                }

                moveUnderscore(cursorPosition);
            }
            break;
        default:
            break;
        }

        Serial.print("Cursor position: ");
        Serial.println(cursorPosition);
    };

    lcd.noCursor();

    return address;
}

int getAddressIndexToUpdate(int cursorPosition)
{
    return cursorPosition / 4;
}

int getSegmentQuantityToUpdate(int cursorPosition)
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

void incrementSegment(int cursorPosition, IPAddress address)
{
    if (cursorPosition < 1)
    {
        return;
    }

    int amount = getSegmentQuantityToUpdate(cursorPosition);
    int index = getAddressIndexToUpdate(cursorPosition);

    int value = address[index];
    Serial.print("Incrementing ");
    Serial.print(value);
    Serial.print(" by ");
    Serial.println(amount);
}

void moveUnderscore(int column)
{
    lcd.noCursor();
    lcd.setCursor(column, 1);
    lcd.cursor();
}


void loop()
{
    receiverOn = getReceiverPower();
    if (receiverOn)
    {
        if (!DisplayManager.isDisplayEnabled())
        {
            DisplayManager.enableDisplay();
        }

        // The receiver is on, so get the updated
        // values to display and refresh.
        String input = getInput();
        String vol = getVolume();
        String surround = getSurround();

        DisplayManager.showStats(vol, input, surround);

        // While the receiver is on, refresh
        // twice a second.
        delay(500);
    }
    else
    {
        if (DisplayManager.isDisplayEnabled())
        {
            DisplayManager.disableDisplay();
        }

        // If the receiver is off, only check
        // every five seconds to see if it's turned
        // back on.
        delay(5000);
    }

    refreshDHCP();
}

void refreshDHCP()
{
    // Ethernet.maintain() ensures the DHCP lease is
    // renewed as needed. It only does work if it has
    // to so call it on every loop.
    switch (Ethernet.maintain())
    {
    case 1:
        // Failed to renew DHCP lease
        Serial.println("DHCP renew fail");
        break;

    case 2:
        // DHCP lease renewed
        Serial.println("DHCP renew success");
        Serial.println(getIPAddressString());
        break;

    case 3:
        // Failed to rebind to DHCP
        Serial.println("DHCP rebind fail");
        break;

    case 4:
        // DHCP rebind success
        Serial.println("DHCP rebind success");
        Serial.println(getIPAddressString());
        break;

    default:
        //nothing happened
        break;
    }
}

String getIPAddressString()
{
    return getIPAddressString(Ethernet.localIP());
}

String getIPAddressString(IPAddress ip)
{
    String address = "";
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        address.concat(String(ip[thisByte], DEC));
        if (thisByte < 3)
        {
            address.concat(".");
        }
    }

    return address;
}

String getPaddedIPAddressString(IPAddress ip)
{
    String address = "";
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        String segment = String(ip[thisByte], DEC);
        String prepend = "";
        for (byte toPrepend = 3; toPrepend > segment.length(); toPrepend--)
        {
            prepend.concat("0");
        }
        address.concat(prepend);
        address.concat(segment);
        if (thisByte < 3)
        {
            address.concat(".");
        }
    }

    return address;
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
