#ifndef NETWORK_LIST_SCREEN_H
#define NETWORK_LIST_SCREEN_H

#include "Screen.h"
#include "../../network/WiFiManager.h"
#include <vector>

class NetworkListScreen : public Screen {
public:
    NetworkListScreen(ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    ScreenReturnTarget _returnTarget;
    bool _canCancel = false;
    bool _isScanning = false;
    bool _hasScanned = false;
    uint8_t _progressFrame = 0;
    uint32_t _lastProgressAtMs = 0;
    int _scrollOffset = 0;
    std::vector<WiFiManager::NetworkInfo> _networks;
    void drawList();
    void drawActions(TFT_eSPI& tft);
};

#endif
