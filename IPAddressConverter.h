#ifndef _IPADDRESSCONVERTER_h
#define _IPADDRESSCONVERTER_h

#if defined(ARDUINO) && ARDUINO >= 100
    #include "arduino.h"
#else
    #include "WProgram.h"
#endif

#include <Ethernet.h>

class IPAddressConverterClass
{
 public:
     String toString(IPAddress ip);
     String toPaddedString(IPAddress ip);
     String segmentToPaddedString(IPAddress ip, int segmentIndex);
};

extern IPAddressConverterClass IPAddressConverter;

#endif

