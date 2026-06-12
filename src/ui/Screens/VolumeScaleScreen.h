#ifndef VOLUME_SCALE_SCREEN_H
#define VOLUME_SCALE_SCREEN_H

#include "Screen.h"
#include "../../storage/ConfigStore.h"

class VolumeScaleScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    enum class Choice { Normalized, Db };

    bool _initialized = false;
    bool _saveFailed = false;
    Choice _savedChoice = Choice::Normalized;
    Choice _pendingChoice = Choice::Normalized;

    void loadChoice();
    bool isNormalizedPressed(TS_Point p) const;
    bool isDbPressed(TS_Point p) const;
    bool isCancelPressed(TS_Point p) const;
    bool isOkPressed(TS_Point p) const;
    bool persistChoice();
    bool useDbScaleFor(Choice choice) const;
};

#endif
