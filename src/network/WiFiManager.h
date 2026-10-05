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

    static void startConnect(const String& ssid, const String& password);
    static wl_status_t getConnectStatus();
    static bool connect(const String& ssid, const String& password);
    static bool isConnected();
    static String getIPAddress();
    static int32_t getSignalStrength();
    static uint8_t signalLevelForRssi(int32_t rssi);

    struct NetworkInfo {
        String ssid;
        int32_t rssi;
        uint8_t encryptionType;
    };

    static void startScan();
    static int16_t getScanStatus();
    static std::vector<NetworkInfo> getScanResults();
    static std::vector<NetworkInfo> scanNetworks(); // Existing for backward compatibility if needed

private:
    WiFiManager() {}
};

#endif
