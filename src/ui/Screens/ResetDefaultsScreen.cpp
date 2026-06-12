#include "ResetDefaultsScreen.h"

#include "SettingsScreen.h"
#include "../IconRenderer.h"
#include "../MaterialStyle.h"
#include "../ScreenManager.h"
#include "../TouchManager.h"
#include "../assets/IconBitmaps.h"

namespace {
constexpr int RowX = 20;
constexpr int RowWidth = 440;
constexpr int RowTopY = 84;

int rowY(int index) {
    return RowTopY + (index * (MaterialStyle::ListRowHeight + MaterialStyle::RowGap));
}
} // namespace

void ResetDefaultsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    switch (_mode) {
        case Mode::ChoosingType:
            drawSelection(tft);
            break;
        case Mode::Confirming:
            drawConfirmation(tft);
            break;
        case Mode::Completed:
            drawCompleted(tft);
            break;
    }
}

void ResetDefaultsScreen::update() {
    // Static while visible.
}

void ResetDefaultsScreen::handleTouch(TS_Point p) {
    if (_mode == Mode::ChoosingType) {
        for (int i = 0; i < 3; ++i) {
            if (isChoicePressed(p, i)) {
                _selectedType = static_cast<ResetType>(i);
                _applyFailed = false;
                _mode = Mode::Confirming;
                draw();
                return;
            }
        }

        if (isCancelPressed(p)) {
            ScreenManager::getInstance().setScreen(new SettingsScreen());
        }
        return;
    }

    if (_mode == Mode::Confirming) {
        if (isCancelPressed(p)) {
            _mode = Mode::ChoosingType;
            _applyFailed = false;
            draw();
            return;
        }

        if (isResetPressed(p)) {
            applyReset();
            draw();
        }
        return;
    }

    if (_mode == Mode::Completed && isOkPressed(p)) {
        ScreenManager::getInstance().setScreen(new SettingsScreen());
    }
}

void ResetDefaultsScreen::drawSelection(TFT_eSPI& tft) {
    MaterialStyle::drawPageHeader(tft, Icons::WARNING, "Reset To Defaults", "Choose one settings type to reset");

    for (int i = 0; i < 3; ++i) {
        ResetType type = static_cast<ResetType>(i);
        const Icons::IconBitmap* icon = type == ResetType::Wifi       ? &Icons::WIFI
                                        : type == ResetType::Receiver ? &Icons::RECEIVER
                                                                      : &Icons::TOUCH_CALIBRATION;
        MaterialStyle::drawListRow(tft,
                                   {RowX,
                                    rowY(i),
                                    RowWidth,
                                    MaterialStyle::ListRowHeight,
                                    icon,
                                    0,
                                    labelFor(type),
                                    detailFor(type),
                                    ">",
                                    MaterialStyle::ComponentState::Normal});
    }

    MaterialStyle::drawStandardButton(tft,
                                      172,
                                      MaterialStyle::BottomActionY,
                                      136,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_CANCEL,
                                      "Cancel",
                                      MaterialStyle::ComponentState::Error);
}

void ResetDefaultsScreen::drawConfirmation(TFT_eSPI& tft) {
    String subtitle = String("Reset ") + labelFor(_selectedType) + "?";
    MaterialStyle::drawPageHeader(tft, Icons::WARNING, "Reset To Defaults", subtitle);

    String message = String("Only ") + labelFor(_selectedType) + " will be cleared.";
    MaterialStyle::drawStatusBlock(tft, MaterialStyle::StatusKind::Warning, "Confirm reset", message, Icons::WARNING);

    if (_applyFailed) {
        MaterialStyle::drawText(tft,
                                "Reset failed. Try again.",
                                240,
                                222,
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
                                      Icons::WARNING,
                                      "Reset",
                                      MaterialStyle::ComponentState::Warning);
}

void ResetDefaultsScreen::drawCompleted(TFT_eSPI& tft) {
    MaterialStyle::drawPageHeader(tft, Icons::SUCCESS, "Reset To Defaults", "The selected defaults are restored");
    MaterialStyle::drawStatusBlock(
        tft, MaterialStyle::StatusKind::Success, "Reset complete", "Press OK to return to Settings.", Icons::SUCCESS);
    MaterialStyle::drawStandardButton(tft,
                                      182,
                                      MaterialStyle::BottomActionY,
                                      116,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "OK",
                                      MaterialStyle::ComponentState::Success);
}

bool ResetDefaultsScreen::isChoicePressed(TS_Point p, int index) const {
    int y = rowY(index);
    return p.x >= RowX && p.x <= RowX + RowWidth && p.y >= y && p.y <= y + MaterialStyle::ListRowHeight;
}

bool ResetDefaultsScreen::isCancelPressed(TS_Point p) const {
    if (_mode == Mode::ChoosingType) {
        return p.x >= 172 && p.x <= 308 && p.y >= MaterialStyle::BottomActionY &&
               p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
    }

    return p.x >= 44 && p.x <= 180 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool ResetDefaultsScreen::isResetPressed(TS_Point p) const {
    return p.x >= 300 && p.x <= 436 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool ResetDefaultsScreen::isOkPressed(TS_Point p) const {
    return p.x >= 182 && p.x <= 298 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

const char* ResetDefaultsScreen::labelFor(ResetType type) const {
    switch (type) {
        case ResetType::Wifi:
            return "Wi-Fi";
        case ResetType::Receiver:
            return "Receiver";
        case ResetType::Calibration:
        default:
            return "Calibration";
    }
}

const char* ResetDefaultsScreen::detailFor(ResetType type) const {
    switch (type) {
        case ResetType::Wifi:
            return "Clear saved network credentials";
        case ResetType::Receiver:
            return "Clear saved receiver address";
        case ResetType::Calibration:
        default:
            return "Restore shipped touch profile";
    }
}

ConfigStore::ResetTarget ResetDefaultsScreen::configTargetFor(ResetType type) const {
    switch (type) {
        case ResetType::Wifi:
            return ConfigStore::ResetTarget::Wifi;
        case ResetType::Receiver:
            return ConfigStore::ResetTarget::Receiver;
        case ResetType::Calibration:
        default:
            return ConfigStore::ResetTarget::Calibration;
    }
}

void ResetDefaultsScreen::applyReset() {
    _applyFailed = !ConfigStore::getInstance().resetToDefaults(configTargetFor(_selectedType));
    if (_applyFailed) {
        return;
    }

    if (_selectedType == ResetType::Calibration) {
        TouchManager::getInstance().applyDefaultCalibration();
    }

    _mode = Mode::Completed;
}
