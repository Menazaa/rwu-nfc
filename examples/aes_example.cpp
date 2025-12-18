/**
 * Task 3: Encrypted Access & Cloner Defeat (Button Integration)
 * * LOGIC:
 * 1. Default Mode: "READER" - Checks access.
 * 2. Toggle Mode: Press SELECT (A2) to switch to "CREATOR".
 * 3. Creator Mode: Encrypts & Writes password to Sector 2.
 */

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <AESLib.h> 

// --- Hardware Pins ---
#define PN532_SCK  (13)
#define PN532_MISO (12)
#define PN532_MOSI (11)
#define PN532_SS   (10)

// --- Button Pins (From your Config.h) ---
#define BTN_SELECT A2

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

// --- SECURITY DATA ---
uint8_t systemKey[16] = { 
    0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8,
    0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8 
};
#define SECURE_BLOCK 8
char validPassword[16] = "TopSecretPasswd"; 

// --- Global State ---
bool creationMode = false; // Start in Reader Mode

void printHex(uint8_t *buffer, uint8_t len) {
  for (uint8_t i = 0; i < len; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  Serial.println();
}

void createSecureTag(uint8_t *uid, uint8_t uidLen) {
    Serial.println("\n>> CREATING SECURE TAG...");
    
    uint8_t dataBlock[16];
    memcpy(dataBlock, validPassword, 16);
    aes128_enc_single(systemKey, dataBlock);

    uint8_t keyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
    
    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLen, SECURE_BLOCK, 0, keyA)) {
        if (nfc.mifareclassic_WriteDataBlock(SECURE_BLOCK, dataBlock)) {
            Serial.println("[SUCCESS] Tag is now SECURE.");
        } else {
            Serial.println("[ERROR] Write failed.");
        }
    } else {
        Serial.println("[ERROR] Auth failed (Locked?).");
    }
}

void checkAccess(uint8_t *uid, uint8_t uidLen) {
    Serial.println("\n>> CHECKING ACCESS...");
    
    uint8_t readBuffer[18];
    uint8_t keyA[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

    if (nfc.mifareclassic_AuthenticateBlock(uid, uidLen, SECURE_BLOCK, 0, keyA)) {
        if (nfc.mifareclassic_ReadDataBlock(SECURE_BLOCK, readBuffer)) {
            // Check for Empty (Clone)
            bool isEmpty = true;
            for(int i=0; i<16; i++) {
                if(readBuffer[i] != 0x00 && readBuffer[i] != 0xFF) isEmpty = false;
            }

            if (isEmpty) {
                 Serial.println("[ACCESS DENIED] CLONE DETECTED (Sector 2 Empty).");
                 return;
            }

            aes128_dec_single(systemKey, readBuffer);
            
            if (memcmp(readBuffer, validPassword, 15) == 0) { 
                Serial.println("*** ACCESS GRANTED ***");
            } else {
                Serial.println("[ACCESS DENIED] Password mismatch.");
            }
        }
    } else {
        Serial.println("[ACCESS DENIED] Auth Failed.");
    }
}

void setup() {
  Serial.begin(115200);
  Serial.println("\n--- TASK 3: AES SECURITY SYSTEM ---");
  
  nfc.begin();
  nfc.SAMConfig();
  
  // Initialize Button (Active LOW)
  pinMode(BTN_SELECT, INPUT_PULLUP);
  
  Serial.println("System Ready.");
  Serial.println("Current Mode: [ READER ]");
  Serial.println("Press SELECT button (A2) to toggle modes.");
}

void loop() {
  // --- 1. HANDLE BUTTON PRESS (Toggle Mode) ---
  if (digitalRead(BTN_SELECT) == LOW) {
      delay(50); // Debounce
      if (digitalRead(BTN_SELECT) == LOW) {
          creationMode = !creationMode; // Toggle
          
          Serial.println("\n--------------------------------");
          if(creationMode) Serial.println("MODE SWITCHED TO: [ CREATOR ]");
          else             Serial.println("MODE SWITCHED TO: [ READER ]");
          Serial.println("--------------------------------");
          
          // Wait for button release so we don't toggle constantly
          while(digitalRead(BTN_SELECT) == LOW); 
      }
  }

  // --- 2. NFC LOGIC ---
  // Only check for cards if we aren't busy handling buttons
  uint8_t uid[7];
  uint8_t uidLen;
  
  // Non-blocking check for card presence
  // We set timeout to 100ms so the button remains responsive
  if (nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLen, 100)) {
      
      if (creationMode) {
          createSecureTag(uid, uidLen);
      } else {
          checkAccess(uid, uidLen);
      }
      
      // Small delay to prevent spamming the same card
      delay(1000);
  }
}