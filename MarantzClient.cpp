#include "MarantzClient.h"

MarantzClientClass MarantzClient;

MarantzClientClass::MarantzClientClass()
{
    _client = EthernetClient();
    _client.setTimeout(500);
}

void MarantzClientClass::init(IPAddress address)
{
    _address = address;
}

void MarantzClientClass::updateStatistics()
{
    if (!_client.connected())
    {
        // If something goes awry and the client is still connected,
        // initiating another connection hangs the whole thing.
        if (_client.connect(_address, 80))
        {
            Serial.print("Connected to ");
            Serial.println(IPAddressConverter.toString(_address));
            _client.print("GET ");
            _client.print(MARANTZ_STATUS_PATH);
            _client.println(" HTTP/1.1");
            _client.print("Host: ");
            _client.println(IPAddressConverter.toString(_address));
            _client.println("Connection: close");
            _client.println();
        }
        else
        {
            Serial.print("Unable to connect to receiver at ");
            Serial.println(IPAddressConverter.toString(_address));
            _client.stop();
            _client.flush();
            _receiverOn = false;
            return;
        }
    }

    if (_client.connected())
    {
        char elementName[XML_ELEMENT_NAME_LENGTH + 1];
        this->readToContentStart();

        while (strcmp("item", elementName) != 0)
        {
            // Read the opening "item" tag.
            this->readElement(elementName);
        }

        bool finished = false;
        while (!finished && (_client.connected() || _client.available()))
        {
            this->readElement(elementName);
            if (strcmp("ZonePower", elementName) == 0)
            {
                Serial.println("ZonePower");
            }
            else if (strcmp("InputFuncSelect", elementName) == 0)
            {
                Serial.println("InputFuncSelect");
            }
            if (strcmp("selectSurround", elementName) == 0)
            {
                Serial.println("selectSurround");
            }
            if (strcmp("MasterVolume", elementName) == 0)
            {
                Serial.println("MasterVolume");
            }
            if (strcmp("Mute", elementName) == 0)
            {
                Serial.println("Mute");
            }
        }
    }

    _client.stop();
    _client.flush();

    Serial.println();
    Serial.println("Finished processing XML.");

    // STUBS!
    _receiverOn = random(1, 100) > 20;

    if (random(1, 100) % 2 == 0)
    {
        _receiverInput = "Roku";
    }
    else
    {
        _receiverInput = "XboxOneSuperLong";
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

void MarantzClientClass::readElement(char* name)
{
    bool matchedOpenBracket = false;
    bool readToElementEnd = false;
    int index = 0;

    while (_client.available() || _client.connected())
    {
        if (_client.available())
        {
            char c = _client.read();
            if (c == '<')
            {
                // We hit the beginning of a tag
                // which may be an opener or a closer.
                matchedOpenBracket = true;
                memset(name, 0, XML_ELEMENT_NAME_LENGTH + 1);
                readToElementEnd = false;
                index = 0;
                continue;
            }

            if (!matchedOpenBracket)
            {
                // Read until we make it to an open bracket;
                continue;
            }

            if (c == '/')
            {
                if (index == 0)
                {
                    // This is the first character since reading an open
                    // bracket so this is a closing tag:
                    // </SomeTag>
                    // Reset and read until we get to an open tag.
                    memset(name, 0, XML_ELEMENT_NAME_LENGTH + 1);
                    index = 0;
                    matchedOpenBracket = false;
                }

                // Allow reading to the close bracket.
                continue;
            }

            if (c == ' ')
            {
                // You can't have a space in an element
                // name so let's just enable reading through
                // to the end.
                readToElementEnd = true;
            }

            if (c == '>')
            {
                if (matchedOpenBracket)
                {
                    // This ended an opening tag, so we're done!
                    return;
                }

                // This ended a closing tag, so continue reading
                // until we find a start element.
                continue;
            }

            if (index >= XML_ELEMENT_NAME_LENGTH)
            {
                // We haven't hit the end but we have enough
                // so just keep reading until we hit the end.
                readToElementEnd = true;
            }

            if (readToElementEnd)
            {
                // We need to just get to the closing bracket.
                continue;
            }

            name[index++] = c;
        }
    }
}

void MarantzClientClass::readToContentStart()
{
    while (_client.available() || _client.connected())
    {
        if (_client.available())
        {
            if (_client.read() == '<')
            {
                break;
            }
        }
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
