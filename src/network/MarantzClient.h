#ifndef MARANTZ_CLIENT_H
#define MARANTZ_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

struct MarantzStatus {
    bool power;
    bool powerKnown;
    float volume;
    bool hasVolume;
    String input;
    String mode;
    bool isValid;

    MarantzStatus()
        : power(false), powerKnown(false), volume(-80.0), hasVolume(false), input(""), mode(""), isValid(false) {}
};

class MarantzClient {
public:
    static MarantzClient& getInstance() {
        static MarantzClient instance;
        return instance;
    }

    void setReceiverIp(const String& ip);
    MarantzStatus getStatus();
    bool verifyReceiver(const String& ip, MarantzStatus* verifiedStatus = nullptr);

private:
    MarantzClient() : _receiverIp("") {}
    String _receiverIp;

    MarantzStatus fetchStatus(const String& ip);
    String extractValue(const String& xml, const String& tag);
    String extractStatusValue(const String& xml, const String& tag);
    String extractPreferredStatusValue(const String& xml,
                                       const String& tag,
                                       const char* const nestedTags[],
                                       size_t nestedTagCount);
    String extractFirstStatusValue(const String& xml, const char* const tags[], size_t tagCount);
};

#endif
