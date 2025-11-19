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

**Pin Connections (SPI Mode)**:

* **SCK**: Arduino D13 -> PN532 SCK
* **MISO**: Arduino D12 -> PN532 MISO
* **MOSI**: Arduino D11 -> PN532 MOSI
* **SS**: Arduino D10 -> PN532 SS
* **IRQ**: Arduino D2 -> PN532 IRQ (Critical for non-blocking detection)

Software Used
-------------
* **Library**: ``Adafruit_PN532`` for low-level communication.
* **Class**: ``NFCReader`` (defined in ``include/NFCReader.h``) encapsulates the reading logic.

**Key Methods**:

* ``readCard()``: The main orchestrator for detecting and reading data.
* ``readPhysicalUID()``: Reads the read-only Block 0.
* ``readCustomSector()``: Authenticates and reads Sector 1 to check for cloned credentials.

Diagrams of Connections
-----------------------
The system uses an interrupt-driven approach where the PN532 signals the Arduino when a card enters the RF field.

[Image of NFC reader SPI connection diagram with Arduino]

Code Snippets and API Implementation
------------------------------------
The system implements an intelligent ``readCard`` method that automatically prioritizes cloned data.

**Reading Strategy (NFCReader.cpp)**:

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

**Custom Sector Reading (NFCReader.cpp)**:

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
