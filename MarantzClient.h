#ifndef _MARANTZCLIENT_h
#define _MARANTZCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

const char MARANTZ_STATUS_PATH[] = "/goform/formMainZone_MainZoneXml.xml";
#define XML_ELEMENT_NAME_LENGTH 24

#include <Ethernet.h>
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
    bool _receiverOn;
    String _receiverInput;
    String _receiverChannels;
    String _receiverVolume;
    void readToContentStart();
    void readElement(char* name);
};

extern MarantzClientClass MarantzClient;

#endif

