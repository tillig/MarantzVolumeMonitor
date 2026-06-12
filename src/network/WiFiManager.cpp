#include "WiFiManager.h"

#include <algorithm>
#include <iterator>
#include <map>

void WiFiManager::startConnect(const String& ssid, const String& password) {
    WiFi.begin(ssid.c_str(), password.c_str());
}

wl_status_t WiFiManager::getConnectStatus() {
    return WiFi.status();
}

bool WiFiManager::connect(const String& ssid, const String& password) {
    WiFi.begin(ssid.c_str(), password.c_str());

    int counter = 0;
    while (WiFi.status() != WL_CONNECTED && counter < 20) {
        delay(500);
        Serial.print(".");
        counter++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nWiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        return true;
    } else {
        Serial.println("\nWiFi connection failed");
        return false;
    }
}

bool WiFiManager::isConnected() {
    return WiFi.status() == WL_CONNECTED;
}

String WiFiManager::getIPAddress() {
    return WiFi.localIP().toString();
}

int32_t WiFiManager::getSignalStrength() {
    return WiFi.RSSI();
}

uint8_t WiFiManager::signalLevelForRssi(int32_t rssi) {
    if (rssi < -80) {
        return 1;
    }
    if (rssi < -70) {
        return 2;
    }
    return 3;
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::getScanResults() {
    std::vector<NetworkInfo> networks;
    int n = WiFi.scanComplete();
    if (n > 0) {
        // Use a map to deduplicate by SSID, keeping the strongest RSSI
        std::map<String, NetworkInfo> deduped;

        for (int i = 0; i < n; ++i) {
            String ssid = WiFi.SSID(i);
            int32_t rssi = WiFi.RSSI(i);
            uint8_t enc = WiFi.encryptionType(i);

            if (deduped.find(ssid) == deduped.end() || rssi > deduped[ssid].rssi) {
                deduped[ssid] = {ssid, rssi, enc};
            }
        }

        networks.reserve(deduped.size());
        std::transform(deduped.begin(),
                       deduped.end(),
                       std::back_inserter(networks),
                       [](const std::pair<const String, NetworkInfo>& entry) { return entry.second; });

        // Sort by RSSI (strongest first)
        std::sort(networks.begin(), networks.end(), [](const NetworkInfo& a, const NetworkInfo& b) {
            return a.rssi > b.rssi;
        });

        WiFi.scanDelete();
    }
    return networks;
}

void WiFiManager::startScan() {
    WiFi.scanNetworks(true);
}

int16_t WiFiManager::getScanStatus() {
    return WiFi.scanComplete();
}

std::vector<WiFiManager::NetworkInfo> WiFiManager::scanNetworks() {
    std::vector<NetworkInfo> networks;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        networks.push_back({WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i)});
    }
    return networks;
}
