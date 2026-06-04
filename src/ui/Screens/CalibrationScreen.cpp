#include "CalibrationScreen.h"

#include <Arduino.h>

#include "HomeScreen.h"
#include "SettingsScreen.h"
#include "../DisplayManager.h"
#include "../ScreenManager.h"
#include "../TouchManager.h"

namespace {
uint16_t average3(uint16_t a, uint16_t b, uint16_t c) {
    return static_cast<uint16_t>((static_cast<uint32_t>(a) + b + c) / 3U);
}
}

CalibrationScreen::CalibrationScreen(ScreenReturnTarget returnTarget)
    : _returnTarget(returnTarget) {}

void CalibrationScreen::draw() {
    if (!_printedHeader) {
        printSessionHeader();
        _printedHeader = true;
    }

    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextDatum(TC_DATUM);
    tft.drawString("TOUCH CALIBRATION", 240, 12, 2);

    if (_isComplete) {
        tft.drawString("Capture complete", 240, 90, 4);
        tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, TFT_BLACK);
        tft.drawString("Copy the serial log from this run.", 240, 140, 2);
        String returnText = _returnTarget == ScreenReturnTarget::Settings ? "Tap anywhere to return to Settings."
                                                                          : "Tap anywhere to return home.";
        tft.drawString(returnText, 240, 170, 2);
        tft.drawString("No new constants are applied yet.", 240, 200, 2);
        return;
    }

    const CalibrationPoint& point = _points[_step];

    tft.drawString("Tap the center of each crosshair once.", 240, 40, 2);
    tft.drawString("Serial capture only. This does not recalibrate live.", 240, 62, 2);
    tft.drawString("Use a deliberate press and release.", 240, 84, 2);

    tft.setTextColor(DisplayManager::COLOR_TEXT_SECONDARY, TFT_BLACK);
    String progress = "Point " + String(_step + 1) + " of " + String(PointCount);
    tft.drawString(progress, 240, 116, 2);
    tft.drawString(point.label, 240, 138, 2);

    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawLine(point.x - 14, point.y, point.x + 14, point.y, TFT_RED);
    tft.drawLine(point.x, point.y - 14, point.x, point.y + 14, TFT_RED);
    tft.drawCircle(point.x, point.y, 8, TFT_RED);
}

void CalibrationScreen::update() {}

void CalibrationScreen::handleTouch(TS_Point p) {
    if (_isComplete) {
        if (_returnTarget == ScreenReturnTarget::Settings) {
            ScreenManager::getInstance().setScreen(new SettingsScreen());
        } else {
            ScreenManager::getInstance().setScreen(new HomeScreen());
        }
        return;
    }

    TS_Point raw = TouchManager::getInstance().getRawPoint();
    const CalibrationPoint& point = _points[_step];
    CalibrationSample& sample = _samples[_step];

    sample.rawX = static_cast<uint16_t>(raw.x);
    sample.rawY = static_cast<uint16_t>(raw.y);
    sample.rawZ = static_cast<uint16_t>(raw.z);
    sample.mappedX = p.x;
    sample.mappedY = p.y;

    Serial.print("CAL_POINT,");
    Serial.print(_step);
    Serial.print(",");
    Serial.print(point.label);
    Serial.print(",");
    Serial.print(point.x);
    Serial.print(",");
    Serial.print(point.y);
    Serial.print(",");
    Serial.print(sample.rawX);
    Serial.print(",");
    Serial.print(sample.rawY);
    Serial.print(",");
    Serial.print(sample.rawZ);
    Serial.print(",");
    Serial.print(sample.mappedX);
    Serial.print(",");
    Serial.println(sample.mappedY);

    while (TouchManager::getInstance().isTouched()) {
        delay(10);
    }
    delay(150);

    _step++;
    if (_step >= PointCount) {
        _isComplete = true;
        printSummary();
    }

    draw();
}

void CalibrationScreen::printSessionHeader() {
    Serial.println("CALIBRATION_SESSION_BEGIN");
    Serial.println("Format: CAL_POINT,index,label,targetX,targetY,rawX,rawY,rawZ,mappedX,mappedY");
    Serial.println("Touch each crosshair once in the displayed order.");
}

void CalibrationScreen::printSummary() {
    const uint16_t leftX = average3(_samples[0].rawX, _samples[3].rawX, _samples[6].rawX);
    const uint16_t centerX = average3(_samples[1].rawX, _samples[4].rawX, _samples[7].rawX);
    const uint16_t rightX = average3(_samples[2].rawX, _samples[5].rawX, _samples[8].rawX);
    const uint16_t topY = average3(_samples[0].rawY, _samples[1].rawY, _samples[2].rawY);
    const uint16_t centerY = average3(_samples[3].rawY, _samples[4].rawY, _samples[5].rawY);
    const uint16_t bottomY = average3(_samples[6].rawY, _samples[7].rawY, _samples[8].rawY);

    Serial.println("CALIBRATION_SUMMARY_BEGIN");
    Serial.print("RAW_X_COLUMNS,left=");
    Serial.print(leftX);
    Serial.print(",center=");
    Serial.print(centerX);
    Serial.print(",right=");
    Serial.println(rightX);

    Serial.print("RAW_Y_ROWS,top=");
    Serial.print(topY);
    Serial.print(",center=");
    Serial.print(centerY);
    Serial.print(",bottom=");
    Serial.println(bottomY);

    Serial.print("CURRENT_AFFINE,x_raw_x=");
    Serial.print(TouchManager::getXFromRawX(), 10);
    Serial.print(",x_raw_y=");
    Serial.print(TouchManager::getXFromRawY(), 10);
    Serial.print(",x_offset=");
    Serial.print(TouchManager::getXOffset(), 10);
    Serial.print(",y_raw_x=");
    Serial.print(TouchManager::getYFromRawX(), 10);
    Serial.print(",y_raw_y=");
    Serial.print(TouchManager::getYFromRawY(), 10);
    Serial.print(",y_offset=");
    Serial.println(TouchManager::getYOffset(), 10);
    Serial.println("CALIBRATION_SUMMARY_END");
}
