#include <Arduino.h>
#include "network/MarantzClient.h"
#include "network/WiFiManager.h"
#include "storage/ConfigStore.h"
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
    ConfigStore::getInstance().begin();

    DeviceConfig config;
    if (ConfigStore::getInstance().loadConfig(config)) {
        if (config.wifiSsid.length() > 0) {
            Serial.print("Attempting WiFi auto-connect to ");
            Serial.println(config.wifiSsid);
            WiFiManager::getInstance().startConnect(config.wifiSsid, config.wifiPassword);
        }

        if (config.receiverIp.length() > 0) {
            MarantzClient::getInstance().setReceiverIp(config.receiverIp);
        }
    }

    // Initial Screen (Start with HomeScreen for prototyping)
    ScreenManager::getInstance().setScreen(new HomeScreen());

    Serial.println("System Initialization Complete");
}

void loop() {
    static bool lastTouchState = false;
    static uint32_t lastTouchDispatchMs = 0;
    static constexpr uint32_t TouchDebounceMs = 175;

    // 1. Handle Touch (forwarding to ScreenManager)
    bool isTouched = TouchManager::getInstance().isTouched();
    if (isTouched && !lastTouchState && (millis() - lastTouchDispatchMs) >= TouchDebounceMs) {
        TS_Point p = TouchManager::getInstance().getPoint();
        ScreenManager::getInstance().handleTouch(p);
        lastTouchDispatchMs = millis();
    }
    lastTouchState = isTouched;

    // 2. Update Active Screen (polling, animations, etc.)
    ScreenManager::getInstance().update();

    // Small delay to keep loop responsive but not spinning
    delay(10);
}
