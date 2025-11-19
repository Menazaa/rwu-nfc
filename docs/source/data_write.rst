.. _nfc-writing-report:

=================================================
Report on Data Writing Operations
=================================================

Introduction
------------
Data writing in the NFC Access Control System is primarily focused on **Card Cloning**. Unlike simple NDEF writing, this system performs raw block writing to specific memory sectors. It writes a "Cloned UID" and metadata into **Sector 1** of a standard Mifare Classic card, allowing it to impersonate another card.

Hardware Used
-------------
* **PN532 NFC Module**: Acts as the writer. It requires a stable 5V power supply capable of delivering up to 200-300mA during active operations.
* **Target Card**: Must be a **Mifare Classic 1K or 4K** card. The card must be writable (not locked) and use default keys for Sector 1.

Software Used
-------------
* **Class**: ``NFCReader`` handles the low-level write commands.
* **Key Methods**:

  * ``cloneCard()``: High-level workflow for reading a source and writing to a target.
  * ``writeClonedUID()``: Writes the specific UID data to Block 4.
  * ``mifareclassic_WriteDataBlock()``: The underlying API call to commit 16 bytes to a block.

Diagrams of Connections
-----------------------
The writing process involves a strict sequence of authentication and verification to ensure data integrity.



Code Snippets and API Implementation
------------------------------------
The writing process involves preparing a 16-byte buffer with specific "Magic Bytes" to mark the card as part of the system.

**Writing Logic (NFCReader.cpp)**:

.. code-block:: cpp

    // From src/NFCReader.cpp
    bool NFCReader::writeClonedUID(const uint8_t* sourceUID, uint8_t sourceUIDLength) {
      // 1. Prepare Data Block (Block 4)
      uint8_t blockData[16] = {0};
      blockData[0] = CARD_MAGIC_BYTE1; // 0xAC
      blockData[1] = CARD_MAGIC_BYTE2; // 0xDB
      blockData[2] = sourceUIDLength;  // Length
      memcpy(&blockData[3], sourceUID, sourceUIDLength); // The UID to clone
      
      // 2. Authenticate
      if (!authenticateMifareBlock(CUSTOM_BLOCK_UID, DEFAULT_KEY, ...)) {
        return false; 
      }
      
      // 3. Perform Write
      if (!_nfc->mifareclassic_WriteDataBlock(CUSTOM_BLOCK_UID, blockData)) {
        return false;
      }
      
      // 4. Verify (Read back)
      // ... (verification logic) ...
      return true;
    }

**Generic Write Example (examples/write_example.cpp)**:

The system also supports generic writing for testing:

.. code-block:: cpp

    // Writing text to a block
    String text = "Mifare data test";
    result = nfcReader.writeMifareClassicString(4, text);

Results
-------
Upon a successful write operation:

1. **Data Persistence**: The Source UID is permanently stored in Block 4 of the Target card (until overwritten).
2. **System Recognition**: When this card is subsequently scanned by the system, the ``readCard()`` function will detect the Magic Bytes written during this process and identify the card by its new "Cloned" UID rather than its physical one.
3. **User Feedback**: The LCD displays "Clone Successful".

Limitations
-----------
* **Card Compatibility**: Writing cloned UIDs is **strictly limited** to Mifare Classic cards. Ultralight and NTAG cards cannot be used as targets for cloning because they lack the sector structure required.
* **Write Cycles**: EEPROM sectors on cards have a life expectancy of ~100,000 cycles.
* **Sector Security**: The target card **must** use default transport keys (``0xFF...``) for Sector 1. If the card has been secured with custom keys, the authentication step will fail, and writing will be impossible.
* **Block 0**: This system does *not* write to Block 0 (Manufacturer Block). It uses a custom sector approach to avoid the need for expensive "Magic" cards.

Conclusion
----------
The writing implementation provides a powerful "soft cloning" capability. By writing to standard data sectors rather than requiring special hardware to modify manufacturer blocks, the system offers a cost-effective and accessible way to duplicate access credentials for authorized use cases.
