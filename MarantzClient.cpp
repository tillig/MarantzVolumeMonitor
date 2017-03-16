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
    _receiverOn = false;
    memset(_receiverInput, 0, RECEIVER_INPUT_MAXLENGTH + 1);
    memset(_receiverChannels, 0, RECEIVER_CHANNELS_MAXLENGTH + 1);

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
            return;
        }
    }

    bool mute = false;
    float volume = 0.0;
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
            char powerValue[3];
            memset(powerValue, 0, 3);
            this->readValue(powerValue, 2);
            _receiverOn = strcasecmp("ON", powerValue) == 0;
            Serial.print("ZonePower: ");
            Serial.print(powerValue);
            Serial.print(" (");
            Serial.print(_receiverOn);
            Serial.println(")");
        }
        else if (strcmp("InputFuncSelect", elementName) == 0)
        {
            this->readValue(_receiverInput, RECEIVER_INPUT_MAXLENGTH);
            Serial.print("InputFuncSelect: ");
            Serial.println(_receiverInput);
        }
        if (strcmp("selectSurround", elementName) == 0)
        {
            this->readValue(_receiverChannels, RECEIVER_CHANNELS_MAXLENGTH);
            Serial.print("selectSurround: ");
            Serial.println(_receiverChannels);
        }
        if (strcmp("MasterVolume", elementName) == 0)
        {
            char volValue[8];
            memset(volValue, 0, 8);
            this->readValue(volValue, 7);
            volume = float(int(String(volValue).toFloat() * 10) / 10.0);
            Serial.print("MasterVolume: ");
            Serial.print(volValue);
            Serial.print(" (");
            Serial.print(volume);
            Serial.println(")");
        }
        if (strcmp("Mute", elementName) == 0)
        {
            char muteValue[3];
            memset(muteValue, 0, 3);
            this->readValue(muteValue, 2);
            _receiverOn = strcasecmp("on", muteValue) == 0;
            Serial.print("Mute: ");
            Serial.print(muteValue);
            Serial.print(" (");
            Serial.print(mute);
            Serial.println(")");
        }
    }

    _client.stop();
    _client.flush();

    if (mute)
    {
        _receiverVolume = "MUTE";
    }
    else
    {
        _receiverVolume = String(volume + 80.0, 1);
    }

    Serial.print("Resolved volume: ");
    Serial.println(_receiverVolume);
    Serial.println("Finished processing XML.");
    Serial.println();
}

void MarantzClientClass::readValue(char *value, int maxlength)
{
    char elementName[XML_ELEMENT_NAME_LENGTH + 1];
    memset(elementName, 0, XML_ELEMENT_NAME_LENGTH + 1);
    while (strcmp("value", elementName) != 0)
    {
        // Read the opening "value" tag.
        this->readElement(elementName);
    }

    int index = 0;
    while ((_client.available() || _client.connected()) && index < maxlength)
    {
        if (_client.available())
        {
            char c = _client.read();
            if (c != '<')
            {
                value[index++] = c;
            }
            else
            {
                break;
            }
        }
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

char* MarantzClientClass::getReceiverInput()
{
    return _receiverInput;
}

char* MarantzClientClass::getReceiverChannels()
{
    return _receiverChannels;
}

String MarantzClientClass::getReceiverVolume()
{
    return _receiverVolume;
}
