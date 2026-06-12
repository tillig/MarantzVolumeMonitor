#include "IconRenderer.h"
#include <pgmspace.h>

namespace {
bool iconPixelSet(const Icons::IconBitmap& icon, uint16_t x, uint16_t y) {
    uint16_t rowBytes = (icon.width + 7) / 8;
    uint16_t byteIndex = y * rowBytes + (x / 8);
    if (byteIndex >= icon.dataSize) {
        return false;
    }

    uint8_t value = pgm_read_byte(icon.data + byteIndex);
    return (value & (1 << (7 - (x % 8)))) != 0;
}
}

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

void IconRenderer::drawFittedCentered(TFT_eSPI& tft, const Icons::IconBitmap& icon,
                                      int centerX, int centerY, int maxWidth, int maxHeight,
                                      uint16_t color) {
    if (icon.width <= maxWidth && icon.height <= maxHeight) {
        drawCentered(tft, icon, centerX, centerY, color);
        return;
    }

    int targetWidth = maxWidth;
    int targetHeight = (icon.height * maxWidth) / icon.width;
    if (targetHeight > maxHeight) {
        targetHeight = maxHeight;
        targetWidth = (icon.width * maxHeight) / icon.height;
    }

    if (targetWidth <= 0 || targetHeight <= 0) {
        return;
    }

    int originX = centerX - (targetWidth / 2);
    int originY = centerY - (targetHeight / 2);
    for (int dy = 0; dy < targetHeight; ++dy) {
        uint16_t srcY = static_cast<uint16_t>((dy * icon.height) / targetHeight);
        for (int dx = 0; dx < targetWidth; ++dx) {
            uint16_t srcX = static_cast<uint16_t>((dx * icon.width) / targetWidth);
            if (iconPixelSet(icon, srcX, srcY)) {
                tft.drawPixel(originX + dx, originY + dy, color);
            }
        }
    }
}
