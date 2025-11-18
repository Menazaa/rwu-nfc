#ifndef NFC_READER_H
#define NFC_READER_H

#include <Arduino.h>
#include <Adafruit_PN532.h>
#include "Config.h"
#include "CardData.h"

class NFCReader {
public:
  NFCReader();
  
  bool begin();
  bool readCard(CardData& card);
  bool writeCard(const CardData& card);
  bool isCardPresent();
  
  // Mifare Classic specific
  bool authenticateMifare(const uint8_t* uid, uint8_t uidLength, uint8_t blockNumber);
  bool readMifareBlock(uint8_t blockNumber, uint8_t* data);
  bool writeMifareBlock(uint8_t blockNumber, const uint8_t* data);
  
  // NTAG/Ultralight specific
  bool readNTAGPage(uint8_t page, uint8_t* data);
  bool writeNTAGPage(uint8_t page, const uint8_t* data);
  
private:
#ifdef USE_I2C
  Adafruit_PN532 nfc_i2c;
#else
  Adafruit_PN532 nfc_spi;
#endif
  
  Adafruit_PN532* nfc; // Pointer to active instance
  uint8_t defaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  
  bool readCardData(CardData& card);
};

#endif // NFC_READER_H
