#ifndef RESET_DEFAULTS_SCREEN_H
#define RESET_DEFAULTS_SCREEN_H

#include "Screen.h"
#include "../../storage/ConfigStore.h"

class ResetDefaultsScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    enum class ResetType { Wifi, Receiver, Calibration };

    enum class Mode { ChoosingType, Confirming, Completed };

    Mode _mode = Mode::ChoosingType;
    ResetType _selectedType = ResetType::Wifi;
    bool _applyFailed = false;

    void drawSelection(TFT_eSPI& tft);
    void drawConfirmation(TFT_eSPI& tft);
    void drawCompleted(TFT_eSPI& tft);
    bool isChoicePressed(TS_Point p, int index) const;
    bool isCancelPressed(TS_Point p) const;
    bool isResetPressed(TS_Point p) const;
    bool isOkPressed(TS_Point p) const;
    const char* labelFor(ResetType type) const;
    const char* detailFor(ResetType type) const;
    ConfigStore::ResetTarget configTargetFor(ResetType type) const;
    void applyReset();
};

#endif
