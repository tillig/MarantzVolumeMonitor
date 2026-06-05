#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "NetworkListScreen.h"
#include "CalibrationScreen.h"
#include "ReceiverListScreen.h"
#include "../IconRenderer.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"
#include "../../network/WiFiManager.h"

HomeScreen::HomeScreen() {
    _colorIndex = 0;
    _currentLayout = Layout::Unified;
    _textColors[0] = TFT_WHITE;
    _textColors[1] = TFT_GREEN;
    _textColors[2] = TFT_BLUE;
    _textColors[3] = TFT_RED;
    _textColors[4] = TFT_YELLOW;
    _textColors[5] = TFT_MAGENTA;
    _textColors[6] = TFT_CYAN;

    // Default placeholder status for prototyping (Set to 50 for 0-100 scale)
    _lastStatus.volume = 50.0;
    _lastStatus.input = "Blu-ray";
    _lastStatus.mode = "Dolby TrueHD";
    _lastStatus.isValid = false;

    loadStoredConfig();
    refreshState();
}

void HomeScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    if (!_lastStatus.isValid) {
        drawSetupState();
        return;
    }

    switch (_currentLayout) {
        case Layout::Classic: drawLayoutClassic(); break;
        case Layout::Modern:  drawLayoutModern();  break;
        case Layout::Minimal: drawLayoutMinimal(); break;
        case Layout::Unified: drawLayoutUnified(); break;
    }

    drawSettingsButton();
}

void HomeScreen::update() {
    if (millis() - _lastRefreshMs >= 1000) {
        MarantzStatus previousStatus = _lastStatus;
        bool previousHasWifiConfig = _hasWifiConfig;
        bool previousHasReceiverConfig = _hasReceiverConfig;
        bool previousWifiConnected = _isWifiConnected;
        String previousIpAddress = _ipAddress;
        String previousWifiSsid = _config.wifiSsid;
        String previousReceiverIp = _config.receiverIp;

        refreshState();
        if (previousHasWifiConfig != _hasWifiConfig ||
            previousHasReceiverConfig != _hasReceiverConfig ||
            previousWifiConnected != _isWifiConnected ||
            previousIpAddress != _ipAddress ||
            previousWifiSsid != _config.wifiSsid ||
            previousReceiverIp != _config.receiverIp ||
            previousStatus.isValid != _lastStatus.isValid ||
            previousStatus.volume != _lastStatus.volume ||
            previousStatus.input != _lastStatus.input ||
            previousStatus.mode != _lastStatus.mode) {
            draw();
        }
    }
}

void HomeScreen::handleTouch(TS_Point p) {
    if (!_lastStatus.isValid) {
        if (isCalibrationButtonPressed(p)) {
            ScreenManager::getInstance().setScreen(new CalibrationScreen());
        } else if (_isWifiConnected) {
            ScreenManager::getInstance().setScreen(new ReceiverListScreen());
        } else {
            ScreenManager::getInstance().setScreen(new NetworkListScreen());
        }
        return;
    }

    if (isSettingsButtonPressed(p)) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void HomeScreen::drawSetupState() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    if (!_hasWifiConfig) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Warning,
                                       "Unconfigured", "Tap anywhere to configure Wi-Fi.",
                                       Icons::WARNING);
    } else if (_isWifiConnected && !_hasReceiverConfig) {
        MaterialStyle::drawText(tft, "Wi-Fi Connected", 240, 80,
                                MaterialStyle::TextRole::StatusMessage, TC_DATUM,
                                MaterialStyle::ComponentState::Success);
        MaterialStyle::drawText(tft, MaterialStyle::truncateToWidth(tft, _config.wifiSsid, 380, 2),
                                240, 122, MaterialStyle::TextRole::Body, TC_DATUM);
        if (_ipAddress.length() > 0) {
            MaterialStyle::drawText(tft, _ipAddress, 240, 146, MaterialStyle::TextRole::Body, TC_DATUM);
        }

        MaterialStyle::drawText(tft, "Receiver setup is still required.", 240, 188,
                                MaterialStyle::TextRole::Body, TC_DATUM,
                                MaterialStyle::ComponentState::Warning);
        MaterialStyle::drawText(tft, "Tap to configure receiver.", 240, 212,
                                MaterialStyle::TextRole::Body, TC_DATUM);
    } else if (!_isWifiConnected) {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Unavailable,
                                       "Connecting Wi-Fi",
                                       MaterialStyle::truncateToWidth(tft, _config.wifiSsid, 360, 2),
                                       Icons::WIFI);
        MaterialStyle::drawText(tft, "Saved credentials found. Tap to reconfigure Wi-Fi.", 240, 224,
                                MaterialStyle::TextRole::Body, TC_DATUM);
    } else {
        MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Unavailable,
                                       "Receiver unavailable",
                                       "Tap to change receiver settings.", Icons::RECEIVER);
    }

    drawCalibrationButton();
}

void HomeScreen::loadStoredConfig() {
    _config = DeviceConfig();
    ConfigStore::getInstance().loadConfig(_config);
}

void HomeScreen::refreshState() {
    _lastRefreshMs = millis();

    _hasWifiConfig = _config.wifiSsid.length() > 0;
    _hasReceiverConfig = _config.receiverIp.length() > 0;
    _isWifiConnected = WiFiManager::getInstance().isConnected();
    _ipAddress = _isWifiConnected ? WiFiManager::getInstance().getIPAddress() : "";

    _lastStatus.isValid = false;
    if (_hasReceiverConfig) {
        MarantzClient::getInstance().setReceiverIp(_config.receiverIp);
    }

    if (_hasWifiConfig && _hasReceiverConfig && _isWifiConnected) {
        _lastStatus = MarantzClient::getInstance().getStatus();
    }
}

bool HomeScreen::isCalibrationButtonPressed(TS_Point p) const {
    return p.x >= 300 && p.x <= 456 &&
           p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool HomeScreen::isSettingsButtonPressed(TS_Point p) const {
    return p.x >= 438 && p.x <= 462 && p.y >= 16 && p.y <= 40;
}

void HomeScreen::drawLayoutUnified() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // 1. Arc Gauge (Center 240, 140, Radius 125, Thickness 16)
    tft.drawArc(240, 140, 125, 109, 60, 300, DisplayManager::COLOR_BAR_BG, DisplayManager::COLOR_BACKGROUND);
    drawVolumeArc(240, 140, 125, _lastStatus.volume);

    // 2. Large Volume Number (Font 8)
    tft.setTextColor(TFT_WHITE, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawNumber((int)_lastStatus.volume, 240, 125, 8);

    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("VOLUME", 240, 185, 2);

    // 3. Labels
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("Blu-ray", 40, 255, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("Dolby TrueHD", 440, 255, 4);

    // 4. Audio family status icons (10px margin from bottom)
    drawAudioFamilyIcons();
}

void HomeScreen::drawVolumeArc(int x, int y, int r, float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    float percent = volume / 100.0;
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;

    int startAngle = 60;
    int maxSweep = 240;
    int currentSweep = (int)(percent * maxSweep);
    int endAngle = startAngle + currentSweep;

    for (int i = 0; i < currentSweep; i += 8) {
        float p = (float)i / maxSweep;
        uint8_t red, green;
        if (p < 0.5) {
            red = (uint8_t)(p * 2 * 255);
            green = 255;
        } else {
            red = 255;
            green = (uint8_t)((1.0 - (p - 0.5) * 2) * 255);
        }
        uint16_t color = tft.color565(red, green, 0);

        int segStart = startAngle + i;
        int segEnd = segStart + 8;
        if (segEnd > endAngle) segEnd = endAngle;
        tft.drawArc(x, y, r, r-16, segStart, segEnd, color, DisplayManager::COLOR_BACKGROUND);
    }

    float r_mid = r - 8;
    float a1 = (60 + 90) * PI / 180.0;
    tft.fillCircle(x + r_mid * cos(a1), y + r_mid * sin(a1), 8, tft.color565(0, 255, 0));

    float a3 = (300 + 90) * PI / 180.0;
    tft.fillCircle(x + r_mid * cos(a3), y + r_mid * sin(a3), 8, DisplayManager::COLOR_BAR_BG);

    if (currentSweep > 0) {
        float a2 = (endAngle + 90) * PI / 180.0;
        float p = (float)currentSweep / maxSweep;
        uint8_t red, green;
        if (p < 0.5) { red = (uint8_t)(p * 2 * 255); green = 255; }
        else { red = 255; green = (uint8_t)((1.0 - (p - 0.5) * 2) * 255); }
        tft.fillCircle(x + r_mid * cos(a2), y + r_mid * sin(a2), 8, tft.color565(red, green, 0));
    }
}

void HomeScreen::drawLayoutClassic() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(_textColors[_colorIndex], DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0 dB", 240, 80, 7);
    tft.fillRoundRect(72, 140, 336, 24, 4, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(72, 140, 200, 24, 4, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("SRC: Blu-ray", 20, 200, 4);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("MODE: Dolby TrueHD", 460, 200, 4);
    for(int i=0; i<4; i++) {
        tft.drawRoundRect(20 + (i*115), 240, 100, 50, 4, DisplayManager::COLOR_BAR_BG);
    }
}

void HomeScreen::drawLayoutModern() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillRoundRect(24, 150, 432, 40, 6, DisplayManager::COLOR_BAR_BG);
    tft.fillRoundRect(24, 150, 300, 40, 6, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("-45.0 dB", 240, 140, 7);
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Blu-ray", 20, 10, 4);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("Dolby TrueHD", 460, 10, 4);
    for(int i=0; i<4; i++) {
        tft.fillRoundRect(20 + (i*115), 230, 100, 40, 20, DisplayManager::COLOR_BAR_BG);
    }
}

void HomeScreen::drawLayoutMinimal() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(_textColors[_colorIndex]);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("-45.0", 240, 160, 8);
    tft.fillRect(0, 310, 480, 10, DisplayManager::COLOR_BAR_BG);
    tft.fillRect(0, 310, 320, 10, DisplayManager::COLOR_ACCENT);
    tft.setTextColor(DisplayManager::COLOR_TEXT_DIMMED);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("BLU-RAY", 10, 10, 2);
    tft.setTextDatum(TR_DATUM);
    tft.drawString("DOLBY TRUEHD", 470, 10, 2);
}

void HomeScreen::drawVolume(float volume) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MC_DATUM);
    char volStr[10];
    dtostrf(volume, 4, 1, volStr);
    String displayVol = String(volStr) + " dB";
    tft.drawString(displayVol, 240, 80, 7);
    int barWidth = 336;
    int barHeight = 24;
    int barX = (480 - barWidth) / 2;
    int barY = 140;
    tft.fillRoundRect(barX, barY, barWidth, barHeight, 4, DisplayManager::COLOR_BAR_BG);
    float percent = (volume + 80) / 98.0;
    if (percent < 0) percent = 0;
    if (percent > 1) percent = 1;
    tft.fillRoundRect(barX, barY, (int)(barWidth * percent), barHeight, 4, DisplayManager::COLOR_ACCENT);
}

void HomeScreen::drawSource(const String& source) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(ML_DATUM);
    tft.drawString("SRC: " + source, 20, 200, 4);
}

void HomeScreen::drawMode(const String& mode) {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, DisplayManager::COLOR_BACKGROUND);
    tft.setTextDatum(MR_DATUM);
    tft.drawString("MODE: " + mode, 460, 200, 4);
}

void HomeScreen::drawTiles(const String& mode) {
}

void HomeScreen::drawSettingsButton() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    IconRenderer::drawCentered(tft, Icons::SETTINGS, 450, 28, DisplayManager::COLOR_TEXT_SECONDARY);
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
    if (!_lastStatus.isValid || _lastStatus.mode.length() == 0) {
        return -1;
    }

    String mode = _lastStatus.mode;
    mode.toLowerCase();
    if (mode.indexOf("dolby") >= 0 || mode.indexOf("atmos") >= 0 || mode.indexOf("truehd") >= 0) {
        return 0;
    }
    if (mode.indexOf("dts") >= 0) {
        return 1;
    }
    if (mode.indexOf("pcm") >= 0 || mode.indexOf("multi ch") >= 0 || mode.indexOf("multichannel") >= 0) {
        return 2;
    }
    return 3;
}
