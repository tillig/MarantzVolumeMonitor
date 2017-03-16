#ifndef _MARANTZCLIENT_h
#define _MARANTZCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

const char MARANTZ_STATUS_PATH[] = "/goform/formMainZone_MainZoneXml.xml";
#define XML_ELEMENT_NAME_LENGTH 24
#define RECEIVER_INPUT_MAXLENGTH 9
#define RECEIVER_CHANNELS_MAXLENGTH 16

#include <Ethernet.h>
#include "IPAddressConverter.h"

class MarantzClientClass
{
 public:
    MarantzClientClass();
    void init(IPAddress address);
    void updateStatistics();
    bool isReceiverOn();
    char* getReceiverInput();
    char* getReceiverChannels();
    String getReceiverVolume();
 private:
    IPAddress _address;
    EthernetClient _client;
    bool _receiverOn;
    char _receiverInput[RECEIVER_INPUT_MAXLENGTH + 1];
    char _receiverChannels[RECEIVER_CHANNELS_MAXLENGTH + 1];
    String _receiverVolume;
    void readToContentStart();
    void readElement(char* name);
    void readValue(char * value, int maxlength);
};

extern MarantzClientClass MarantzClient;

#endif

