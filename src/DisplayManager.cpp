#include "DisplayManager.h"

DisplayManager::DisplayManager() : tft(TFT_CS, TFT_DC, TFT_RST) {
  // tft is now public member
}

bool DisplayManager::begin() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); // Landscape mode
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(TEXT_SIZE);
  tft.setTextWrap(true);
  return true;
}

void DisplayManager::clear() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(0, 0);
}

void DisplayManager::showInitScreen() {
  clear();
  tft.setTextColor(ST77XX_CYAN);
  tft.println(F("NFC System"));
  tft.println("");
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(F("Init..."));
}

void DisplayManager::showMainMenu(int selectedIndex) {
  clear();
  setHeader("MENU", ST77XX_CYAN);
  
  const char* menuItems[] = {
    "1.Read Card",
    "2.Write Card",
    "3.View Stored",
    "4.Clear Data",
    "5.Settings"
  };
  
  for (int i = 0; i < MENU_ITEMS; i++) {
    if (i == selectedIndex) {
      tft.setTextColor(ST77XX_BLACK, ST77XX_WHITE);
    } else {
      tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);
    }
    tft.println(menuItems[i]);
  }
}

void DisplayManager::showReadCardScreen() {
  clear();
  setHeader("READ", ST77XX_GREEN);
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(F("Place card..."));
  tft.println("");
  tft.setTextColor(ST77XX_WHITE);
  tft.println(F("BACK=Return"));
}

void DisplayManager::showCardDetected(const CardData& card) {
  clear();
  tft.setTextColor(ST77XX_GREEN);
  tft.println(F("Card OK!"));
  tft.println("");
  
  tft.setTextColor(ST77XX_WHITE);
  printUID(card);
  tft.println("");
  
  tft.setTextColor(ST77XX_CYAN);
  if (card.uidLength == 4) {
    tft.println(F("Mifare"));
  } else {
    tft.println(F("NTAG"));
  }
  
  tft.println("");
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(F("SEL=Save"));
}

void DisplayManager::showWriteCardScreen() {
  clear();
  setHeader("WRITE", ST77XX_MAGENTA);
  
  tft.setTextColor(ST77XX_YELLOW);
  tft.println(F("Coming soon"));
  tft.println("");
  tft.setTextColor(ST77XX_WHITE);
  tft.println(F("BACK=Return"));
}

void DisplayManager::showStoredCardsScreen(int count) {
  clear();
  setHeader("STORED", ST77XX_CYAN);
  
  if (count == 0) {
    tft.setTextColor(ST77XX_YELLOW);
    tft.println(F("No cards"));
  } else {
    tft.setTextColor(ST77XX_WHITE);
    tft.print(F("Total: "));
    tft.println(count);
  }
  
  tft.println("");
  tft.setTextColor(ST77XX_WHITE);
  tft.println(F("BACK=Return"));
}

void DisplayManager::showClearEEPROMScreen() {
  clear();
  tft.setTextColor(ST77XX_RED);
  tft.println(F("Clear All?"));
  tft.println("");
  tft.println(F("DELETE ALL"));
  tft.println(F("CARDS!"));
  tft.println("");
  tft.setTextColor(ST77XX_WHITE);
  tft.println(F("SEL=Confirm"));
  tft.println(F("BACK=Cancel"));
}

void DisplayManager::showSettingsScreen(int storedCards, int maxCards) {
  clear();
  setHeader("INFO", ST77XX_CYAN);
  
  tft.setTextColor(ST77XX_WHITE);
  tft.print(F("Cards: "));
  tft.println(storedCards);
  
  tft.print(F("Free: "));
  tft.println(maxCards - storedCards);
  
  tft.println("");
  
#ifdef USE_SPI
  tft.println(F("Mode: SPI"));
#else
  tft.println(F("Mode: I2C"));
#endif
}

void DisplayManager::showMessage(const char* message, uint16_t color) {
  clear();
  tft.setTextColor(color);
  tft.println(message);
}

void DisplayManager::showCardSaved() {
  clear();
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.println(F("Saved!"));
  tft.setTextSize(TEXT_SIZE);
}

void DisplayManager::showEEPROMCleared() {
  clear();
  tft.setTextColor(ST77XX_GREEN);
  tft.setTextSize(2);
  tft.println(F("Cleared"));
  tft.setTextSize(TEXT_SIZE);
}

void DisplayManager::printUID(const CardData& card) {
  for (uint8_t i = 0; i < card.uidLength; i++) {
    if (card.uid[i] < 0x10) tft.print("0");
    tft.print(card.uid[i], HEX);
    if (i < card.uidLength - 1) tft.print(":");
  }
}

void DisplayManager::setHeader(const char* title, uint16_t color) {
  tft.setTextColor(color);
  tft.print("=== ");
  tft.print(title);
  tft.println(" ===");
  tft.println("");
}
