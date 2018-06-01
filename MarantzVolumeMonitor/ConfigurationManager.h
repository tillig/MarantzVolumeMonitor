#ifndef _CONFIGURATIONMANAGER_h
#define _CONFIGURATIONMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include <EEPROM.h>
#include <Ethernet.h>
#include "ConfigurationSettings.h"

#define CONFIG_VERSION 1
#define CONFIG_EEPROM_ADDRESS 0

class ConfigurationManagerClass
{
 public:
     ConfigurationManagerClass();
     IPAddress getConfiguration();
     void setConfiguration(IPAddress address);
 private:
    int _configurationAddress;
    ConfigurationSettings _settings;
};

extern ConfigurationManagerClass ConfigurationManager;

#endif

