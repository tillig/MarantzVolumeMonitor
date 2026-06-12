#include "SettingsScreen.h"
#include "CalibrationScreen.h"
#include "CurrentSettingsScreen.h"
#include "HomeScreen.h"
#include "NetworkListScreen.h"
#include "ResetDefaultsScreen.h"
#include "ReceiverListScreen.h"
#include "VolumeScaleScreen.h"
#include "../ScreenManager.h"
#include "../MaterialStyle.h"
#include "../assets/IconBitmaps.h"

namespace {

constexpr int SettingsListTopY = 68;

int entryY(int indexOnPage) {
    return SettingsListTopY + (indexOnPage * (MaterialStyle::ListRowHeight + MaterialStyle::RowGap));
}

} // namespace

void SettingsScreen::draw() {
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(DisplayManager::COLOR_BACKGROUND);

    MaterialStyle::drawText(tft, "Settings", 20, 20, MaterialStyle::TextRole::ScreenTitle, TL_DATUM);

    int startIndex = pageStartIndex();
    for (int i = 0; i < ItemsPerPage; ++i) {
        int entryIndex = startIndex + i;
        if (entryIndex >= EntryCount) {
            break;
        }
        drawEntry(tft, entryIndex, entryY(i));
    }

    MaterialStyle::drawPagination(tft, _pageIndex, totalPages(), _pageIndex > 0, _pageIndex < totalPages() - 1);

    MaterialStyle::drawStandardButton(tft,
                                      340,
                                      MaterialStyle::BottomActionY,
                                      116,
                                      MaterialStyle::ButtonHeight,
                                      Icons::KEYBOARD_OK,
                                      "OK",
                                      MaterialStyle::ComponentState::Success);
}

void SettingsScreen::update() {
    // Nothing to poll here usually
}

void SettingsScreen::handleTouch(TS_Point p) {
    if (isOkPressed(p)) {
        ScreenManager::getInstance().setScreen(new HomeScreen());
        return;
    }

    bool goPrev = false;
    bool goNext = false;
    if (isPaginationPressed(p, goPrev, goNext)) {
        if (goPrev) {
            _pageIndex--;
            draw();
        } else if (goNext) {
            _pageIndex++;
            draw();
        }
        return;
    }

    int entryIndex = touchedEntryIndex(p);
    if (entryIndex >= 0) {
        openEntry(entryIndex);
    }
}

void SettingsScreen::drawEntry(TFT_eSPI& tft, int entryIndex, int y) {
    const Icons::IconBitmap* icon = &Icons::SETTINGS;
    const char* primary = "Current Settings";
    const char* secondary = "View Wi-Fi and receiver state";

    if (entryIndex == 1) {
        icon = &Icons::SETTINGS;
        primary = "Volume Display Scale";
        secondary = "Choose 0-100 or dB";
    } else if (entryIndex == 2) {
        icon = &Icons::WIFI;
        primary = "Wi-Fi Setup";
        secondary = "Choose network and password";
    } else if (entryIndex == 3) {
        icon = &Icons::RECEIVER;
        primary = "Receiver Setup";
        secondary = "Discover or enter receiver";
    } else if (entryIndex == 4) {
        icon = &Icons::TOUCH_CALIBRATION;
        primary = "Touch Calibration";
        secondary = "Align touch on the device";
    } else if (entryIndex == 5) {
        icon = &Icons::WARNING;
        primary = "Reset To Defaults";
        secondary = "Reset Wi-Fi, receiver, or touch";
    }

    MaterialStyle::drawListRow(tft,
                               {20,
                                y,
                                440,
                                MaterialStyle::ListRowHeight,
                                icon,
                                0,
                                primary,
                                secondary,
                                ">",
                                MaterialStyle::ComponentState::Normal});
}

void SettingsScreen::openEntry(int entryIndex) {
    switch (entryIndex) {
        case 0:
            ScreenManager::getInstance().setScreen(new CurrentSettingsScreen());
            return;
        case 1:
            ScreenManager::getInstance().setScreen(new VolumeScaleScreen());
            return;
        case 2:
            ScreenManager::getInstance().setScreen(new NetworkListScreen(ScreenReturnTarget::Settings));
            return;
        case 3:
            ScreenManager::getInstance().setScreen(new ReceiverListScreen(ScreenReturnTarget::Settings));
            return;
        case 4:
            ScreenManager::getInstance().setScreen(new CalibrationScreen(ScreenReturnTarget::Settings));
            return;
        case 5:
            ScreenManager::getInstance().setScreen(new ResetDefaultsScreen());
            return;
        default:
            return;
    }
}

int SettingsScreen::totalPages() const {
    return (EntryCount + ItemsPerPage - 1) / ItemsPerPage;
}

int SettingsScreen::pageStartIndex() const {
    return _pageIndex * ItemsPerPage;
}

bool SettingsScreen::isOkPressed(TS_Point p) const {
    return p.x >= 340 && p.x <= 456 && p.y >= MaterialStyle::BottomActionY &&
           p.y <= MaterialStyle::BottomActionY + MaterialStyle::ButtonHeight;
}

bool SettingsScreen::isPaginationPressed(TS_Point p, bool& goPrev, bool& goNext) const {
    goPrev = false;
    goNext = false;
    if (totalPages() <= 1 || p.y < 230 || p.y > 262) {
        return false;
    }

    if (p.x >= 20 && p.x <= 116 && _pageIndex > 0) {
        goPrev = true;
        return true;
    }
    if (p.x >= 364 && p.x <= 460 && _pageIndex < totalPages() - 1) {
        goNext = true;
        return true;
    }
    return false;
}

int SettingsScreen::touchedEntryIndex(TS_Point p) const {
    int startIndex = pageStartIndex();
    for (int i = 0; i < ItemsPerPage; ++i) {
        int entryIndex = startIndex + i;
        if (entryIndex >= EntryCount) {
            break;
        }

        int y = entryY(i);
        if (p.x >= 20 && p.x <= 460 && p.y >= y && p.y <= y + MaterialStyle::ListRowHeight) {
            return entryIndex;
        }
    }

    return -1;
}
