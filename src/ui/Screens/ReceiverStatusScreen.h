#ifndef RECEIVER_STATUS_SCREEN_H
#define RECEIVER_STATUS_SCREEN_H

#include "Screen.h"
#include "../../network/ReceiverDiscovery.h"

class ReceiverStatusScreen : public Screen {
public:
    ReceiverStatusScreen(const ReceiverCandidate& candidate,
                         ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    ReceiverCandidate _candidate;
    ScreenReturnTarget _returnTarget;
    ReceiverVerificationResult _result;
    bool _started = false;
    bool _complete = false;
    uint32_t _startedAtMs = 0;
    uint8_t _progressFrame = 0;
    uint32_t _lastProgressAtMs = 0;

    void verify();
    void saveVerifiedReceiver();
    String failureText() const;
};

#endif
