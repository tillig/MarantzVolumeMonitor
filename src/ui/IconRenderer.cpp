#include "IconRenderer.h"
#include <pgmspace.h>

void IconRenderer::draw(TFT_eSPI& tft, const Icons::IconBitmap& icon, int x, int y, uint16_t color) {
    uint16_t rowBytes = (icon.width + 7) / 8;
    for (uint16_t row = 0; row < icon.height; ++row) {
        for (uint16_t col = 0; col < icon.width; ++col) {
            uint16_t byteIndex = row * rowBytes + (col / 8);
            if (byteIndex >= icon.dataSize) {
                return;
            }

            uint8_t value = pgm_read_byte(icon.data + byteIndex);
            if (value & (1 << (7 - (col % 8)))) {
                tft.drawPixel(x + col, y + row, color);
            }
        }
    }
}

void IconRenderer::drawCentered(TFT_eSPI& tft, const Icons::IconBitmap& icon, int centerX, int centerY, uint16_t color) {
    draw(tft, icon, centerX - (icon.width / 2), centerY - (icon.height / 2), color);
}
