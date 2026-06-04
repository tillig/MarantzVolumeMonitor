#ifndef CALIBRATION_SCREEN_H
#define CALIBRATION_SCREEN_H

#include "Screen.h"

class CalibrationScreen : public Screen {
public:
    CalibrationScreen(ScreenReturnTarget returnTarget = ScreenReturnTarget::Home);
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    static constexpr uint8_t PointCount = 9;

    struct CalibrationPoint {
        int16_t x;
        int16_t y;
        const char* label;
    };

    struct CalibrationSample {
        uint16_t rawX = 0;
        uint16_t rawY = 0;
        uint16_t rawZ = 0;
        int16_t mappedX = 0;
        int16_t mappedY = 0;
    };

    uint8_t _step = 0;
    bool _isComplete = false;
    bool _printedHeader = false;
    ScreenReturnTarget _returnTarget;

    CalibrationPoint _points[PointCount] = {
        {40, 40, "Top Left"},
        {240, 40, "Top Center"},
        {440, 40, "Top Right"},
        {40, 160, "Mid Left"},
        {240, 160, "Center"},
        {440, 160, "Mid Right"},
        {40, 280, "Bottom Left"},
        {240, 280, "Bottom Center"},
        {440, 280, "Bottom Right"}
    };
    CalibrationSample _samples[PointCount];

    void printSessionHeader();
    void printSummary();
};

#endif
