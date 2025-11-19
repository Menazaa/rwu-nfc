.. _nfc-reading-report:

=================================================
Report on Data Reading Operations
=================================================

Introduction
------------
Data reading is the fundamental operation of the NFC Access Control System. It serves two primary purposes: **Access Verification** (scanning a card to grant entry) and **Source Identification** (scanning a card to be cloned). The system utilizes a sophisticated "Dual UID" logic, prioritizing data read from a custom sector over the card's physical manufacturer ID.

Hardware Used
-------------
The reading operation relies on the following components:

* **PN532 NFC Module**: Configured in SPI mode for high-speed communication.
* **Arduino Nano (ATmega328P)**: Processes the data and handles the interrupt logic.

Pin Configuration
^^^^^^^^^^^^^^^^^

Default pin assignments are defined in ``include/Config.h``. Modify if needed::

    // PN532 NFC Module pins
    #define PN532_SCK  (13)
    #define PN532_MISO (12)
    #define PN532_MOSI (11)
    #define PN532_SS   (10)
    #define PN532_IRQ  (2)
    #define PN532_RST  (3)

    // LCD Display pins
    #define LCD_RS (4)
    #define LCD_EN (5)
    #define LCD_D4 (6)
    #define LCD_D5 (7)
    #define LCD_D6 (8)
    #define LCD_D7 (9)

    // Button pins
    #define BTN_UP     (A0)
    #define BTN_DOWN   (A1)
    #define BTN_SELECT (A2)
    #define BTN_BACK   (A3)

    // Relay pin
    #define RELAY_PIN (A4)

Software Used
-------------
* **Library**: ``Adafruit_PN532`` for low-level communication.
* **Class**: ``NFCReader`` (defined in ``include/NFCReader.h``) encapsulates the reading logic.

Key Methods
^^^^^^^^^^^

* ``readCard()``: The main orchestrator for detecting and reading data.
* ``readPhysicalUID()``: Reads the read-only Block 0.
* ``readCustomSector()``: Authenticates and reads Sector 1 to check for cloned credentials.

Diagrams of Connections
-----------------------
The system uses an interrupt-driven approach where the PN532 signals the Arduino when a card enters the RF field.



Code Snippets and API Implementation
------------------------------------
The system implements an intelligent ``readCard`` method that automatically prioritizes cloned data.

Reading Strategy (NFCReader.cpp)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    // From src/NFCReader.cpp
    NFCCardInfo NFCReader::readCard() {
      // ... (IRQ/Polling logic) ...
      
      // 1. Read Physical UID first
      success = _nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength, 100);
      
      if (success) {
        // ... (Store Physical UID) ...
        
        // 2. Attempt to read Custom Sector (Sector 1)
        if (info.cardType == NFCCardType::MIFARE_CLASSIC_1K) {
           readCustomSector(info); // Updates info.hasClonedUID if found
        }
      }
      return info;
    }

Custom Sector Reading (NFCReader.cpp)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    bool NFCReader::readCustomSector(NFCCardInfo& info) {
      // Authenticate to Block 4 using Default Key A
      if (!authenticateMifareBlock(CUSTOM_BLOCK_UID, DEFAULT_KEY, ...)) {
        return false; 
      }
      // Read data
      if (!_nfc->mifareclassic_ReadDataBlock(CUSTOM_BLOCK_UID, blockData)) {
        return false;
      }
      // Check for Magic Bytes to verify this is a valid clone
      if (blockData[0] == CARD_MAGIC_BYTE1 && blockData[1] == CARD_MAGIC_BYTE2) {
        info.hasClonedUID = true;
        // Extract the "virtual" UID
        memcpy(info.clonedUID, &blockData[3], info.clonedUIDLength);
        return true;
      }
      return false;
    }

Results
-------
When a card is presented:

1. **Detection**: The system detects the card within 100ms using IRQ mode.
2. **Identification**: 
   
   * If the card is a standard tag, the system returns the **Physical UID** (e.g., ``DE AD BE EF``).
   * If the card has been written to by this system previously, it returns the **Cloned UID** (e.g., ``AB 12 CD 34``) stored in Sector 1.

3. **Access**: The retrieved UID is compared against the EEPROM database to grant or deny access.

Limitations
-----------
* **Distance**: The card must be within 4 cm of the reader.
* **Card Types**: The "Dual UID" reading feature (reading cloned data from Sector 1) only works on **Mifare Classic** cards. NTAG/Ultralight cards will only yield their physical UID.
* **Security**: The system reads Sector 1 using default keys (``0xFF...``). If a card uses custom encryption keys for Sector 1, the "Cloned UID" read will fail, and the system will default to the Physical UID.

Conclusion
----------
The reading mechanism is robust and non-blocking, suitable for real-time access control. By implementing a priority read logic (Custom Sector > Manufacturer Block), the system allows inexpensive standard cards to function as "clones" of other credentials without needing special hardware.
