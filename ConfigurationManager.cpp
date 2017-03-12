#include "ConfigurationManager.h"

ConfigurationManagerClass ConfigurationManager;

ConfigurationManagerClass::ConfigurationManagerClass()
{
    _settings.version = CONFIG_VERSION;
    _settings.receiverAddress[0] = 0;
    _settings.receiverAddress[1] = 0;
    _settings.receiverAddress[2] = 0;
    _settings.receiverAddress[3] = 0;
}

IPAddress ConfigurationManagerClass::getConfiguration()
{
    if (EEPROM.read(CONFIG_EEPROM_ADDRESS) != CONFIG_VERSION)
    {
        return INADDR_NONE;
    }

    _settings = EEPROM.get(CONFIG_EEPROM_ADDRESS, _settings);
    return IPAddress(_settings.receiverAddress[0], _settings.receiverAddress[1], _settings.receiverAddress[2], _settings.receiverAddress[3]);
}

void ConfigurationManagerClass::setConfiguration(IPAddress address)
{
    for (byte i = 0; i < 4; i++)
    {
        _settings.receiverAddress[i] = address[i];
    }

    EEPROM.put(CONFIG_EEPROM_ADDRESS, _settings);
}
