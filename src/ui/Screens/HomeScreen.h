#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include "Screen.h"
#include "../MaterialStyle.h"
#include "../../network/MarantzClient.h"
#include "../../storage/ConfigStore.h"

class HomeScreen : public Screen {
public:
    enum class Layout {
        Classic,
        Modern,
        Minimal,
        Unified
    };

    enum class DisplayState {
        WifiSetupRequired,
        WifiConnecting,
        ReceiverSetupRequired,
        ReceiverUnavailable,
        ReceiverOffVisible,
        ReceiverOffBlank,
        Live
    };

    HomeScreen();
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    struct Rect {
        int x;
        int y;
        int w;
        int h;
    };

    struct VolumeAnimation {
        bool active = false;
        float startVolume = -80.0f;
        float displayedVolume = -80.0f;
        float targetVolume = -80.0f;
        float lastRenderedVolume = -80.0f;
        String lastRenderedText;
        uint8_t lastRenderedFont = 0;
        uint32_t startMs = 0;
        uint32_t durationMs = 0;
        uint32_t lastFrameMs = 0;
    };

    struct ReceiverOffTimer {
        uint32_t startedAtMs = 0;
        uint32_t durationMs = 3000;
        bool active = false;
    };

    MarantzStatus _lastStatus;
    DeviceConfig _config;
    uint16_t _textColors[7];
    int _colorIndex;
    Layout _currentLayout;
    DisplayState _displayState;
    bool _hasWifiConfig = false;
    bool _hasReceiverConfig = false;
    bool _isWifiConnected = false;
    String _ipAddress;
    uint32_t _lastRefreshMs = 0;
    VolumeAnimation _volumeAnimation;
    ReceiverOffTimer _receiverOffTimer;

    void drawVolume(float volume);
    void drawSource(const String& source);
    void drawMode(const String& mode);
    void drawTiles(const String& mode);
    void drawSettingsButton();
    void drawAudioFamilyIcons();
    void drawSetupState();
    void drawCalibrationButton();
    void loadStoredConfig();
    void refreshState();
    DisplayState classifyDisplayState(const MarantzStatus& status) const;
    DisplayState resolveDisplayState(DisplayState classifiedState) const;
    void setDisplayState(DisplayState state, uint32_t now);
    void startReceiverOffTimer(uint32_t now);
    void stopReceiverOffTimer();
    bool isReceiverOffTimerExpired(uint32_t now) const;
    bool isReceiverOffDisplayState(DisplayState state) const;
    bool isCalibrationButtonPressed(TS_Point p) const;
    bool isSettingsButtonPressed(TS_Point p) const;
    bool isSettingsAccessible() const;
    int activeAudioFamilyIndex() const;
    String formatVolume(float volume) const;
    String displaySource() const;
    String displayMode() const;
    float displayVolume(float receiverVolume) const;
    float volumeToPercent(float volume) const;
    uint8_t volumeFont(const String& valueText) const;
    uint32_t animationDurationFor(float startVolume, float targetVolume) const;
    float animationProgress(uint32_t now) const;
    float easedAnimationProgress(float progress) const;
    void syncDisplayedVolume(float volume);
    void retargetVolumeAnimation(float targetVolume);
    void stopVolumeAnimation();
    void redrawLiveVolumeRegion();
    void redrawLiveVolumeFrame(bool forceFull = false);
    void redrawLiveMetadataRegion();
    bool tickVolumeAnimation(uint32_t now);
    void renderLiveVolumeRegion(TFT_eSPI& target, int originX, int originY);
    Rect liveGaugeRegion() const;
    Rect liveSourceRegion() const;
    Rect liveModeRegion() const;
    Rect liveAudioIconsRegion() const;
    Rect volumeTextRegion(float volume) const;
    int volumeSweep(float volume) const;
    uint16_t volumeArcColor(int sweep) const;
    void drawVolumeArcDelta(TFT_eSPI& target, float previousVolume, float currentVolume);
    void drawVolumeArcSegment(TFT_eSPI& target, int x, int y, int r, int startSweep,
                              int endSweep, bool colored);
    void drawVolumeCap(TFT_eSPI& target, int x, int y, int r, int sweep,
                       uint16_t color);
    void drawVolumeValueText(TFT_eSPI& target, const String& valueText, uint8_t font,
                             int centerX, int centerY);
    void drawVolumeValueTextDelta(TFT_eSPI& target, const String& valueText,
                                  uint8_t font);
    void drawVolumeTextAt(TFT_eSPI& target, const String& text, int x, int y,
                          uint8_t font);
    void clearRect(const Rect& rect);
    void drawReceiverStatusState(const String& title, const String& message,
                                 MaterialStyle::StatusKind kind);

    // Layout-specific drawing
    void drawLayoutClassic();
    void drawLayoutModern();
    void drawLayoutMinimal();
    void drawLayoutUnified();

    // Helper for Arc Gauge
    void drawVolumeArc(TFT_eSPI& target, int x, int y, int r, float volume);
};
#endif
