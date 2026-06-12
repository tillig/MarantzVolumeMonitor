#include "CurrentSettingsScreen.h"

#include "SettingsScreen.h"
#include "../IconRenderer.h"
#include "../MaterialStyle.h"
#include "../ScreenManager.h"
#include "../assets/IconBitmaps.h"
#include "../../network/ReceiverDiscovery.h"
#include "../../network/WiFiManager.h"

namespace {
constexpr int CardX = 20;
constexpr int CardWidth = 440;
constexpr int WifiCardY = MaterialStyle::SetupListTopY;
constexpr int WifiCardHeight = 86;
constexpr int ReceiverCardY = 164;
constexpr int ReceiverCardHeight = 86;
constexpr int CardBodyInsetX = 16;
constexpr int CardBodyTopInset = 34;
constexpr int CardBodyBottomInset = 10;
constexpr int SummaryLine1Y = 48;
constexpr int SummaryLine2Y = 68;
constexpr int SummaryTextX = CardX + CardBodyInsetX;
constexpr int SummaryTextRightX = CardX + CardWidth - CardBodyInsetX;
constexpr int WifiSignalCenterX = CardX + 30;
constexpr int WifiSignalTextX = CardX + 52;
constexpr int ReceiverStatusGap = 12;

void drawWifiSignal(TFT_eSPI& tft, int centerX, int centerY, int level) {
    int x = centerX - 12;
    int baseY = centerY + 9;
    uint16_t active = level >= 3   ? DisplayManager::COLOR_ICON_ACTIVE
                      : level == 2 ? DisplayManager::COLOR_WARNING
                                   : DisplayManager::COLOR_ERROR;
    for (int i = 0; i < 3; ++i) {
        int h = 8 + (i * 5);
        uint16_t color = i < level ? active : DisplayManager::COLOR_BAR_BG;
        tft.fillRoundRect(x + (i * 8), baseY - h, 6, h, 3, color);
    }
}
} // namespace

void CurrentSettingsScreen::draw() {
    if (!_initialized) {
        refreshState();
    }

    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawPageHeader(tft, Icons::SETTINGS, "Current Settings", "View saved and live monitor status");

    MaterialStyle::drawInfoCard(tft, CardX, WifiCardY, CardWidth, WifiCardHeight, Icons::WIFI, "Wi-Fi");
    MaterialStyle::drawInfoCard(tft, CardX, ReceiverCardY, CardWidth, ReceiverCardHeight, Icons::RECEIVER, "Receiver");

    drawDynamicContent(tft);

    MaterialStyle::drawStandardButton(tft,
                                      182,
                                      MaterialStyle::BottomActionY,
                                      116,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "OK",
                                      MaterialStyle::ComponentState::Success);
}

void CurrentSettingsScreen::update() {
    if (!_initialized || millis() - _lastRefreshAtMs >= RefreshIntervalMs) {
        refreshState();
        drawDynamicContent(DisplayManager::getInstance().getTft());
    }
}

void CurrentSettingsScreen::handleTouch(TS_Point p) {
    if (isOkPressed(p)) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void CurrentSettingsScreen::refreshState() {
    DeviceConfig refreshedConfig;
    ConfigStore::getInstance().loadConfig(refreshedConfig);

    if (refreshedConfig.receiverIp != _identityIpAddress) {
        _receiverIdentity.clear();
        _identityIpAddress = refreshedConfig.receiverIp;
        _receiverIdentityAttempted = false;
    }

    _config = refreshedConfig;
    _wifiConnected = WiFiManager::getInstance().isConnected();
    _monitorIpAddress = _wifiConnected ? WiFiManager::getInstance().getIPAddress() : "";
    _hasWifiSignal = _wifiConnected;
    _wifiRssi = _hasWifiSignal ? WiFiManager::getInstance().getSignalStrength() : 0;

    _receiverStatus = MarantzStatus();
    if (_wifiConnected && _config.receiverIp.length() > 0) {
        MarantzClient::getInstance().setReceiverIp(_config.receiverIp);
        _receiverStatus = MarantzClient::getInstance().getStatus();
    }

    if (_wifiConnected && _config.receiverIp.length() > 0 && !_receiverIdentityAttempted) {
        _receiverIdentity = ReceiverDiscovery::getInstance().resolveReceiverIdentity(_config.receiverIp);
        _receiverIdentityAttempted = true;
    }

    _initialized = true;
    _lastRefreshAtMs = millis();
}

void CurrentSettingsScreen::drawDynamicContent(TFT_eSPI& tft) const {
    clearCardBody(tft, WifiCardY, WifiCardHeight);
    clearCardBody(tft, ReceiverCardY, ReceiverCardHeight);
    drawWifiSummary(tft);
    drawReceiverSummary(tft);
}

void CurrentSettingsScreen::clearCardBody(TFT_eSPI& tft, int cardY, int cardHeight) const {
    tft.fillRect(CardX + 8,
                 cardY + CardBodyTopInset,
                 CardWidth - 16,
                 cardHeight - CardBodyTopInset - CardBodyBottomInset,
                 DisplayManager::COLOR_PANEL);
}

void CurrentSettingsScreen::drawWifiSummary(TFT_eSPI& tft) const {
    int line1Y = WifiCardY + SummaryLine1Y;
    int line2Y = WifiCardY + SummaryLine2Y;

    if (_config.wifiSsid.length() == 0) {
        drawSummaryLine(tft,
                        SummaryTextX,
                        line1Y,
                        CardWidth - (CardBodyInsetX * 2),
                        "Unconfigured",
                        MaterialStyle::ComponentState::Warning,
                        TL_DATUM);
        return;
    }

    if (_hasWifiSignal) {
        drawWifiSignal(tft, WifiSignalCenterX, line1Y - 2, wifiSignalLevel());

        String signalText = String(_wifiRssi) + " dBm";
        tft.setTextDatum(TL_DATUM);
        tft.setTextColor(MaterialStyle::textColorFor(MaterialStyle::TextRole::Body), DisplayManager::COLOR_PANEL);
        tft.drawString(signalText, WifiSignalTextX, line1Y, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));

        int ssidX =
            WifiSignalTextX + tft.textWidth(signalText, MaterialStyle::fontFor(MaterialStyle::TextRole::Body)) + 14;
        int ssidMaxWidth = SummaryTextRightX - ssidX;
        String ssid = MaterialStyle::truncateToWidth(
            tft, _config.wifiSsid, ssidMaxWidth, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));
        tft.drawString(ssid, ssidX, line1Y, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));
        drawSummaryLine(tft,
                        SummaryTextX,
                        line2Y,
                        CardWidth - (CardBodyInsetX * 2),
                        _monitorIpAddress,
                        MaterialStyle::ComponentState::Normal,
                        TL_DATUM);
        return;
    }

    drawSummaryLine(
        tft, SummaryTextX, line1Y, CardWidth - (CardBodyInsetX * 2), _config.wifiSsid, wifiHeadlineState(), TL_DATUM);
    drawSummaryLine(
        tft, SummaryTextX, line2Y, CardWidth - (CardBodyInsetX * 2), wifiDetailText(), wifiDetailState(), TL_DATUM);
}

void CurrentSettingsScreen::drawReceiverSummary(TFT_eSPI& tft) const {
    int line1Y = ReceiverCardY + SummaryLine1Y;
    int line2Y = ReceiverCardY + SummaryLine2Y;

    if (_config.receiverIp.length() == 0) {
        drawSummaryLine(tft,
                        SummaryTextX,
                        line1Y,
                        CardWidth - (CardBodyInsetX * 2),
                        "Unconfigured",
                        MaterialStyle::ComponentState::Warning,
                        TL_DATUM);
        return;
    }

    String status = receiverStatusText();
    int statusWidth = tft.textWidth(status, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));
    int headlineMaxWidth = CardWidth - (CardBodyInsetX * 2) - statusWidth - ReceiverStatusGap;

    drawSummaryLine(
        tft, SummaryTextX, line1Y, headlineMaxWidth, receiverHeadlineText(), receiverHeadlineState(), TL_DATUM);
    drawSummaryLine(tft, SummaryTextRightX, line1Y, statusWidth, status, receiverStatusState(), TR_DATUM);
    drawSummaryLine(tft,
                    SummaryTextX,
                    line2Y,
                    CardWidth - (CardBodyInsetX * 2),
                    receiverDetailText(),
                    receiverDetailState(),
                    TL_DATUM);
}

void CurrentSettingsScreen::drawSummaryLine(TFT_eSPI& tft,
                                            int x,
                                            int y,
                                            int maxWidth,
                                            const String& text,
                                            MaterialStyle::ComponentState state,
                                            uint8_t datum) const {
    String display =
        MaterialStyle::truncateToWidth(tft, text, maxWidth, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));
    tft.setTextDatum(datum);
    tft.setTextColor(MaterialStyle::textColorFor(MaterialStyle::TextRole::Body, state), DisplayManager::COLOR_PANEL);
    tft.drawString(display, x, y, MaterialStyle::fontFor(MaterialStyle::TextRole::Body));
}

String CurrentSettingsScreen::wifiHeadlineText() const {
    return _config.wifiSsid.length() > 0 ? _config.wifiSsid : "Unconfigured";
}

String CurrentSettingsScreen::wifiDetailText() const {
    if (_config.wifiSsid.length() == 0) {
        return "";
    }
    if (_wifiConnected && _monitorIpAddress.length() > 0) {
        return _monitorIpAddress;
    }
    return "Disconnected";
}

String CurrentSettingsScreen::receiverHeadlineText() const {
    if (_config.receiverIp.length() == 0) {
        return "Unconfigured";
    }
    if (_receiverIdentity.length() > 0) {
        return _receiverIdentity;
    }
    return "Configured receiver";
}

String CurrentSettingsScreen::receiverDetailText() const {
    if (_config.receiverIp.length() == 0) {
        return "";
    }
    return _config.receiverIp;
}

String CurrentSettingsScreen::receiverStatusText() const {
    if (_config.receiverIp.length() == 0) {
        return "";
    }
    if (!_receiverStatus.isValid || !_receiverStatus.powerKnown) {
        return "Unavailable";
    }
    return _receiverStatus.power ? "On" : "Off";
}

int CurrentSettingsScreen::wifiSignalLevel() const {
    return _hasWifiSignal ? WiFiManager::signalLevelForRssi(_wifiRssi) : 0;
}

MaterialStyle::ComponentState CurrentSettingsScreen::wifiHeadlineState() const {
    return _config.wifiSsid.length() > 0 ? MaterialStyle::ComponentState::Normal
                                         : MaterialStyle::ComponentState::Warning;
}

MaterialStyle::ComponentState CurrentSettingsScreen::wifiDetailState() const {
    return _wifiConnected ? MaterialStyle::ComponentState::Normal : MaterialStyle::ComponentState::Unavailable;
}

MaterialStyle::ComponentState CurrentSettingsScreen::receiverHeadlineState() const {
    if (_config.receiverIp.length() == 0) {
        return MaterialStyle::ComponentState::Warning;
    }
    return _receiverIdentity.length() > 0 ? MaterialStyle::ComponentState::Normal
                                          : MaterialStyle::ComponentState::Unavailable;
}

MaterialStyle::ComponentState CurrentSettingsScreen::receiverDetailState() const {
    return _config.receiverIp.length() > 0 ? MaterialStyle::ComponentState::Normal
                                           : MaterialStyle::ComponentState::Warning;
}

MaterialStyle::ComponentState CurrentSettingsScreen::receiverStatusState() const {
    if (_config.receiverIp.length() == 0) {
        return MaterialStyle::ComponentState::Warning;
    }
    if (!_receiverStatus.isValid || !_receiverStatus.powerKnown) {
        return MaterialStyle::ComponentState::Unavailable;
    }
    return _receiverStatus.power ? MaterialStyle::ComponentState::Success : MaterialStyle::ComponentState::Warning;
}

bool CurrentSettingsScreen::isOkPressed(TS_Point p) const {
    return p.x >= 182 && p.x <= 298 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}
