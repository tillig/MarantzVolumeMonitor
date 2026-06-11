#ifndef CURRENT_SETTINGS_SCREEN_H
#define CURRENT_SETTINGS_SCREEN_H

#include "Screen.h"
#include "../MaterialStyle.h"
#include "../../network/MarantzClient.h"
#include "../../storage/ConfigStore.h"

class CurrentSettingsScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    static constexpr uint32_t RefreshIntervalMs = 1000;

    DeviceConfig _config;
    bool _initialized = false;
    bool _wifiConnected = false;
    String _monitorIpAddress;
    bool _hasWifiSignal = false;
    int32_t _wifiRssi = 0;
    String _receiverIdentity;
    String _identityIpAddress;
    bool _receiverIdentityAttempted = false;
    MarantzStatus _receiverStatus;
    uint32_t _lastRefreshAtMs = 0;

    void refreshState();
    void drawDynamicContent(TFT_eSPI& tft) const;
    void clearCardBody(TFT_eSPI& tft, int cardY, int cardHeight) const;
    void drawWifiSummary(TFT_eSPI& tft) const;
    void drawReceiverSummary(TFT_eSPI& tft) const;
    void drawSummaryLine(TFT_eSPI& tft, int x, int y, int maxWidth, const String& text,
                         MaterialStyle::ComponentState state,
                         uint8_t datum = TL_DATUM) const;
    String wifiHeadlineText() const;
    String wifiDetailText() const;
    String receiverHeadlineText() const;
    String receiverDetailText() const;
    String receiverStatusText() const;
    int wifiSignalLevel() const;
    MaterialStyle::ComponentState wifiHeadlineState() const;
    MaterialStyle::ComponentState wifiDetailState() const;
    MaterialStyle::ComponentState receiverHeadlineState() const;
    MaterialStyle::ComponentState receiverDetailState() const;
    MaterialStyle::ComponentState receiverStatusState() const;
    bool isOkPressed(TS_Point p) const;
};

#endif
