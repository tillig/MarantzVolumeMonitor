#include <Arduino.h>
#include "storage/ConfigStore.h"
#include "network/WiFiManager.h"
#include "network/MarantzClient.h"
#include "ui/DisplayManager.h"
#include "ui/TouchManager.h"
#include "ui/ScreenManager.h"
#include "ui/Screens/HomeScreen.h"

DeviceConfig config;
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // 1 second

void setup() {
    Serial.begin(115200);
    
    // Initialize Storage
    if (ConfigStore::getInstance().begin()) {
        ConfigStore::getInstance().loadConfig(config);
    }
    
    // Initialize Display
    DisplayManager::getInstance().begin();
    
    // Initialize Touch
    TouchManager::getInstance().begin();
    
    // Show splash or loading message
    TFT_eSPI& tft = DisplayManager::getInstance().getTft();
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(DisplayManager::COLOR_TEXT_PRIMARY);
    tft.drawString("Connecting to WiFi...", 240, 160, 4);
    
    // Initialize WiFi
    if (config.wifiSsid != "") {
        if (WiFiManager::getInstance().connect(config.wifiSsid, config.wifiPassword)) {
            tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
            tft.drawString("WiFi Connected", 240, 160, 4);
        } else {
            tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
            tft.drawString("WiFi Failed", 240, 160, 4);
        }
    } else {
        tft.fillScreen(DisplayManager::COLOR_BACKGROUND);
        tft.drawString("No WiFi Configured", 240, 160, 4);
    }
    
    // Initialize Marantz Client
    if (config.receiverIp != "") {
        MarantzClient::getInstance().setReceiverIp(config.receiverIp);
    }
    
    // Start with Home Screen
    ScreenManager::getInstance().setScreen(new HomeScreen());
}

void loop() {
    // Handle Touch
    if (TouchManager::getInstance().isTouched()) {
        TS_Point p = TouchManager::getInstance().getPoint();
        
        // Map touch point to screen coordinates
        // This mapping depends on calibration, will need refinement
        int16_t x = map(p.y, 200, 3800, 0, 480);
        int16_t y = map(p.x, 200, 3800, 320, 0);
        
        ScreenManager::getInstance().handleTouch({x, y, p.z});
        delay(100); // Debounce
    }
    
    // Update current screen
    if (millis() - lastUpdate >= UPDATE_INTERVAL) {
        ScreenManager::getInstance().update();
        lastUpdate = millis();
    }
}
