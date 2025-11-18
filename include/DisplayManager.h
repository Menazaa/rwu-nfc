#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "Config.h"
#include "CardData.h"

class DisplayManager {
public:
  Adafruit_ST7735 tft;
  
  DisplayManager();
  
  bool begin();
  void clear();
  void showInitScreen();
  void showMainMenu(int selectedIndex);
  void showReadCardScreen();
  void showCardDetected(const CardData& card);
  void showWriteCardScreen();
  void showStoredCardsScreen(int count);
  void showClearEEPROMScreen();
  void showSettingsScreen(int storedCards, int maxCards);
  void showMessage(const char* message, uint16_t color);
  void showCardSaved();
  void showEEPROMCleared();
  
private:
  void printUID(const CardData& card);
  void setHeader(const char* title, uint16_t color);
};

#endif // DISPLAY_MANAGER_H
