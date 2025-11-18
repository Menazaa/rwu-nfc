#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <Arduino.h>
#include <EEPROM.h>
#include "Config.h"
#include "CardData.h"

class StorageManager {
public:
  StorageManager();
  
  bool begin();
  bool saveCard(const CardData& card);
  bool loadCard(int index, CardData& card);
  void clearAll();
  int getCardCount();
  int getFreeSlots();
  bool isFull();
  
private:
  int cardCount;
  
  void updateCardCount();
  int calculateAddress(int index);
};

#endif // STORAGE_MANAGER_H
