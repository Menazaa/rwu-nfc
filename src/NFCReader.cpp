#include "NFCReader.h"
#include <Wire.h>

NFCReader::NFCReader() 
#ifdef USE_I2C
  : nfc_i2c(PN532_IRQ, PN532_RESET)
#else
  : nfc_spi(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS)
#endif
{
#ifdef USE_I2C
  nfc = &nfc_i2c;
#else
  nfc = &nfc_spi;
#endif
}

bool NFCReader::begin() {
  Serial.println(F("Init PN532..."));
  
#ifdef USE_I2C
  Serial.println(F("Mode: I2C"));
  Serial.println(F("SDA=A4, SCL=A5"));
  Wire.begin();
  delay(100);
#else
  Serial.println(F("Mode: SPI"));
  Serial.println(F("SS=D10, SCK=D13"));
#endif
  
  nfc->begin();
  delay(500); // Give PN532 time to wake up - CRITICAL!
  
  // Try to get firmware version with retries
  uint32_t versiondata = 0;
  for (int i = 0; i < 5; i++) {
    Serial.print(F("Attempt "));
    Serial.print(i + 1);
    Serial.print(F("..."));
    
    versiondata = nfc->getFirmwareVersion();
    if (versiondata) {
      Serial.println(F("OK"));
      break;
    }
    Serial.println(F("fail"));
    delay(300);
  }
  
  if (!versiondata) {
    Serial.println(F(""));
    Serial.println(F("*** NO PN532 ***"));
    return false;
  }
  
  Serial.print(F("PN5"));
  Serial.print((versiondata >> 24) & 0xFF, HEX);
  Serial.print(F(" v"));
  Serial.print((versiondata >> 16) & 0xFF);
  Serial.print('.');
  Serial.println((versiondata >> 8) & 0xFF);
  
  nfc->SAMConfig();
  Serial.println(F("PN532 OK"));
  return true;
}

bool NFCReader::readCard(CardData& card) {
  uint8_t success;
  uint8_t uid[7] = {0};
  uint8_t uidLength;
  
  success = nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);
  
  if (success) {
    memcpy(card.uid, uid, 7);
    card.uidLength = uidLength;
    card.timestamp = millis() / 1000;
    card.valid = true;
    readCardData(card);
    return true;
  }
  
  return false;
}

bool NFCReader::readCardData(CardData& card) {
  if (card.uidLength == 4) {
    // Mifare Classic
    if (authenticateMifare(card.uid, card.uidLength, 4)) {
      uint8_t data[16];
      if (readMifareBlock(4, data)) {
        memcpy(card.data, data, 16);
        
        // Try to read additional blocks
        if (authenticateMifare(card.uid, card.uidLength, 5)) {
          if (readMifareBlock(5, data)) {
            memcpy(card.data + 16, data, 16);
          }
        }
        if (authenticateMifare(card.uid, card.uidLength, 6)) {
          if (readMifareBlock(6, data)) {
            memcpy(card.data + 32, data, 16);
          }
        }
        return true;
      }
    }
  } else if (card.uidLength == 7) {
    // NTAG/Ultralight
    uint8_t data[16];
    
    // Read pages 4-7 (first 16 bytes of user data)
    if (readNTAGPage(4, data)) {
      memcpy(card.data, data, 16);
      
      // Read pages 8-11
      if (readNTAGPage(8, data)) {
        memcpy(card.data + 16, data, 16);
      }
      
      // Read pages 12-15
      if (readNTAGPage(12, data)) {
        memcpy(card.data + 32, data, 16);
      }
      
      return true;
    }
  }
  
  return false;
}

bool NFCReader::writeCard(const CardData& card) {
  // TODO: Implement
  return false;
}

bool NFCReader::isCardPresent() {
  uint8_t uid[7];
  uint8_t uidLength;
  return nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 50);
}

bool NFCReader::authenticateMifare(const uint8_t* uid, uint8_t uidLength, uint8_t blockNumber) {
  return nfc->mifareclassic_AuthenticateBlock(const_cast<uint8_t*>(uid), uidLength, blockNumber, 0, defaultKey);
}

bool NFCReader::readMifareBlock(uint8_t blockNumber, uint8_t* data) {
  return nfc->mifareclassic_ReadDataBlock(blockNumber, data);
}

bool NFCReader::writeMifareBlock(uint8_t blockNumber, const uint8_t* data) {
  return nfc->mifareclassic_WriteDataBlock(blockNumber, const_cast<uint8_t*>(data));
}

bool NFCReader::readNTAGPage(uint8_t page, uint8_t* data) {
  return nfc->ntag2xx_ReadPage(page, data);
}

bool NFCReader::writeNTAGPage(uint8_t page, const uint8_t* data) {
  return nfc->ntag2xx_WritePage(page, const_cast<uint8_t*>(data));
}
