#include "ReceiverDiscovery.h"

#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <algorithm>

void ReceiverDiscovery::start() {
    stop();
    _candidates.clear();
    _state = ReceiverDiscoveryState::Searching;
    _startedAtMs = millis();

    _udpStarted = _udp.begin(0);
    if (!_udpStarted) {
        _state = ReceiverDiscoveryState::Failed;
        return;
    }

    sendSearchRequests();
}

void ReceiverDiscovery::update() {
    if (_state != ReceiverDiscoveryState::Searching) {
        return;
    }

    int packetSize = _udp.parsePacket();
    if (packetSize > 0) {
        String packet;
        packet.reserve(packetSize);
        while (_udp.available()) {
            packet += static_cast<char>(_udp.read());
        }
        parsePacket(packet, _udp.remoteIP());
    }

    if (millis() - _startedAtMs >= DISCOVERY_TIMEOUT_MS) {
        _state = _candidates.empty() ? ReceiverDiscoveryState::TimedOut : ReceiverDiscoveryState::Complete;
    }
}

void ReceiverDiscovery::stop() {
    if (_udpStarted) {
        _udp.stop();
        _udpStarted = false;
    }
    _state = ReceiverDiscoveryState::Idle;
}

ReceiverDiscoveryState ReceiverDiscovery::getState() const {
    return _state;
}

const std::vector<ReceiverCandidate>& ReceiverDiscovery::getCandidates() const {
    return _candidates;
}

bool ReceiverDiscovery::isSearching() const {
    return _state == ReceiverDiscoveryState::Searching;
}

bool ReceiverDiscovery::isValidIpv4(const String& value) {
    if (value.length() < 7 || value.length() > 15) {
        return false;
    }

    int parts = 0;
    int start = 0;
    while (start < value.length()) {
        int dot = value.indexOf('.', start);
        int end = dot == -1 ? value.length() : dot;
        if (end == start || end - start > 3) {
            return false;
        }

        int octet = 0;
        for (int i = start; i < end; ++i) {
            if (!isDigit(value[i])) {
                return false;
            }
            octet = (octet * 10) + (value[i] - '0');
        }
        if (octet > 255) {
            return false;
        }

        parts++;
        if (dot == -1) {
            break;
        }
        start = dot + 1;
    }

    return parts == 4;
}

void ReceiverDiscovery::sendSearchRequests() {
    const char* searchTargets[SEARCH_REQUEST_COUNT] = {
        "ssdp:all",
        "upnp:rootdevice",
        "urn:schemas-upnp-org:device:MediaRenderer:1",
    };

    for (uint8_t i = 0; i < SEARCH_REQUEST_COUNT; ++i) {
        sendSearchRequest(searchTargets[i]);
        delay(20);
    }
}

void ReceiverDiscovery::sendSearchRequest(const char* searchTarget) {
    IPAddress multicastIp(239, 255, 255, 250);
    Serial.print("SSDP search: ");
    Serial.println(searchTarget);
    _udp.beginPacket(multicastIp, SSDP_PORT);
    _udp.print("M-SEARCH * HTTP/1.1\r\n");
    _udp.print("HOST: 239.255.255.250:1900\r\n");
    _udp.print("MAN: \"ssdp:discover\"\r\n");
    _udp.print("MX: 3\r\n");
    _udp.print("ST: ");
    _udp.print(searchTarget);
    _udp.print("\r\n\r\n");
    _udp.endPacket();
}

void ReceiverDiscovery::parsePacket(const String& packet, const IPAddress& remoteIp) {
    Serial.print("SSDP response from ");
    Serial.println(remoteIp.toString());

    bool compatiblePacket = isCompatibleResponse(packet);
    String location = extractHeader(packet, "LOCATION");
    String resolvedName = "";

    if (location.length() > 0) {
        resolvedName = resolveDescriptionName(location);
    }

    if (!compatiblePacket && resolvedName.length() == 0) {
        Serial.print("SSDP ignored ST=");
        Serial.print(extractHeader(packet, "ST"));
        Serial.print(" USN=");
        Serial.print(extractHeader(packet, "USN"));
        Serial.print(" SERVER=");
        Serial.print(extractHeader(packet, "SERVER"));
        Serial.print(" LOCATION=");
        Serial.println(location);
        return;
    }

    String name = resolvedName;
    if (name.length() == 0) {
        name = extractHeader(packet, "SERVER");
    }
    if (name.length() == 0) {
        name = "Unknown Receiver";
    }

    addOrUpdateCandidate(ReceiverCandidate(name, remoteIp.toString(), ReceiverSource::Discovered, millis()));
}

String ReceiverDiscovery::extractHeader(const String& packet, const String& headerName) const {
    String lowerPacket = packet;
    String lowerHeader = headerName;
    lowerPacket.toLowerCase();
    lowerHeader.toLowerCase();

    int lineStart = 0;
    while (lineStart < lowerPacket.length()) {
        int lineEnd = lowerPacket.indexOf('\n', lineStart);
        if (lineEnd == -1) {
            lineEnd = lowerPacket.length();
        }

        String lowerLine = lowerPacket.substring(lineStart, lineEnd);
        lowerLine.trim();
        String prefix = lowerHeader + ":";
        if (lowerLine.startsWith(prefix)) {
            String value = packet.substring(lineStart, lineEnd);
            int colon = value.indexOf(':');
            if (colon >= 0) {
                value = value.substring(colon + 1);
                value.trim();
                return value;
            }
        }

        lineStart = lineEnd + 1;
    }

    return "";
}

String ReceiverDiscovery::extractXmlValue(const String& xml, const String& tagName) const {
    String lowerXml = xml;
    String lowerTagName = tagName;
    lowerXml.toLowerCase();
    lowerTagName.toLowerCase();

    String openTag = "<" + lowerTagName;
    String closeTag = "</" + lowerTagName + ">";
    int start = lowerXml.indexOf(openTag);
    if (start < 0) {
        return "";
    }

    start = lowerXml.indexOf('>', start);
    if (start < 0) {
        return "";
    }
    start++;

    int end = lowerXml.indexOf(closeTag, start);
    if (end < 0) {
        return "";
    }

    String value = xml.substring(start, end);
    value.trim();
    return value;
}

String ReceiverDiscovery::resolveDescriptionName(const String& location) {
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(DESCRIPTION_TIMEOUT_MS);

    if (!http.begin(client, location)) {
        Serial.print("UPnP description begin failed: ");
        Serial.println(location);
        return "";
    }

    int statusCode = http.GET();
    if (statusCode != HTTP_CODE_OK) {
        Serial.print("UPnP description HTTP ");
        Serial.print(statusCode);
        Serial.print(": ");
        Serial.println(location);
        http.end();
        return "";
    }

    String xml = http.getString();
    http.end();

    String friendlyName = extractXmlValue(xml, "friendlyName");
    String manufacturer = extractXmlValue(xml, "manufacturer");
    String modelName = extractXmlValue(xml, "modelName");
    String modelNumber = extractXmlValue(xml, "modelNumber");

    String identity = friendlyName + " " + manufacturer + " " + modelName + " " + modelNumber;
    String lowerIdentity = identity;
    lowerIdentity.toLowerCase();

    bool compatible = lowerIdentity.indexOf("marantz") >= 0 || lowerIdentity.indexOf("denon") >= 0 ||
                      lowerIdentity.indexOf("d&m") >= 0 || lowerIdentity.indexOf("avr") >= 0 ||
                      lowerIdentity.indexOf("sr5010") >= 0;

    Serial.print("UPnP description name=");
    Serial.print(friendlyName);
    Serial.print(" manufacturer=");
    Serial.print(manufacturer);
    Serial.print(" model=");
    Serial.print(modelName);
    Serial.print(" ");
    Serial.println(modelNumber);

    if (!compatible) {
        return "";
    }

    if (friendlyName.length() > 0) {
        return friendlyName;
    }
    if (modelName.length() > 0) {
        return modelName;
    }
    if (modelNumber.length() > 0) {
        return modelNumber;
    }
    return "Marantz Receiver";
}

bool ReceiverDiscovery::isCompatibleResponse(const String& packet) const {
    String lowerPacket = packet;
    lowerPacket.toLowerCase();
    return lowerPacket.indexOf("marantz") >= 0 || lowerPacket.indexOf("denon") >= 0 ||
           lowerPacket.indexOf("avr") >= 0 || lowerPacket.indexOf("sr5010") >= 0;
}

void ReceiverDiscovery::addOrUpdateCandidate(const ReceiverCandidate& candidate) {
    if (!isValidIpv4(candidate.ipAddress)) {
        return;
    }

    auto existing = std::find_if(_candidates.begin(), _candidates.end(), [&](const ReceiverCandidate& item) {
        return item.ipAddress == candidate.ipAddress;
    });

    if (existing != _candidates.end()) {
        existing->name = candidate.name;
        existing->discoveredAtMs = candidate.discoveredAtMs;
        return;
    }

    _candidates.push_back(candidate);
}
