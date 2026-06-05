#ifndef RECEIVER_LIST_SCREEN_H
#define RECEIVER_LIST_SCREEN_H

#include "Screen.h"
#include "../../network/ReceiverDiscovery.h"

class ReceiverListScreen : public Screen {
public:
    ReceiverListScreen(ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    ScreenReturnTarget _returnTarget;
    ReceiverDiscoveryState _lastState = ReceiverDiscoveryState::Idle;
    size_t _lastCandidateCount = 0;
    uint8_t _progressFrame = 0;
    uint32_t _lastProgressAtMs = 0;

    void drawHeader(TFT_eSPI& tft);
    void drawActions(TFT_eSPI& tft);
    void drawCandidateRow(TFT_eSPI& tft, const ReceiverCandidate& candidate, int index, int y);
    void startDiscovery();
    int touchedCandidateIndex(TS_Point p) const;
};

#endif
