#ifndef _CONFIGURATIONINTERFACE_h
#define _CONFIGURATIONINTERFACE_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include <Ethernet.h>
#include <LiquidCrystal.h>
#include "DisplayManager.h"
#include "ButtonManager.h"
#include "DFRobotLCDShield.h"
#include "IPAddressConverter.h"

class ConfigurationInterfaceClass
{
 public:
    void init(DisplayManagerClass* displayManager);
    IPAddress readConfigurationFromUser(IPAddress start);
 protected:
    IPAddress incrementSegment(byte cursorPosition, IPAddress address);
    IPAddress decrementSegment(byte cursorPosition, IPAddress address);
 private:
    DisplayManagerClass* _displayManager;
    IPAddress changeSegment(byte cursorPosition, IPAddress address, int quantity);
    int getAmountToChangeSegment(byte cursorPosition);
    IPAddress runUserInputLoop(IPAddress start);
    void updateAddressEntryLine(byte cursorPosition, IPAddress address);
};

extern ConfigurationInterfaceClass ConfigurationInterface;

#endif

