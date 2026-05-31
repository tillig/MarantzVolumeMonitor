#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "Screen.h"
#include "../../network/MarantzClient.h"

class HomeScreen : public Screen {
public:
    HomeScreen();
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    MarantzStatus _lastStatus;
    uint16_t _textColors[7];
    int _colorIndex;

    void drawVolume(float volume);
    void drawSource(const String& source);
    void drawMode(const String& mode);
    void drawTiles(const String& mode);
    void drawSettingsButton();
    void drawSpikeText();
};

#endif
