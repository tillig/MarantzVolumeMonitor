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

    void startConnect(const String& ssid, const String& password);
    wl_status_t getConnectStatus();
    bool connect(const String& ssid, const String& password);
    bool isConnected();
    String getIPAddress();
    int32_t getSignalStrength();
    static uint8_t signalLevelForRssi(int32_t rssi);

    struct NetworkInfo {
        String ssid;
        int32_t rssi;
        uint8_t encryptionType;
    };

    void startScan();
    int16_t getScanStatus();
    std::vector<NetworkInfo> getScanResults();
    std::vector<NetworkInfo> scanNetworks(); // Existing for backward compatibility if needed

private:
    WiFiManager() {}
};

#endif
