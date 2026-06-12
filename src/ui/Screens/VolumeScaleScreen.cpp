#include "VolumeScaleScreen.h"

#include "SettingsScreen.h"
#include "../IconRenderer.h"
#include "../MaterialStyle.h"
#include "../ScreenManager.h"
#include "../assets/IconBitmaps.h"

namespace {
constexpr int ChoiceRowX = 20;
constexpr int ChoiceRowWidth = 440;
constexpr int ChoiceRowTopY = 84;
constexpr int ChoiceRowGap = MaterialStyle::RowGap;

int choiceRowY(int index) {
    return ChoiceRowTopY + (index * (MaterialStyle::ListRowHeight + ChoiceRowGap));
}
} // namespace

void VolumeScaleScreen::draw() {
    if (!_initialized) {
        loadChoice();
    }

    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawPageHeader(tft, Icons::SETTINGS, "Volume Display Scale", "Choose the home volume number format");

    MaterialStyle::drawChoiceRow(tft,
                                 {ChoiceRowX,
                                  choiceRowY(0),
                                  ChoiceRowWidth,
                                  MaterialStyle::ListRowHeight,
                                  "0-100",
                                  "Normalized home scale",
                                  _pendingChoice == Choice::Normalized,
                                  MaterialStyle::ComponentState::Normal});
    MaterialStyle::drawChoiceRow(tft,
                                 {ChoiceRowX,
                                  choiceRowY(1),
                                  ChoiceRowWidth,
                                  MaterialStyle::ListRowHeight,
                                  "dB",
                                  "Raw receiver volume",
                                  _pendingChoice == Choice::Db,
                                  MaterialStyle::ComponentState::Normal});

    if (_saveFailed) {
        MaterialStyle::drawText(tft,
                                "Save failed. Try again.",
                                240,
                                206,
                                MaterialStyle::TextRole::Body,
                                TC_DATUM,
                                MaterialStyle::ComponentState::Error);
    }

    MaterialStyle::drawStandardButton(tft,
                                      44,
                                      MaterialStyle::BottomActionY,
                                      136,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_CANCEL,
                                      "Cancel",
                                      MaterialStyle::ComponentState::Error);
    MaterialStyle::drawStandardButton(tft,
                                      300,
                                      MaterialStyle::BottomActionY,
                                      136,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "OK",
                                      MaterialStyle::ComponentState::Success);
}

void VolumeScaleScreen::update() {
    // This screen is static while visible.
}

void VolumeScaleScreen::handleTouch(TS_Point p) {
    if (isNormalizedPressed(p)) {
        _pendingChoice = Choice::Normalized;
        _saveFailed = false;
        draw();
        return;
    }

    if (isDbPressed(p)) {
        _pendingChoice = Choice::Db;
        _saveFailed = false;
        draw();
        return;
    }

    if (isCancelPressed(p)) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
        return;
    }

    if (isOkPressed(p)) {
        if (persistChoice()) {
            ScreenManager::getInstance().setScreen(new SettingsScreen());
        } else {
            _saveFailed = true;
            draw();
        }
    }
}

void VolumeScaleScreen::loadChoice() {
    DeviceConfig config;
    ConfigStore::getInstance().loadConfig(config);

    _savedChoice = config.useDbScale ? Choice::Db : Choice::Normalized;
    _pendingChoice = _savedChoice;
    _initialized = true;
    _saveFailed = false;
}

bool VolumeScaleScreen::isNormalizedPressed(TS_Point p) const {
    int y = choiceRowY(0);
    return p.x >= ChoiceRowX && p.x <= ChoiceRowX + ChoiceRowWidth && p.y >= y &&
           p.y <= y + MaterialStyle::ListRowHeight;
}

bool VolumeScaleScreen::isDbPressed(TS_Point p) const {
    int y = choiceRowY(1);
    return p.x >= ChoiceRowX && p.x <= ChoiceRowX + ChoiceRowWidth && p.y >= y &&
           p.y <= y + MaterialStyle::ListRowHeight;
}

bool VolumeScaleScreen::isCancelPressed(TS_Point p) const {
    return p.x >= 44 && p.x <= 180 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool VolumeScaleScreen::isOkPressed(TS_Point p) const {
    return p.x >= 300 && p.x <= 436 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool VolumeScaleScreen::persistChoice() {
    DeviceConfig config;
    ConfigStore::getInstance().loadConfig(config);
    config.useDbScale = useDbScaleFor(_pendingChoice);
    if (!ConfigStore::getInstance().saveConfig(config)) {
        return false;
    }

    _savedChoice = _pendingChoice;
    return true;
}

bool VolumeScaleScreen::useDbScaleFor(Choice choice) const {
    return choice == Choice::Db;
}
