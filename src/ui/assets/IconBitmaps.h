#ifndef ICON_BITMAPS_H
#define ICON_BITMAPS_H

#include <Arduino.h>

namespace Icons {

struct IconBitmap {
    uint16_t width;
    uint16_t height;
    const uint8_t* data;
    uint16_t dataSize;
};

extern const IconBitmap AUDIO_DOLBY;
extern const IconBitmap AUDIO_DTS;
extern const IconBitmap AUDIO_PCM;
extern const IconBitmap AUDIO_OTHER;
extern const IconBitmap SETTINGS;
extern const IconBitmap CHEVRON_RIGHT;
extern const IconBitmap KEYBOARD_CAPS_LOCK;
extern const IconBitmap KEYBOARD_VISIBILITY;
extern const IconBitmap KEYBOARD_VISIBILITY_OFF;
extern const IconBitmap KEYBOARD_BACKSPACE;
extern const IconBitmap KEYBOARD_OK;
extern const IconBitmap KEYBOARD_CANCEL;
extern const IconBitmap SCAN;
extern const IconBitmap MANUAL_ENTRY;
extern const IconBitmap RETRY;
extern const IconBitmap SUCCESS;
extern const IconBitmap WARNING;
extern const IconBitmap FAILURE;
extern const IconBitmap WIFI;
extern const IconBitmap TOUCH_CALIBRATION;
extern const IconBitmap RECEIVER;

} // namespace Icons

#endif
