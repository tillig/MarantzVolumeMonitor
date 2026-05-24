#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <vector>

class WiFiManager {
public:
    static WiFiManager& getInstance() {
        static WiFiManager instance;
        return instance;
    }

    bool connect(const String& ssid, const String& password);
    bool isConnected();
    String getIPAddress();

    struct NetworkInfo {
        String ssid;
        int32_t rssi;
        uint8_t encryptionType;
    };

    std::vector<NetworkInfo> scanNetworks();

private:
    WiFiManager() {}
};

#endif
