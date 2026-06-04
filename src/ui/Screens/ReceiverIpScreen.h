#ifndef RECEIVER_IP_SCREEN_H
#define RECEIVER_IP_SCREEN_H

#include "Screen.h"

class ReceiverIpScreen : public Screen {
public:
    ReceiverIpScreen(ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    ScreenReturnTarget _returnTarget;
    String _ipAddress;
    String _errorMessage;

    void drawInput(TFT_eSPI& tft);
    void drawKeypad(TFT_eSPI& tft);
    void submit();
};

#endif
