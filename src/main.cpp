#include <Arduino.h>
#include "ui/DisplayManager.h"
#include "ui/TouchManager.h"

// Hardware Spike State
uint16_t textColors[] = {TFT_WHITE, TFT_GREEN, TFT_BLUE, TFT_RED, TFT_YELLOW, TFT_MAGENTA, TFT_CYAN};
int colorIndex = 0;
bool lastTouchState = false;

void setup() {
    Serial.begin(115200);
    delay(1000); // Give serial monitor time to connect
    Serial.println("Hardware Spike Started");

    // Initialize Display
    Serial.println("Initializing Display...");
    DisplayManager::getInstance().begin();
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();

    // Clear screen and draw initial text
    tft.fillScreen(TFT_BLACK);
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(textColors[colorIndex]);
    tft.drawString("Hello World", 240, 160, 4); // Large font (Font 4)
    Serial.println("Hello World drawn to display");

    // Initialize Touch
    Serial.println("Initializing Touch...");
    TouchManager::getInstance().begin();
    Serial.println("Hardware initialization complete");
}

void loop() {
    bool isTouched = TouchManager::getInstance().isTouched();

    // Check for new touch event (rising edge)
    if (isTouched && !lastTouchState) {
        TS_Point p = TouchManager::getInstance().getPoint();
        Serial.print("Screen Touch at: x=");
        Serial.print(p.x);
        Serial.print(", y=");
        Serial.println(p.y);

        // Cycle color
        colorIndex = (colorIndex + 1) % (sizeof(textColors) / sizeof(textColors[0]));

        // Update display
        TFT_eSPI& tft = DisplayManager::getInstance().getTft();
        tft.setTextColor(textColors[colorIndex]);

        // Redraw text over background to avoid clearing whole screen (less flicker)
        tft.fillScreen(TFT_BLACK);
        tft.drawString("Hello World", 240, 160, 4);

        Serial.print("Text color changed. Index: ");
        Serial.println(colorIndex);

        // Simple debounce
        delay(200);
    }

    lastTouchState = isTouched;

    // Small delay to prevent tight loop
    delay(10);
}
