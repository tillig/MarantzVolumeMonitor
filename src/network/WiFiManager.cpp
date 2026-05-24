#include "WiFiManager.h"

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

std::vector<WiFiManager::NetworkInfo> WiFiManager::scanNetworks() {
    std::vector<NetworkInfo> networks;
    int n = WiFi.scanNetworks();
    for (int i = 0; i < n; ++i) {
        networks.push_back({WiFi.SSID(i), WiFi.RSSI(i), WiFi.encryptionType(i)});
    }
    return networks;
}
