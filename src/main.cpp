#include <Arduino.h>
#include "ui/DisplayManager.h"
#include "ui/TouchManager.h"
#include "ui/ScreenManager.h"
#include "ui/Screens/HomeScreen.h"

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("Marantz Volume Monitor v2 Starting...");

    // Initialize Hardware via Managers
    DisplayManager::getInstance().begin();
    TouchManager::getInstance().begin();

    // Initial Screen (Start with HomeScreen for prototyping)
    ScreenManager::getInstance().setScreen(new HomeScreen());

    Serial.println("System Initialization Complete");
}

void loop() {
    // 1. Handle Touch (forwarding to ScreenManager)
    if (TouchManager::getInstance().isTouched()) {
        TS_Point p = TouchManager::getInstance().getPoint();
        ScreenManager::getInstance().handleTouch(p);
    }

    // 2. Update Active Screen (polling, animations, etc.)
    ScreenManager::getInstance().update();

    // Small delay to keep loop responsive but not spinning
    delay(10);
}
