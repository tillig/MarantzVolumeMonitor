#ifndef ICON_RENDERER_H
#define ICON_RENDERER_H

#include <TFT_eSPI.h>
#include "assets/IconBitmaps.h"

class IconRenderer {
public:
    static void draw(TFT_eSPI& tft, const Icons::IconBitmap& icon, int x, int y, uint16_t color);
    static void drawCentered(TFT_eSPI& tft, const Icons::IconBitmap& icon, int centerX, int centerY, uint16_t color);
    static void drawFittedCentered(TFT_eSPI& tft,
                                   const Icons::IconBitmap& icon,
                                   int centerX,
                                   int centerY,
                                   int maxWidth,
                                   int maxHeight,
                                   uint16_t color);
};

#endif
