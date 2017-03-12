#ifndef _MARANTZCLIENT_h
#define _MARANTZCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

const String MARANTZ_STATUS_PATH = "/goform/formMainZone_MainZoneXml.xml";

#include <Ethernet.h>
#include "MicroXPath.h"
#include "IPAddressConverter.h"

class MarantzClientClass
{
 public:
    MarantzClientClass();
    void init(IPAddress address);
    void updateStatistics();
    bool isReceiverOn();
    String getReceiverInput();
    String getReceiverChannels();
    String getReceiverVolume();
 private:
    IPAddress _address;
    EthernetClient _client;
    MicroXPath _xPath;
    bool _receiverOn;
    String _receiverInput;
    String _receiverChannels;
    String _receiverVolume;
};

extern MarantzClientClass MarantzClient;

#endif

