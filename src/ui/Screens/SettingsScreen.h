#ifndef SETTINGS_SCREEN_H
#define SETTINGS_SCREEN_H

#include "Screen.h"

class SettingsScreen : public Screen {
public:
    void draw() override;
    void update() override;
    void handleTouch(TS_Point p) override;

private:
    static constexpr int ItemsPerPage = 3;
    static constexpr int EntryCount = 6;

    int _pageIndex = 0;

    static void drawEntry(TFT_eSPI& tft, int entryIndex, int y);
    static void openEntry(int entryIndex);
    static int totalPages();
    int pageStartIndex() const;
    static bool isOkPressed(TS_Point p);
    bool isPaginationPressed(TS_Point p, bool& goPrev, bool& goNext) const;
    int touchedEntryIndex(TS_Point p) const;
};

#endif
