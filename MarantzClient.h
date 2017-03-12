#ifndef _MARANTZCLIENT_h
#define _MARANTZCLIENT_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include <Ethernet.h>

class MarantzClientClass
{
 public:
    void init(IPAddress address);
    void updateStatistics();
    bool isReceiverOn();
    String getReceiverInput();
    String getReceiverChannels();
    String getReceiverVolume();
 private:
    IPAddress _address;
    bool _receiverOn;
    String _receiverInput;
    String _receiverChannels;
    String _receiverVolume;
};

extern MarantzClientClass MarantzClient;

#endif

