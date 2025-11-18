#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H

#include <Arduino.h>
#include "DisplayManager.h"
#include "ButtonHandler.h"
#include "NFCReader.h"
#include "StorageManager.h"
#include "CardData.h"

enum MenuState {
  MENU_MAIN,
  MENU_READ_CARD,
  MENU_WRITE_CARD,
  MENU_VIEW_STORED,
  MENU_CLEAR_EEPROM,
  MENU_SETTINGS
};

class MenuSystem {
public:
  MenuSystem(DisplayManager& display, ButtonHandler& buttons, 
             NFCReader& nfc, StorageManager& storage);
  
  void begin();
  void update();
  MenuState getCurrentState();
  
private:
  DisplayManager& display;
  ButtonHandler& buttons;
  NFCReader& nfc;
  StorageManager& storage;
  
  MenuState currentState;
  int menuIndex;
  bool needsRedraw;
  CardData lastScannedCard;
  
  void handleMainMenu(Button button);
  void handleReadCard(Button button);
  void handleWriteCard(Button button);
  void handleViewStored(Button button);
  void handleClearEEPROM(Button button);
  void handleSettings(Button button);
  
  void changeState(MenuState newState);
  void redrawCurrentMenu();
};

#endif // MENU_SYSTEM_H
