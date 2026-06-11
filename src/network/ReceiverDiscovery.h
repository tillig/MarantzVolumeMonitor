#ifndef RECEIVER_DISCOVERY_H
#define RECEIVER_DISCOVERY_H

#include <Arduino.h>
#include <WiFiUdp.h>
#include <vector>

enum class ReceiverSource {
    Discovered,
    Manual
};

enum class ReceiverDiscoveryState {
    Idle,
    Searching,
    Complete,
    TimedOut,
    Failed
};

enum class ReceiverFailureReason {
    None,
    InvalidAddress,
    Timeout,
    NoStatus,
    NetworkUnavailable,
    SaveFailed
};

struct ReceiverCandidate {
    String name;
    String ipAddress;
    ReceiverSource source;
    uint32_t discoveredAtMs;

    ReceiverCandidate()
        : name(""), ipAddress(""), source(ReceiverSource::Discovered), discoveredAtMs(0) {}

    ReceiverCandidate(const String& receiverName, const String& ip, ReceiverSource receiverSource, uint32_t discoveredAt)
        : name(receiverName), ipAddress(ip), source(receiverSource), discoveredAtMs(discoveredAt) {}
};

struct ReceiverVerificationResult {
    bool success;
    String ipAddress;
    bool statusValid;
    ReceiverFailureReason failureReason;

    ReceiverVerificationResult()
        : success(false), ipAddress(""), statusValid(false), failureReason(ReceiverFailureReason::None) {}
};

class ReceiverDiscovery {
public:
    static ReceiverDiscovery& getInstance() {
        static ReceiverDiscovery instance;
        return instance;
    }

    void start();
    void update();
    void stop();
    ReceiverDiscoveryState getState() const;
    const std::vector<ReceiverCandidate>& getCandidates() const;
    bool isSearching() const;
    String resolveReceiverIdentity(const String& ipAddress);
    static bool isValidIpv4(const String& value);

private:
    ReceiverDiscovery() {}

    static constexpr uint16_t SSDP_PORT = 1900;
    static constexpr uint32_t DISCOVERY_TIMEOUT_MS = 15000;
    static constexpr uint16_t DESCRIPTION_TIMEOUT_MS = 750;
    static constexpr uint32_t RECEIVE_WINDOW_MS = 8;
    static constexpr uint8_t SEARCH_REQUEST_COUNT = 3;

    WiFiUDP _udp;
    ReceiverDiscoveryState _state = ReceiverDiscoveryState::Idle;
    std::vector<ReceiverCandidate> _candidates;
    uint32_t _startedAtMs = 0;
    bool _udpStarted = false;

    void sendSearchRequests();
    void sendSearchRequest(const char* searchTarget);
    void parsePacket(const String& packet, const IPAddress& remoteIp);
    String extractHeader(const String& packet, const String& headerName) const;
    String extractXmlValue(const String& xml, const String& tagName) const;
    String resolveDescriptionName(const String& location);
    bool isCompatibleResponse(const String& packet) const;
    void addOrUpdateCandidate(const ReceiverCandidate& candidate);
};

#endif
