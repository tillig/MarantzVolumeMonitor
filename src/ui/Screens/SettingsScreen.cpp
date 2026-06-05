#include "SettingsScreen.h"
#include "CalibrationScreen.h"
#include "HomeScreen.h"
#include "NetworkListScreen.h"
#include "ReceiverListScreen.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"

void SettingsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawText(tft, "Settings", 20, 20, MaterialStyle::TextRole::ScreenTitle, TL_DATUM);

    MaterialStyle::drawListRow(tft, {
        20, 68, 440, MaterialStyle::ListRowHeight,
        &Icons::WIFI,
        0,
        "Wi-Fi Setup",
        "Choose network and password",
        ">",
        MaterialStyle::ComponentState::Normal
    });
    MaterialStyle::drawListRow(tft, {
        20, 128, 440, MaterialStyle::ListRowHeight,
        &Icons::RECEIVER,
        0,
        "Receiver Setup",
        "Discover or enter receiver",
        ">",
        MaterialStyle::ComponentState::Normal
    });
    MaterialStyle::drawListRow(tft, {
        20, 188, 440, MaterialStyle::ListRowHeight,
        &Icons::TOUCH_CALIBRATION,
        0,
        "Touch Calibration",
        "Capture raw touch samples",
        ">",
        MaterialStyle::ComponentState::Normal
    });

    MaterialStyle::drawStandardButton(tft, 340, MaterialStyle::BottomActionY, 116, MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK, "OK",
                                      MaterialStyle::ComponentState::Success);
}

void SettingsScreen::update() {
    // Nothing to poll here usually
}

void SettingsScreen::handleTouch(TS_Point p) {
    if (p.x >= 340 && p.x <= 456 && p.y >= MaterialStyle::BottomActionY &&
        p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight) {
        ScreenManager::getInstance().setScreen(new HomeScreen());
    } else if (p.y >= 68 && p.y <= 114) {
        ScreenManager::getInstance().setScreen(new NetworkListScreen(ScreenReturnTarget::Settings));
    } else if (p.y >= 128 && p.y <= 174) {
        ScreenManager::getInstance().setScreen(new ReceiverListScreen(ScreenReturnTarget::Settings));
    } else if (p.y >= 188 && p.y <= 234) {
        ScreenManager::getInstance().setScreen(new CalibrationScreen(ScreenReturnTarget::Settings));
    }
}
