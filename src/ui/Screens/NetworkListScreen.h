#ifndef NETWORK_LIST_SCREEN_H
#define NETWORK_LIST_SCREEN_H

#include "Screen.h"
#include "../../network/WiFiManager.h"
#include <vector>

class NetworkListScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    bool _isScanning = false;
    int _scrollOffset = 0;
    std::vector<WiFiManager::NetworkInfo> _networks;
    void drawList();
};

#endif
