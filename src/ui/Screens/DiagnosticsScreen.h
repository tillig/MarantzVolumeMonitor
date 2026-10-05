#ifndef DIAGNOSTICS_SCREEN_H
#define DIAGNOSTICS_SCREEN_H

#include "Screen.h"

class DiagnosticsScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    static constexpr uint32_t RefreshIntervalMs = 1000;

    uint32_t _lastRefreshAtMs = 0;
    uint32_t _drawnLogCount = 0;
    String _drawnSubtitle;

    void drawLog(TFT_eSPI& tft);
    static String subtitleText();
    static bool isOkPressed(TS_Point p);
};

#endif
