#ifndef CARD_DATA_H
#define CARD_DATA_H

#include <Arduino.h>

// Structure to hold NFC card information
struct CardData {
  uint8_t uid[7];           // Card UID (up to 7 bytes)
  uint8_t uidLength;        // Length of UID (4 or 7 bytes)
  uint8_t data[48];         // Card data (first block/page)
  uint32_t timestamp;       // When card was read (seconds since boot)
  bool valid;               // Is this card data valid?
  
  CardData() : uidLength(0), timestamp(0), valid(false) {
    memset(uid, 0, sizeof(uid));
    memset(data, 0, sizeof(data));
  }
  
  void clear() {
    memset(uid, 0, sizeof(uid));
    memset(data, 0, sizeof(data));
    uidLength = 0;
    timestamp = 0;
    valid = false;
  }
};

#endif // CARD_DATA_H
