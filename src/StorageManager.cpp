#include "StorageManager.h"

StorageManager::StorageManager() : cardCount(0) {
}

bool StorageManager::begin() {
  int addr = EEPROM_START_ADDR + (MAX_STORED_CARDS * CARD_DATA_SIZE);
  EEPROM.get(addr, cardCount);
  
  if (cardCount < 0 || cardCount > MAX_STORED_CARDS) {
    cardCount = 0;
    updateCardCount();
  }
  
  Serial.print(F("Cards: "));
  Serial.println(cardCount);
  return true;
}

bool StorageManager::saveCard(const CardData& card) {
  if (isFull()) {
    return false;
  }
  
  int addr = calculateAddress(cardCount);
  EEPROM.put(addr, card);
  
  cardCount++;
  updateCardCount();
  return true;
}

bool StorageManager::loadCard(int index, CardData& card) {
  if (index < 0 || index >= cardCount) {
    card.valid = false;
    return false;
  }
  
  int addr = calculateAddress(index);
  EEPROM.get(addr, card);
  return card.valid;
}

void StorageManager::clearAll() {
  for (int i = 0; i < MAX_STORED_CARDS * CARD_DATA_SIZE + sizeof(int); i++) {
    EEPROM.write(EEPROM_START_ADDR + i, 0);
  }
  cardCount = 0;
  updateCardCount();
}

int StorageManager::getCardCount() {
  return cardCount;
}

int StorageManager::getFreeSlots() {
  return MAX_STORED_CARDS - cardCount;
}

bool StorageManager::isFull() {
  return cardCount >= MAX_STORED_CARDS;
}

void StorageManager::updateCardCount() {
  int addr = EEPROM_START_ADDR + (MAX_STORED_CARDS * CARD_DATA_SIZE);
  EEPROM.put(addr, cardCount);
}

int StorageManager::calculateAddress(int index) {
  return EEPROM_START_ADDR + (index * CARD_DATA_SIZE);
}
