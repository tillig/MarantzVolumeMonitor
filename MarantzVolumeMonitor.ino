#include "ConfigurationInterface.h"
#include "IPAddressConverter.h"
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
    Serial.println(IPAddressConverter.toString(Ethernet.localIP()));
    DisplayManager.showMessage("IP address:", IPAddressConverter.toString(Ethernet.localIP()));
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
        ConfigurationInterface.init(&DisplayManager);
        receiverAddress = ConfigurationInterface.readConfigurationFromUser(receiverAddress);
        Serial.print("Updating receiver IP address to: ");
        Serial.println(IPAddressConverter.toString(receiverAddress));
        // TODO: Store IP address in memory
    }
    else
    {
        Serial.println("Skipping setup");
    }

    Serial.print("Receiver IP address: ");
    Serial.println(IPAddressConverter.toString(receiverAddress));
    DisplayManager.showMessage("Receiver IP:", IPAddressConverter.toString(receiverAddress));
    delay(2000);
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
        Serial.println(IPAddressConverter.toString(Ethernet.localIP()));
        break;

    case 3:
        // Failed to rebind to DHCP
        Serial.println("DHCP rebind fail");
        break;

    case 4:
        // DHCP rebind success
        Serial.println("DHCP rebind success");
        Serial.println(IPAddressConverter.toString(Ethernet.localIP()));
        break;

    default:
        //nothing happened
        break;
    }
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
