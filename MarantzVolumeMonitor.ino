#include "DFRobotLCDShield.h"
#include "StatDisplay.h"
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
    StatDisplay.init(&lcd, LCD_BACKLIGHT);

    lcd.setCursor(0, 0);
    lcd.print("DHCP starting...");
    Serial.println("DHCP starting...");

    // Initialize Ethernet connection
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("No DHCP lease.");
        showDhcpError("No DHCP lease.", "Reset to retry.");
        while (true)
        {
            // Block until reset to try again.
        }
    }

    Serial.println("DHCP success");
    resetLine(0);
    lcd.print("IP address:");
    printIPAddress();
}

void showDhcpError(String line1, String line2)
{
    digitalWrite(LCD_BACKLIGHT, HIGH);
    lcd.clear();
    lcd.display();
    lcd.setCursor(0, 0);
    lcd.print(line1);
    Serial.println(line1);

    if (line2.length() > 0)
    {
        lcd.setCursor(0, 1);
        lcd.print(line2);
        Serial.println(line2);
    }
}

void loop()
{
    // Ethernet.maintain() ensures the DHCP lease is
    // renewed as needed. It only does work if it has
    // to so call it on every loop.
    switch (Ethernet.maintain())
    {
    case 1:
        // Failed to renew DHCP lease
        showDhcpError("DHCP renew fail", "");
        break;

    case 2:
        //renewed success
        Serial.println("Renew success");
        resetLine(0);
        lcd.print("Renew success");
        printIPAddress();
        break;

    case 3:
        // Failed to rebind to DHCP
        Serial.println("Rebind fail");
        showDhcpError("DHCP rebind fail", "");
        break;

    case 4:
        // Rebind success
        Serial.println("Rebind success");
        printIPAddress();
        break;

    default:
        //nothing happened
        break;
    }


    /*
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
    */
}

void resetLine(int lineNumber)
{
    lcd.setCursor(0, lineNumber);
    lcd.print("                ");
    lcd.setCursor(0, lineNumber);
}

String getIPAddress()
{
    String address = "";
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        address.concat(String(Ethernet.localIP()[thisByte], DEC));
        if (thisByte < 3)
        {
            address.concat(".");
        }
    }

    return address;
}

void printIPAddress()
{
    String address = getIPAddress();
    Serial.println(address);
    resetLine(1);
    lcd.print(address);
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
