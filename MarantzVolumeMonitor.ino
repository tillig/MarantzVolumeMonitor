#include "MarantzClient.h"
#include "ConfigurationManager.h"
#include "ConfigurationInterface.h"
#include "IPAddressConverter.h"
#include "ButtonManager.h"
#include "DFRobotLCDShield.h"
#include "DisplayManager.h"
#include "MarantzClient.h"
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
EthernetClient client;
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
    receiverAddress = ConfigurationManager.getConfiguration();
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

    if (receiverAddress == INADDR_NONE)
    {
        Serial.println("No configured receiver address found.");
        setupShouldRun = true;
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
        ConfigurationManager.setConfiguration(receiverAddress);
    }
    else
    {
        Serial.println("Skipping setup");
    }

    MarantzClient.init(receiverAddress);
    Serial.print("Receiver IP address: ");
    Serial.println(IPAddressConverter.toString(receiverAddress));
    DisplayManager.showMessage("Receiver IP:", IPAddressConverter.toString(receiverAddress));
    delay(2000);
}

void loop()
{
    MarantzClient.updateStatistics();
    receiverOn = MarantzClient.isReceiverOn();
    if (receiverOn)
    {
        if (!DisplayManager.isDisplayEnabled())
        {
            DisplayManager.enableDisplay();
        }

        // The receiver is on, so get the updated
        // values to display and refresh.
        DisplayManager.showStats(MarantzClient.getReceiverVolume(), MarantzClient.getReceiverInput(), MarantzClient.getReceiverChannels());

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
