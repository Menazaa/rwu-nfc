#include "MenuSystem.h"

MenuSystem::MenuSystem(DisplayManager& disp, ButtonHandler& btn, 
                       NFCReader& nfcReader, StorageManager& stor)
  : display(disp), buttons(btn), nfc(nfcReader), storage(stor),
    currentState(MENU_MAIN), menuIndex(0), needsRedraw(true) {
}

void MenuSystem::begin() {
  redrawCurrentMenu();
}

void MenuSystem::update() {
  // Check for button press
  Button button = buttons.getButtonPress();
  
  // Handle based on current state
  switch (currentState) {
    case MENU_MAIN:
      handleMainMenu(button);
      break;
    case MENU_READ_CARD:
      handleReadCard(button);
      break;
    case MENU_WRITE_CARD:
      handleWriteCard(button);
      break;
    case MENU_VIEW_STORED:
      handleViewStored(button);
      break;
    case MENU_CLEAR_EEPROM:
      handleClearEEPROM(button);
      break;
    case MENU_SETTINGS:
      handleSettings(button);
      break;
  }
  
  // Redraw if needed
  if (needsRedraw) {
    redrawCurrentMenu();
    needsRedraw = false;
  }
}

MenuState MenuSystem::getCurrentState() {
  return currentState;
}

void MenuSystem::handleMainMenu(Button button) {
  if (button == BUTTON_UP) {
    menuIndex--;
    if (menuIndex < 0) menuIndex = MENU_ITEMS - 1;
    needsRedraw = true;
  }
  else if (button == BUTTON_DOWN) {
    menuIndex++;
    if (menuIndex >= MENU_ITEMS) menuIndex = 0;
    needsRedraw = true;
  }
  else if (button == BUTTON_SELECT) {
    switch (menuIndex) {
      case 0: changeState(MENU_READ_CARD); break;
      case 1: changeState(MENU_WRITE_CARD); break;
      case 2: changeState(MENU_VIEW_STORED); break;
      case 3: changeState(MENU_CLEAR_EEPROM); break;
      case 4: changeState(MENU_SETTINGS); break;
    }
  }
}

void MenuSystem::handleReadCard(Button button) {
  // Check for card
  if (nfc.readCard(lastScannedCard)) {
    display.showCardDetected(lastScannedCard);
  }
  
  if (button == BUTTON_SELECT && lastScannedCard.valid) {
    // Save card
    if (storage.saveCard(lastScannedCard)) {
      display.showCardSaved();
      delay(1500);
      lastScannedCard.clear();
      changeState(MENU_MAIN);
    }
  }
  else if (button == BUTTON_BACK) {
    lastScannedCard.clear();
    changeState(MENU_MAIN);
  }
}

void MenuSystem::handleWriteCard(Button button) {
  if (button == BUTTON_BACK) {
    changeState(MENU_MAIN);
  }
}

void MenuSystem::handleViewStored(Button button) {
  if (button == BUTTON_BACK) {
    changeState(MENU_MAIN);
  }
  // TODO: Add navigation through stored cards
}

void MenuSystem::handleClearEEPROM(Button button) {
  if (button == BUTTON_SELECT) {
    storage.clearAll();
    display.showEEPROMCleared();
    delay(1500);
    changeState(MENU_MAIN);
  }
  else if (button == BUTTON_BACK) {
    changeState(MENU_MAIN);
  }
}

void MenuSystem::handleSettings(Button button) {
  if (button == BUTTON_BACK) {
    changeState(MENU_MAIN);
  }
}

void MenuSystem::changeState(MenuState newState) {
  currentState = newState;
  needsRedraw = true;
}

void MenuSystem::redrawCurrentMenu() {
  switch (currentState) {
    case MENU_MAIN:
      display.showMainMenu(menuIndex);
      break;
    case MENU_READ_CARD:
      display.showReadCardScreen();
      break;
    case MENU_WRITE_CARD:
      display.showWriteCardScreen();
      break;
    case MENU_VIEW_STORED:
      display.showStoredCardsScreen(storage.getCardCount());
      break;
    case MENU_CLEAR_EEPROM:
      display.showClearEEPROMScreen();
      break;
    case MENU_SETTINGS:
      display.showSettingsScreen(storage.getCardCount(), MAX_STORED_CARDS);
      break;
  }
}
