// 
// 
// 

#include "MarantzClient.h"

MarantzClientClass MarantzClient;

void MarantzClientClass::init(IPAddress address)
{
    _address = address;
}

void MarantzClientClass::updateStatistics()
{
    // STUBS!
    _receiverOn = random(1, 100) > 20;

    if (random(1, 100) % 2 == 0)
    {
        _receiverInput = "Roku";
    }
    else
    {
        _receiverInput =  "XboxOneSuperLong";
    }

    if (random(1, 100) % 2 == 0)
    {
        _receiverChannels = "Stereo";
    }
    else
    {
        _receiverChannels = "Multi Ch Input 7.1";
    }

    if (random(1, 100) % 5 == 0)
    {
        _receiverVolume = "MUTE";
    }
    else
    {
        float roundedVol = float(int((random(0, 1000) / 10.0) * 10) / 10.0);
        _receiverVolume = String(roundedVol, 1);
    }
}

bool MarantzClientClass::isReceiverOn()
{
    return _receiverOn;
}

String MarantzClientClass::getReceiverInput()
{
    return _receiverInput;
}

String MarantzClientClass::getReceiverChannels()
{
    return _receiverChannels;
}

String MarantzClientClass::getReceiverVolume()
{
    return _receiverVolume;
}
