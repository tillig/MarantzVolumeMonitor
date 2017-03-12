#include "MarantzClient.h"

MarantzClientClass MarantzClient;

MarantzClientClass::MarantzClientClass()
{
    _xPath = MicroXPath();
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
            return;
        }
    }

    bool read = false;
    String response = "";
    // TODO: client.connected seems to return false too soon. Why is it disconnecting so early?
    while (_client.available() || _client.connected())
    {
        if (_client.available())
        {
            char c = _client.read();
            if (!read && c == '<')
            {
                read = true; //Ready to start reading the part 
            }
            if (read)
            {
                response.concat(c);
            }
        }
    }

    Serial.print("Client available: ");
    Serial.println(_client.available());
    Serial.print("Client connected: ");
    Serial.println(_client.connected());

    _client.stop();
    _client.flush();
    Serial.println(response);

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
