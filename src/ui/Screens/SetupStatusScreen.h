#ifndef SETUP_STATUS_SCREEN_H
#define SETUP_STATUS_SCREEN_H

#include "Screen.h"

class SetupStatusScreen : public Screen {
public:
    SetupStatusScreen(const String& ssid, const String& password,
                      ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    String _ssid;
    String _password;
    ScreenReturnTarget _returnTarget;
    String _failureMessage = "Connection Failed";
    bool _isConnecting = false;
    bool _failed = false;
    bool _progressVisible = false;
    uint32_t _startTime = 0;
    uint8_t _progressFrame = 0;
    uint32_t _lastProgressAtMs = 0;
};

#endif
