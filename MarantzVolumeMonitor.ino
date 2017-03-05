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
LiquidCrystal lcd(LCD_RS, LCD_ENABLE, LCD_DB4, LCD_DB5, LCD_DB6, LCD_DB7);
bool receiverOn;

// For discovery...
char packetBuffer[1024];
int discoveryPort = 1900;
int localPort = 8888;
IPAddress discoveryAddress(239, 255, 255, 250);
EthernetUDP Udp;

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

    bool runSetup = false;
    DisplayManager.showMessage("Push SELECT to", "run setup");
    unsigned long currentMillis = millis();
    while (millis() - currentMillis < 5000)
    {
        if (getButton() == BUTTON_SELECT)
        {
            Serial.println("User pushed SELECT to start setup");
            runSetup = true;
            break;
        }
    }

    if (!runSetup)
    {
        // TODO: Read saved IP address of Marantz receiver.
        // TODO: If there is no saved IP address, initiate setup.
    }

    if (runSetup)
    {
        Serial.println("Initiating setup");
        DisplayManager.showMessage("Running Setup");
        delay(1000);
        runDiscovery();
    }
    else
    {
        Serial.println("Skipping setup");
    }
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

void runDiscovery()
{
    // https://tkkrlab.nl/wiki/Arduino_UDP_multicast
    Serial.println("Sending multicast UPnP search request.");
    int success = 0;
    success = Udp.beginMulticast(discoveryAddress, localPort);
    Serial.print("beginMulticast: ");
    Serial.println(success);
    success = Udp.beginPacket(discoveryAddress, discoveryPort);
    Serial.print("beginPacket: ");
    Serial.println(success);
    success = Udp.write("M-SEARCH * HTTP/1.1\r\nContent-Length: 0\r\nST: urn:schemas-upnp-org:device:MediaRenderer:1\r\nMX: 3\r\nMAN: \"ssdp:discover\"\r\nHOST: 239.255.255.250:1900\r\n");
    Serial.print("write: ");
    Serial.println(success);
    success = Udp.endPacket();
    Serial.print("endPacket: ");
    Serial.println(success);
    Serial.println("Search sent. Waiting for response.");
    unsigned long currentMillis = millis();
    int packetSize;
    while (millis() - currentMillis < 3000)
    {
        packetSize = 0;
        packetSize = Udp.parsePacket();
        //int n = 0;
        if (packetSize)
        {
            Serial.print("Received packet of size ");
            Serial.println(packetSize);
            Serial.print("From ");
            Serial.print(getIPAddressString(Udp.remoteIP()));
            Serial.print(", port ");
            Serial.println(Udp.remotePort());
            /*

            // read the packet into packetBufffer
            n = Udp.read(packetBuffer, 1024);
            packetBuffer[n] = 0;

            Serial.println("Contents:");
            Serial.println(packetBuffer);
            */
        }
    }

    Serial.println("Done processing responses.");

    //memset(packetBuffer, 0, 1024);

/*
M-SEARCH * HTTP/1.1
Content-Length: 0
ST: urn:schemas-upnp-org:device:MediaRenderer:1
MX: 3
MAN: "ssdp:discover"
HOST: 239.255.255.250:1900
*/
    // TODO: Send UDP search to get UPnP devices.
    // TODO: Find Marantz receivers.
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
