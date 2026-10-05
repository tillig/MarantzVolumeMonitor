#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <atomic>
#include <mutex>
#include <vector>

class WiFiManager {
public:
    static WiFiManager& getInstance() {
        static WiFiManager instance;
        return instance;
    }

    static void begin();
    void update();
    void startConnect(const String& ssid, const String& password);
    void stopConnecting();
    static wl_status_t getConnectStatus();
    uint8_t lastDisconnectReason() const;
    uint32_t disconnectCount() const;
    uint32_t retryCount() const;
    static String disconnectReasonName(uint8_t reason);
    static String describeDisconnectReason(uint8_t reason);
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

    // The Arduino core stops retrying after some failures (for example AUTH_FAIL on a weak signal), so the
    // firmware retries the saved network itself with a growing delay.
    static constexpr uint32_t RetryBaseDelayMs = 20000;
    static constexpr uint32_t RetryMaxDelayMs = 60000;

    std::mutex _mutex;
    String _targetSsid;
    String _targetPassword;
    uint32_t _lastProgressMs = 0;
    uint32_t _retryAttempt = 0;
    std::atomic<uint8_t> _lastDisconnectReason{0};
    std::atomic<uint32_t> _disconnectCount{0};
    std::atomic<uint32_t> _retryCount{0};
    std::atomic<bool> _wasConnected{false};

    void beginConnectAttempt();
    uint32_t retryDelayMs() const;
    void handleEvent(arduino_event_id_t event, arduino_event_info_t info);
};

#endif
