#include "HomeScreen.h"

#include <math.h>

#include "SettingsScreen.h"
#include "NetworkListScreen.h"
#include "CalibrationScreen.h"
#include "ReceiverListScreen.h"
#include "../IconRenderer.h"
#include "../ScreenManager.h"
#include "../assets/IconBitmaps.h"
#include "../../network/WiFiManager.h"

namespace {
constexpr uint32_t RefreshIntervalMs = 1000;
constexpr uint32_t AnimationFrameMs = 16;
constexpr uint32_t MinAnimationDurationMs = 420;
constexpr uint32_t MaxAnimationDurationMs = 820;
constexpr float AnimationEpsilon = 0.05f;
constexpr int ArcCenterX = 240;
constexpr int ArcCenterY = 140;
constexpr int ArcOuterRadius = 125;
constexpr int ArcInnerRadius = 109;
constexpr int ArcStartAngle = 60;
constexpr int ArcSweepDegrees = 240;
constexpr int ArcCapRadius = 7;
constexpr int VolumeValueCenterX = 240;
constexpr int VolumeValueCenterY = 139;
constexpr int VolumeLabelCenterY = 72;

bool nearlyEqual(float left, float right, float epsilon = AnimationEpsilon) {
    return fabsf(left - right) <= epsilon;
}
}

HomeScreen::HomeScreen() {
    _colorIndex = 0;
    _currentLayout = Layout::Unified;
    _displayState = DisplayState::WifiSetupRequired;
    _textColors[0] = TFT_WHITE;
    _textColors[1] = TFT_GREEN;
    _textColors[2] = TFT_BLUE;
    _textColors[3] = TFT_RED;
    _textColors[4] = TFT_YELLOW;
    _textColors[5] = TFT_MAGENTA;
    _textColors[6] = TFT_CYAN;

    refreshState();
    setDisplayState(classifyDisplayState(_lastStatus), millis());
    if (_displayState == DisplayState::Live) {
        syncDisplayedVolume(displayVolume(_lastStatus.volume));
    } else {
        stopVolumeAnimation();
    }
}

void HomeScreen::draw() {
    DisplayManager& display = DisplayManager::getInstance();
    TFT_eSPI& tft = display.getTft();

    if (_displayState != DisplayState::ReceiverOffBlank) {
        display.setBacklightEnabled(true);
    }

    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    switch (_displayState) {
        case DisplayState::Live:
            switch (_currentLayout) {
                case Layout::Classic:
                case Layout::Modern:
                case Layout::Minimal:
                case Layout::Unified:
                default:
                    drawLayoutUnified();
                    break;
            }
            drawSettingsButton();
            break;
        case DisplayState::ReceiverOffVisible:
            drawReceiverStatusState("Receiver off",
                                    "Open Settings to change receiver setup.",
                                    MaterialStyle::StatusKind::Warning);
            drawSettingsButton();
            break;
        case DisplayState::ReceiverOffBlank:
            display.setBacklightEnabled(false);
            break;
        case DisplayState::ReceiverUnavailable:
            drawReceiverStatusState("Receiver unavailable",
                                    "Open Settings to change receiver setup.",
                                    MaterialStyle::StatusKind::Unavailable);
            drawSettingsButton();
            break;
        case DisplayState::WifiSetupRequired:
        case DisplayState::WifiConnecting:
        case DisplayState::ReceiverSetupRequired:
        default:
            drawSetupState();
            break;
    }
}

void HomeScreen::update() {
    uint32_t now = millis();

    if (now - _lastRefreshMs >= RefreshIntervalMs) {
        MarantzStatus previousStatus = _lastStatus;
        DisplayState previousDisplayState = _displayState;
        bool previousHasWifiConfig = _hasWifiConfig;
        bool previousHasReceiverConfig = _hasReceiverConfig;
        bool previousWifiConnected = _isWifiConnected;
        String previousIpAddress = _ipAddress;
        String previousWifiSsid = _config.wifiSsid;
        String previousReceiverIp = _config.receiverIp;

        refreshState();
        DisplayState classifiedState = classifyDisplayState(_lastStatus);
        DisplayState resolvedState = resolveDisplayState(classifiedState);
        setDisplayState(resolvedState, millis());
        now = millis();

        bool requiresFullRedraw = previousDisplayState != _displayState ||
                                  previousHasWifiConfig != _hasWifiConfig ||
                                  previousHasReceiverConfig != _hasReceiverConfig ||
                                  previousWifiConnected != _isWifiConnected ||
                                  previousIpAddress != _ipAddress ||
                                  previousWifiSsid != _config.wifiSsid ||
                                  previousReceiverIp != _config.receiverIp;

        if (_displayState == DisplayState::Live) {
            float previousDisplayVolume = displayVolume(previousStatus.volume);
            float currentDisplayVolume = displayVolume(_lastStatus.volume);
            if (previousDisplayState != DisplayState::Live) {
                syncDisplayedVolume(currentDisplayVolume);
                requiresFullRedraw = true;
            } else {
                if (previousStatus.input != _lastStatus.input ||
                    previousStatus.mode != _lastStatus.mode) {
                    redrawLiveMetadataRegion();
                }
                if (!nearlyEqual(previousDisplayVolume, currentDisplayVolume)) {
                    retargetVolumeAnimation(currentDisplayVolume);
                }
            }
        } else if (previousDisplayState == DisplayState::Live) {
            stopVolumeAnimation();
            requiresFullRedraw = true;
        }

        if (requiresFullRedraw) {
            draw();
            return;
        }
    }

    if (_displayState == DisplayState::ReceiverOffVisible &&
        isReceiverOffTimerExpired(now)) {
        setDisplayState(DisplayState::ReceiverOffBlank, now);
        draw();
        return;
    }

    if (_displayState == DisplayState::Live) {
        tickVolumeAnimation(now);
    }
}

void HomeScreen::handleTouch(TS_Point p) {
    if (_displayState == DisplayState::ReceiverOffBlank) {
        DisplayState wakeState = classifyDisplayState(_lastStatus);
        setDisplayState(wakeState, millis());
        draw();
        return;
    }

    if (isSettingsAccessible()) {
        if (isSettingsButtonPressed(p)) {
            ScreenManager::getInstance().setScreen(new SettingsScreen());
            return;
        }

        if (_displayState == DisplayState::ReceiverOffVisible) {
            startReceiverOffTimer(millis());
        }
        return;
    }

    if (isCalibrationButtonPressed(p)) {
        ScreenManager::getInstance().setScreen(new CalibrationScreen());
        return;
    }

    if (_displayState == DisplayState::ReceiverSetupRequired) {
        ScreenManager::getInstance().setScreen(new ReceiverListScreen());
        return;
    }

    ScreenManager::getInstance().setScreen(new NetworkListScreen());
}

void HomeScreen::drawSetupState() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    switch (_displayState) {
        case DisplayState::WifiSetupRequired:
            MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Warning,
                                           "Unconfigured",
                                           "Tap anywhere to configure Wi-Fi.",
                                           Icons::WARNING);
            drawCalibrationButton();
            break;
        case DisplayState::ReceiverSetupRequired:
            MaterialStyle::drawText(tft, "Wi-Fi Connected", 240, 80,
                                    MaterialStyle::TextRole::StatusMessage, TC_DATUM,
                                    MaterialStyle::ComponentState::Success);
            MaterialStyle::drawText(
                tft, MaterialStyle::truncateToWidth(tft, _config.wifiSsid, 380, 2),
                240, 122, MaterialStyle::TextRole::Body, TC_DATUM);
            if (_ipAddress.length() > 0) {
                MaterialStyle::drawText(tft, _ipAddress, 240, 146,
                                        MaterialStyle::TextRole::Body, TC_DATUM);
            }

            MaterialStyle::drawText(tft, "Receiver setup is still required.", 240, 188,
                                    MaterialStyle::TextRole::Body, TC_DATUM,
                                    MaterialStyle::ComponentState::Warning);
            MaterialStyle::drawText(tft, "Tap to configure receiver.", 240, 212,
                                    MaterialStyle::TextRole::Body, TC_DATUM);
            drawCalibrationButton();
            break;
        case DisplayState::WifiConnecting:
        default:
            MaterialStyle::drawStatusBlock(
                tft, MaterialStyle::StatusKind::Unavailable, "Connecting Wi-Fi",
                MaterialStyle::truncateToWidth(tft, _config.wifiSsid, 360, 2),
                Icons::WIFI);
            MaterialStyle::drawText(tft,
                                    "Saved credentials found. Tap to reconfigure Wi-Fi.",
                                    240, 224, MaterialStyle::TextRole::Body, TC_DATUM);
            drawCalibrationButton();
            break;
    }
}

void HomeScreen::drawReceiverStatusState(const String& title, const String& message,
                                         MaterialStyle::StatusKind kind) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    MaterialStyle::drawStatusBlock(tft, kind, title, message, Icons::RECEIVER);
}

void HomeScreen::loadStoredConfig() {
    _config = DeviceConfig();
    ConfigStore::getInstance().loadConfig(_config);
}

void HomeScreen::refreshState() {
    _lastRefreshMs = millis();
    loadStoredConfig();

    _hasWifiConfig = _config.wifiSsid.length() > 0;
    _hasReceiverConfig = _config.receiverIp.length() > 0;
    _isWifiConnected = WiFiManager::getInstance().isConnected();
    _ipAddress = _isWifiConnected ? WiFiManager::getInstance().getIPAddress() : "";

    _lastStatus = MarantzStatus();
    if (_hasReceiverConfig) {
        MarantzClient::getInstance().setReceiverIp(_config.receiverIp);
    }

    if (_hasWifiConfig && _hasReceiverConfig && _isWifiConnected) {
        _lastStatus = MarantzClient::getInstance().getStatus();
    }
}

HomeScreen::DisplayState HomeScreen::classifyDisplayState(const MarantzStatus& status) const {
    if (!_hasWifiConfig) {
        return DisplayState::WifiSetupRequired;
    }
    if (!_isWifiConnected) {
        return DisplayState::WifiConnecting;
    }
    if (!_hasReceiverConfig) {
        return DisplayState::ReceiverSetupRequired;
    }
    if (status.isValid && status.powerKnown && !status.power) {
        return DisplayState::ReceiverOffVisible;
    }
    if (status.isValid && status.power && status.hasVolume) {
        return DisplayState::Live;
    }
    return DisplayState::ReceiverUnavailable;
}

HomeScreen::DisplayState HomeScreen::resolveDisplayState(DisplayState classifiedState) const {
    if (_displayState == DisplayState::ReceiverOffBlank) {
        if (classifiedState == DisplayState::Live) {
            return DisplayState::Live;
        }
        return DisplayState::ReceiverOffBlank;
    }

    return classifiedState;
}

void HomeScreen::setDisplayState(DisplayState state, uint32_t now) {
    if (state == _displayState) {
        if (state == DisplayState::ReceiverOffVisible && !_receiverOffTimer.active) {
            startReceiverOffTimer(now);
        }
        return;
    }

    DisplayState previousState = _displayState;
    _displayState = state;

    if (state == DisplayState::ReceiverOffVisible) {
        startReceiverOffTimer(now);
    } else {
        stopReceiverOffTimer();
    }

    if (previousState == DisplayState::Live && state != DisplayState::Live) {
        stopVolumeAnimation();
    }
}

void HomeScreen::startReceiverOffTimer(uint32_t now) {
    _receiverOffTimer.startedAtMs = now;
    _receiverOffTimer.active = true;
}

void HomeScreen::stopReceiverOffTimer() {
    _receiverOffTimer.active = false;
}

bool HomeScreen::isReceiverOffTimerExpired(uint32_t now) const {
    return _receiverOffTimer.active &&
           now - _receiverOffTimer.startedAtMs >= _receiverOffTimer.durationMs;
}

bool HomeScreen::isReceiverOffDisplayState(DisplayState state) const {
    return state == DisplayState::ReceiverOffVisible ||
           state == DisplayState::ReceiverOffBlank;
}

bool HomeScreen::isCalibrationButtonPressed(TS_Point p) const {
    return p.x >= 300 && p.x <= 456 &&
           p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool HomeScreen::isSettingsButtonPressed(TS_Point p) const {
    return p.x >= 426 && p.x <= 474 && p.y >= 12 && p.y <= 48;
}

bool HomeScreen::isSettingsAccessible() const {
    return _displayState == DisplayState::Live ||
           _displayState == DisplayState::ReceiverOffVisible ||
           _displayState == DisplayState::ReceiverUnavailable;
}

String HomeScreen::formatVolume(float volume) const {
    char buffer[12];
    dtostrf(volume, 1, 1, buffer);
    String value(buffer);
    value.trim();
    return value;
}

String HomeScreen::displaySource() const {
    if (_lastStatus.input.length() == 0) {
        return "Input unavailable";
    }
    return _lastStatus.input;
}

String HomeScreen::displayMode() const {
    if (_lastStatus.mode.length() == 0) {
        return "Mode unavailable";
    }
    return _lastStatus.mode;
}

float HomeScreen::displayVolume(float receiverVolume) const {
    return constrain(receiverVolume + 80.0f, 0.0f, 100.0f);
}

float HomeScreen::volumeToPercent(float volume) const {
    return constrain(volume / 100.0f, 0.0f, 1.0f);
}

uint8_t HomeScreen::volumeFont(const String& valueText) const {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    if (tft.textWidth(valueText, 8) <= 206) {
        return 8;
    }
    if (tft.textWidth(valueText, 7) <= 188) {
        return 7;
    }
    return 7;
}

uint32_t HomeScreen::animationDurationFor(float startVolume, float targetVolume) const {
    float distance = fabsf(targetVolume - startVolume);
    float scaled = constrain(distance / 18.0f, 0.0f, 1.0f);
    return MinAnimationDurationMs +
           static_cast<uint32_t>((MaxAnimationDurationMs - MinAnimationDurationMs) * scaled);
}

float HomeScreen::animationProgress(uint32_t now) const {
    if (_volumeAnimation.durationMs == 0) {
        return 1.0f;
    }
    if (now <= _volumeAnimation.startMs) {
        return 0.0f;
    }
    float progress =
        static_cast<float>(now - _volumeAnimation.startMs) / _volumeAnimation.durationMs;
    return constrain(progress, 0.0f, 1.0f);
}

float HomeScreen::easedAnimationProgress(float progress) const {
    float inverse = 1.0f - progress;
    return 1.0f - (inverse * inverse * inverse);
}

void HomeScreen::syncDisplayedVolume(float volume) {
    _volumeAnimation.active = false;
    _volumeAnimation.startVolume = volume;
    _volumeAnimation.displayedVolume = volume;
    _volumeAnimation.targetVolume = volume;
    _volumeAnimation.lastRenderedVolume = volume;
    _volumeAnimation.lastRenderedText = "";
    _volumeAnimation.lastRenderedFont = 0;
    _volumeAnimation.startMs = millis();
    _volumeAnimation.durationMs = 0;
    _volumeAnimation.lastFrameMs = 0;
}

void HomeScreen::retargetVolumeAnimation(float targetVolume) {
    uint32_t now = millis();
    if (_volumeAnimation.active) {
        float progress = easedAnimationProgress(animationProgress(now));
        _volumeAnimation.displayedVolume =
            _volumeAnimation.startVolume +
            ((_volumeAnimation.targetVolume - _volumeAnimation.startVolume) * progress);
    }

    if (nearlyEqual(_volumeAnimation.displayedVolume, targetVolume)) {
        syncDisplayedVolume(targetVolume);
        redrawLiveVolumeRegion();
        return;
    }

    _volumeAnimation.active = true;
    _volumeAnimation.startVolume = _volumeAnimation.displayedVolume;
    _volumeAnimation.targetVolume = targetVolume;
    _volumeAnimation.startMs = now;
    _volumeAnimation.durationMs =
        animationDurationFor(_volumeAnimation.startVolume, targetVolume);
    _volumeAnimation.lastFrameMs = 0;
}

void HomeScreen::stopVolumeAnimation() {
    _volumeAnimation.active = false;
    _volumeAnimation.lastFrameMs = 0;
}

void HomeScreen::redrawLiveVolumeRegion() {
    redrawLiveVolumeFrame(true);
}

void HomeScreen::redrawLiveVolumeFrame(bool forceFull) {
    Rect region = liveGaugeRegion();
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    if (forceFull) {
        clearRect(region);
        renderLiveVolumeRegion(tft, 0, 0);
        _volumeAnimation.lastRenderedText = formatVolume(_volumeAnimation.displayedVolume);
        _volumeAnimation.lastRenderedFont = volumeFont(_volumeAnimation.lastRenderedText);
        _volumeAnimation.lastRenderedVolume = _volumeAnimation.displayedVolume;
        return;
    }

    drawVolumeArcDelta(tft, _volumeAnimation.lastRenderedVolume,
                       _volumeAnimation.displayedVolume);
    String volumeText = formatVolume(_volumeAnimation.displayedVolume);
    uint8_t font = volumeFont(volumeText);
    drawVolumeValueTextDelta(tft, volumeText, font);

    _volumeAnimation.lastRenderedText = volumeText;
    _volumeAnimation.lastRenderedFont = font;
    _volumeAnimation.lastRenderedVolume = _volumeAnimation.displayedVolume;
}

void HomeScreen::redrawLiveMetadataRegion() {
    clearRect(liveSourceRegion());
    clearRect(liveModeRegion());
    clearRect(liveAudioIconsRegion());

    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(ML_DATUM);
    tft.drawString(
        MaterialStyle::truncateToWidth(tft, displaySource(), liveSourceRegion().w, 4),
        liveSourceRegion().x, 255, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString(MaterialStyle::truncateToWidth(tft, displayMode(), liveModeRegion().w, 4),
                   liveModeRegion().x + liveModeRegion().w, 255, 4);

    drawAudioFamilyIcons();
}

bool HomeScreen::tickVolumeAnimation(uint32_t now) {
    if (!_volumeAnimation.active) {
        return false;
    }
    if (_volumeAnimation.lastFrameMs != 0 &&
        now - _volumeAnimation.lastFrameMs < AnimationFrameMs) {
        return false;
    }

    float progress = animationProgress(now);
    float eased = easedAnimationProgress(progress);
    _volumeAnimation.displayedVolume =
        _volumeAnimation.startVolume +
        ((_volumeAnimation.targetVolume - _volumeAnimation.startVolume) * eased);
    _volumeAnimation.lastFrameMs = now;

    if (progress >= 1.0f || nearlyEqual(_volumeAnimation.displayedVolume,
                                        _volumeAnimation.targetVolume)) {
        _volumeAnimation.displayedVolume = _volumeAnimation.targetVolume;
        _volumeAnimation.active = false;
    }

    redrawLiveVolumeFrame();
    return true;
}

void HomeScreen::renderLiveVolumeRegion(TFT_eSPI& target, int originX, int originY) {
    int centerX = ArcCenterX - originX;
    int centerY = ArcCenterY - originY;
    target.drawArc(centerX, centerY, ArcOuterRadius, ArcInnerRadius, ArcStartAngle,
                   ArcStartAngle + ArcSweepDegrees, DisplayManager::COLOR_BAR_BG,
                   DisplayManager::COLOR_BACKGROUND);
    drawVolumeArc(target, centerX, centerY, ArcOuterRadius, _volumeAnimation.displayedVolume);

    String volumeText = formatVolume(_volumeAnimation.displayedVolume);
    uint8_t font = volumeFont(volumeText);
    drawVolumeValueText(target, volumeText, font, VolumeValueCenterX - originX,
                        VolumeValueCenterY - originY);

    target.setTextDatum(MC_DATUM);
    target.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    target.drawString("VOLUME", 240 - originX, VolumeLabelCenterY - originY, 2);
}

HomeScreen::Rect HomeScreen::liveGaugeRegion() const {
    return {92, 6, 296, 204};
}

HomeScreen::Rect HomeScreen::liveSourceRegion() const {
    return {28, 226, 172, 44};
}

HomeScreen::Rect HomeScreen::liveModeRegion() const {
    return {280, 226, 172, 44};
}

HomeScreen::Rect HomeScreen::liveAudioIconsRegion() const {
    return {20, 270, 440, 42};
}

void HomeScreen::clearRect(const Rect& rect) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    int x = rect.x < 0 ? 0 : rect.x;
    int y = rect.y < 0 ? 0 : rect.y;
    int maxW = MaterialStyle::ScreenWidth - x;
    int maxH = MaterialStyle::ScreenHeight - y;
    int w = rect.w > maxW ? maxW : rect.w;
    int h = rect.h > maxH ? maxH : rect.h;
    tft.fillRect(x, y, w, h, DisplayManager::COLOR_BACKGROUND);
}

HomeScreen::Rect HomeScreen::volumeTextRegion(float volume) const {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    String volumeText = formatVolume(volume);
    uint8_t font = volumeFont(volumeText);
    int textWidth = tft.textWidth(volumeText, font);
    int textHeight = tft.fontHeight(font);
    int left = VolumeValueCenterX - (textWidth / 2);
    int top = VolumeValueCenterY - (textHeight / 2);

    return {left - 3, top - 2, textWidth + 8, textHeight + 4};
}

int HomeScreen::volumeSweep(float volume) const {
    return constrain(static_cast<int>(lroundf(volumeToPercent(volume) * ArcSweepDegrees)),
                     0, ArcSweepDegrees);
}

uint16_t HomeScreen::volumeArcColor(int sweep) const {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    float p = static_cast<float>(sweep) / ArcSweepDegrees;
    p = constrain(p, 0.0f, 1.0f);
    uint8_t red;
    uint8_t green;
    if (p < 0.5f) {
        red = static_cast<uint8_t>(p * 2.0f * 255.0f);
        green = 255;
    } else {
        red = 255;
        green = static_cast<uint8_t>((1.0f - ((p - 0.5f) * 2.0f)) * 255.0f);
    }
    return tft.color565(red, green, 0);
}

void HomeScreen::drawVolumeArcDelta(TFT_eSPI& target, float previousVolume,
                                    float currentVolume) {
    int previousSweep = volumeSweep(previousVolume);
    int currentSweep = volumeSweep(currentVolume);

    if (currentSweep > previousSweep) {
        drawVolumeArcSegment(target, ArcCenterX, ArcCenterY, ArcOuterRadius,
                             previousSweep, currentSweep, true);
    } else if (currentSweep < previousSweep) {
        drawVolumeArcSegment(target, ArcCenterX, ArcCenterY, ArcOuterRadius,
                             currentSweep, previousSweep, false);
        drawVolumeCap(target, ArcCenterX, ArcCenterY, ArcOuterRadius,
                      previousSweep, DisplayManager::COLOR_BAR_BG);
    }

    drawVolumeCap(target, ArcCenterX, ArcCenterY, ArcOuterRadius, 0,
                  volumeArcColor(0));
    drawVolumeCap(target, ArcCenterX, ArcCenterY, ArcOuterRadius, ArcSweepDegrees,
                  DisplayManager::COLOR_BAR_BG);
    if (currentSweep > 0) {
        drawVolumeCap(target, ArcCenterX, ArcCenterY, ArcOuterRadius, currentSweep,
                      volumeArcColor(currentSweep));
    }
}

void HomeScreen::drawVolumeArcSegment(TFT_eSPI& target, int x, int y, int r,
                                      int startSweep, int endSweep, bool colored) {
    startSweep = constrain(startSweep, 0, ArcSweepDegrees);
    endSweep = constrain(endSweep, 0, ArcSweepDegrees);
    if (endSweep <= startSweep) {
        return;
    }

    for (int sweep = startSweep; sweep < endSweep; sweep += 4) {
        int nextSweep = sweep + 4;
        if (nextSweep > endSweep) {
            nextSweep = endSweep;
        }
        uint16_t color = colored ? volumeArcColor(nextSweep)
                                 : DisplayManager::COLOR_BAR_BG;
        target.drawArc(x, y, r, ArcInnerRadius, ArcStartAngle + sweep,
                       ArcStartAngle + nextSweep, color,
                       DisplayManager::COLOR_BACKGROUND);
    }
}

void HomeScreen::drawVolumeCap(TFT_eSPI& target, int x, int y, int r, int sweep,
                               uint16_t color) {
    float midRadius = r - 8;
    float radians = (ArcStartAngle + sweep + 90) * PI / 180.0f;
    target.fillCircle(x + midRadius * cosf(radians),
                      y + midRadius * sinf(radians), ArcCapRadius, color);
}

void HomeScreen::drawVolumeValueText(TFT_eSPI& target, const String& valueText,
                                     uint8_t font, int centerX, int centerY) {
    int textWidth = target.textWidth(valueText, font);
    int textHeight = target.fontHeight(font);
    int left = centerX - (textWidth / 2);
    int top = centerY - (textHeight / 2);

    target.setTextDatum(TL_DATUM);
    drawVolumeTextAt(target, valueText, left, top, font);
}

void HomeScreen::drawVolumeValueTextDelta(TFT_eSPI& target, const String& valueText,
                                          uint8_t font) {
    int textWidth = target.textWidth(valueText, font);
    int textHeight = target.fontHeight(font);
    int left = VolumeValueCenterX - (textWidth / 2);
    int top = VolumeValueCenterY - (textHeight / 2);
    String previousText = _volumeAnimation.lastRenderedText;

    bool requiresFullTextRedraw =
        previousText.length() == 0 ||
        previousText.length() != valueText.length() ||
        _volumeAnimation.lastRenderedFont != font ||
        target.textWidth(previousText, font) != textWidth;

    if (!requiresFullTextRedraw) {
        for (uint8_t i = 0; i < valueText.length(); i++) {
            String previousChar = previousText.substring(i, i + 1);
            String currentChar = valueText.substring(i, i + 1);
            if (target.textWidth(previousChar, font) !=
                target.textWidth(currentChar, font)) {
                requiresFullTextRedraw = true;
                break;
            }
        }
    }

    if (requiresFullTextRedraw) {
        Rect oldRegion = volumeTextRegion(_volumeAnimation.lastRenderedVolume);
        Rect newRegion = volumeTextRegion(_volumeAnimation.displayedVolume);
        int x1 = min(oldRegion.x, newRegion.x);
        int y1 = min(oldRegion.y, newRegion.y);
        int x2 = max(oldRegion.x + oldRegion.w, newRegion.x + newRegion.w);
        int y2 = max(oldRegion.y + oldRegion.h, newRegion.y + newRegion.h);
        clearRect({x1, y1, x2 - x1, y2 - y1});
        drawVolumeValueText(target, valueText, font, VolumeValueCenterX,
                            VolumeValueCenterY);
        return;
    }

    int x = left;
    for (uint8_t i = 0; i < valueText.length(); i++) {
        String previousChar = previousText.substring(i, i + 1);
        String currentChar = valueText.substring(i, i + 1);
        int charWidth = target.textWidth(currentChar, font);

        if (previousChar != currentChar) {
            clearRect({x - 2, top - 2, charWidth + 5, textHeight + 4});
            target.setTextDatum(TL_DATUM);
            drawVolumeTextAt(target, currentChar, x, top, font);
        }

        x += charWidth;
    }
}

void HomeScreen::drawVolumeTextAt(TFT_eSPI& target, const String& text, int x, int y,
                                  uint8_t font) {
    target.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    target.drawString(text, x, y, font);
    target.drawString(text, x + 1, y, font);
    target.setTextDatum(MC_DATUM);
}

void HomeScreen::drawLayoutUnified() {
    redrawLiveVolumeRegion();
    redrawLiveMetadataRegion();
}

void HomeScreen::drawVolumeArc(TFT_eSPI& target, int x, int y, int r, float volume) {
    int currentSweep = volumeSweep(volume);
    int endAngle = ArcStartAngle + currentSweep;

    for (int i = 0; i < currentSweep; i += 4) {
        int nextSweep = i + 4;
        if (nextSweep > currentSweep) {
            nextSweep = currentSweep;
        }
        int segmentStart = ArcStartAngle + i;
        int segmentEnd = ArcStartAngle + nextSweep;
        target.drawArc(x, y, r, ArcInnerRadius, segmentStart, segmentEnd,
                       volumeArcColor(nextSweep), DisplayManager::COLOR_BACKGROUND);
    }

    float midRadius = r - 8;
    float startRadians = (ArcStartAngle + 90) * PI / 180.0f;
    target.fillCircle(x + midRadius * cosf(startRadians),
                      y + midRadius * sinf(startRadians), ArcCapRadius,
                      volumeArcColor(0));

    float endRadians = (ArcStartAngle + ArcSweepDegrees + 90) * PI / 180.0f;
    target.fillCircle(x + midRadius * cosf(endRadians),
                      y + midRadius * sinf(endRadians), ArcCapRadius,
                      DisplayManager::COLOR_BAR_BG);

    if (currentSweep > 0) {
        float tipRadians = (endAngle + 90) * PI / 180.0f;
        target.fillCircle(x + midRadius * cosf(tipRadians),
                          y + midRadius * sinf(tipRadians), ArcCapRadius,
                          volumeArcColor(currentSweep));
    }
}

void HomeScreen::drawLayoutClassic() {
    drawLayoutUnified();
}

void HomeScreen::drawLayoutModern() {
    drawLayoutUnified();
}

void HomeScreen::drawLayoutMinimal() {
    drawLayoutUnified();
}

void HomeScreen::drawVolume(float volume) {
    syncDisplayedVolume(displayVolume(volume));
    redrawLiveVolumeRegion();
}

void HomeScreen::drawSource(const String& source) {
    _lastStatus.input = source;
    redrawLiveMetadataRegion();
}

void HomeScreen::drawMode(const String& mode) {
    _lastStatus.mode = mode;
    redrawLiveMetadataRegion();
}

void HomeScreen::drawTiles(const String& mode) {
    _lastStatus.mode = mode;
    redrawLiveMetadataRegion();
}

void HomeScreen::drawSettingsButton() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    IconRenderer::drawCentered(tft, Icons::SETTINGS, 450, 28,
                               DisplayManager::COLOR_TEXT_SECONDARY);
}

void HomeScreen::drawCalibrationButton() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    MaterialStyle::drawStandardButton(tft, 300, MaterialStyle::BottomActionY, 156,
                                      MaterialStyle::ButtonHeight,
                                      Icons::TOUCH_CALIBRATION, "Calibrate");
}

void HomeScreen::drawAudioFamilyIcons() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    const Icons::IconBitmap* icons[] = {
        &Icons::AUDIO_DOLBY,
        &Icons::AUDIO_DTS,
        &Icons::AUDIO_PCM,
        &Icons::AUDIO_OTHER
    };
    int activeIndex = activeAudioFamilyIndex();

    for (int i = 0; i < 4; i++) {
        int tx = 25 + (i * 112);
        int ty = 278;
        uint16_t color = (i == activeIndex) ? DisplayManager::COLOR_ICON_ACTIVE
                                            : DisplayManager::COLOR_ICON_INACTIVE;
        tft.fillRoundRect(tx, ty, 102, 32, 16, DisplayManager::COLOR_BACKGROUND);
        IconRenderer::drawCentered(tft, *icons[i], tx + 51, ty + 16, color);
    }
}

int HomeScreen::activeAudioFamilyIndex() const {
    if (_displayState != DisplayState::Live || _lastStatus.mode.length() == 0) {
        return -1;
    }

    String mode = _lastStatus.mode;
    mode.toLowerCase();
    if (mode.indexOf("dolby") >= 0 || mode.indexOf("atmos") >= 0 ||
        mode.indexOf("truehd") >= 0) {
        return 0;
    }
    if (mode.indexOf("dts") >= 0) {
        return 1;
    }
    if (mode.indexOf("pcm") >= 0 || mode.indexOf("multi ch") >= 0 ||
        mode.indexOf("multichannel") >= 0) {
        return 2;
    }
    return 3;
}
