#include <Arduino.h>
#include "Config.h"
#include "DisplayManager.h"
#include "NFCReader.h"
#include "StorageManager.h"
#include "ButtonHandler.h"
#include "MenuSystem.h"

// ========== GLOBAL OBJECTS ==========
DisplayManager display;
NFCReader nfcReader;
StorageManager storage;
ButtonHandler buttons;
MenuSystem menu(display, buttons, nfcReader, storage);

// ========== SETUP ==========
void setup() {
  Serial.begin(115200);
  delay(500); // Wait for serial to stabilize
  
  Serial.println(F("==================="));
  Serial.println(F("NFC System v2.0"));
  Serial.println(F("==================="));
  
  // Initialize Display
  Serial.println(F("Init display..."));
  if (!display.begin()) {
    Serial.println(F("Display fail"));
    while (1) delay(10);
  }
  display.showInitScreen();
  Serial.println(F("Display OK"));
  
  // Initialize Buttons
  Serial.println(F("Init buttons..."));
  buttons.begin();
  Serial.println(F("Buttons OK"));
  
  // Initialize NFC Reader
  Serial.println(F(""));
  if (!nfcReader.begin()) {
    display.clear();
    display.tft.setTextSize(2);
    display.tft.setTextColor(ST77XX_RED);
    display.tft.println(F("NFC"));
    display.tft.println(F("ERROR"));
    display.tft.setTextSize(1);
    display.tft.println("");
    display.tft.setTextColor(ST77XX_YELLOW);
    display.tft.println(F("Check:"));
    display.tft.println(F("Wiring"));
    display.tft.println(F("Power"));
    display.tft.println(F("SPI mode"));
    
    Serial.println(F("FATAL: NFC init failed"));
    Serial.println(F(""));
    Serial.println(F("Troubleshooting:"));
    Serial.println(F("1. Check wiring (SS=D10)"));
    Serial.println(F("2. Check power (3.3V or 5V)"));
    Serial.println(F("3. PN532 in SPI mode?"));
    Serial.println(F("4. Try I2C mode instead"));
    while (1) delay(10);
  }
  Serial.println(F(""));
  
  // Initialize Storage
  Serial.println(F("Init storage..."));
  storage.begin();
  Serial.println(F("Storage OK"));
  
  // Show ready message
  delay(500);
  display.showMessage("Ready!", ST77XX_GREEN);
  delay(1000);
  
  // Initialize Menu System
  menu.begin();
  
  Serial.println(F("==================="));
  Serial.println(F("System ready"));
  Serial.println(F("==================="));
}

// ========== MAIN LOOP ==========
void loop() {
  // Update menu system (handles buttons, NFC, display)
  menu.update();
  
  // Small delay to prevent excessive polling
  delay(50);
}
