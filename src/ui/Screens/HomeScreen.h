#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "Screen.h"
#include "../../network/MarantzClient.h"
#include "../../storage/ConfigStore.h"

class HomeScreen : public Screen {
public:
    enum class Layout {
        Classic,
        Modern,
        Minimal,
        Unified
    };

    HomeScreen();
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    MarantzStatus _lastStatus;
    DeviceConfig _config;
    uint16_t _textColors[7];
    int _colorIndex;
    Layout _currentLayout;
    bool _hasWifiConfig = false;
    bool _hasReceiverConfig = false;
    bool _isWifiConnected = false;
    String _ipAddress;
    uint32_t _lastRefreshMs = 0;

    void drawVolume(float volume);
    void drawSource(const String& source);
    void drawMode(const String& mode);
    void drawTiles(const String& mode);
    void drawSettingsButton();
    void drawSetupState();
    void loadStoredConfig();
    void refreshState();
    bool isCalibrationButtonPressed(TS_Point p) const;
    bool isSettingsButtonPressed(TS_Point p) const;

    // Layout-specific drawing
    void drawLayoutClassic();
    void drawLayoutModern();
    void drawLayoutMinimal();
    void drawLayoutUnified();

    // Helper for Arc Gauge
    void drawVolumeArc(int x, int y, int r, float volume);
};
#endif
