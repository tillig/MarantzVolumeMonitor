#include "IPAddressConverter.h"

IPAddressConverterClass IPAddressConverter;

String IPAddressConverterClass::toString(IPAddress ip)
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

String IPAddressConverterClass::toPaddedString(IPAddress ip)
{
    String address = "";
    for (byte thisByte = 0; thisByte < 4; thisByte++) {
        String segment = String(ip[thisByte], DEC);
        String prepend = "";
        for (byte toPrepend = 3; toPrepend > segment.length(); toPrepend--)
        {
            prepend.concat("0");
        }
        address.concat(prepend);
        address.concat(segment);
        if (thisByte < 3)
        {
            address.concat(".");
        }
    }

    return address;
}

String IPAddressConverterClass::segmentToPaddedString(IPAddress ip, int segmentIndex)
{
    String segment = String(ip[segmentIndex], DEC);
    String prepend = "";
    for (byte toPrepend = 3; toPrepend > segment.length(); toPrepend--)
    {
        prepend.concat("0");
    }

    return prepend + segment;
}
