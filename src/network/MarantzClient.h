#ifndef MARANTZ_CLIENT_H
#define MARANTZ_CLIENT_H

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFi.h>

struct MarantzStatus {
    bool power;
    float volume;
    String input;
    String mode;
    bool isValid;

    MarantzStatus() : power(false), volume(-80.0), input(""), mode(""), isValid(false) {}
};

class MarantzClient {
public:
    static MarantzClient& getInstance() {
        static MarantzClient instance;
        return instance;
    }

    void setReceiverIp(const String& ip);
    MarantzStatus getStatus();

private:
    MarantzClient() : _receiverIp("") {}
    String _receiverIp;

    String extractValue(const String& xml, const String& tag);
};

#endif
