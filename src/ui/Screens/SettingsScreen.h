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
    static constexpr int EntryCount = 5;

    int _pageIndex = 0;

    void drawEntry(TFT_eSPI& tft, int entryIndex, int y);
    void openEntry(int entryIndex);
    int totalPages() const;
    int pageStartIndex() const;
    bool isOkPressed(TS_Point p) const;
    bool isPaginationPressed(TS_Point p, bool& goPrev, bool& goNext) const;
    int touchedEntryIndex(TS_Point p) const;
};

#endif
