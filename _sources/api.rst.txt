API Reference
=============

This section provides detailed documentation of the classes, methods, and
configuration options available in the NFC Access Control System.

.. note::
   
   For practical examples of using these APIs, see :doc:`examples` which covers 
   the basic read and write operations (Course Tasks 1 & 2).

Core Classes
------------

AccessControlSystem
^^^^^^^^^^^^^^^^^^^

The main class that manages the access control logic, menu system, and
hardware interface.

**Header**: ``include/AccessControlSystem.h``

**Public Methods**:

.. cpp:class:: AccessControlSystem

   .. cpp:function:: AccessControlSystem()

      Constructor. Initializes the access control system with default settings.

   .. cpp:function:: void begin()

      Initializes all hardware components (LCD, NFC reader, buttons, relay).
      Must be called in ``setup()`` before using the system.

   .. cpp:function:: void update()

      Main update loop. Should be called repeatedly in ``loop()``.
      Handles state machine updates, button input, and card reading.

   .. cpp:function:: bool isCardAuthorized(uint8_t* uid, uint8_t uidLength)

      Checks if a card UID is in the authorized list.
      
      :param uid: Pointer to UID bytes array
      :param uidLength: Length of UID (4 or 7 bytes)
      :return: ``true`` if authorized, ``false`` otherwise

   .. cpp:function:: bool registerCard(uint8_t* uid, uint8_t uidLength)

      Adds a card to the authorized list in EEPROM.
      
      :param uid: Pointer to UID bytes array
      :param uidLength: Length of UID (4 or 7 bytes)
      :return: ``true`` if successful, ``false`` if card exists or storage full

   .. cpp:function:: bool deleteCard(uint8_t* uid, uint8_t uidLength)

      Removes a card from the authorized list.
      
      :param uid: Pointer to UID bytes array
      :param uidLength: Length of UID (4 or 7 bytes)
      :return: ``true`` if successful, ``false`` if card not found

   .. cpp:function:: int getStoredCardCount()

      Returns the number of cards currently stored.
      
      :return: Number of authorized cards (0-40)

   .. cpp:function:: void grantAccess()

      Activates the relay for the configured duration.
      Called when an authorized card is detected.

   .. cpp:function:: void denyAccess()

      Displays access denied message.
      Called when an unauthorized card is detected.

NFCReader
^^^^^^^^^

Wrapper class for the PN532 NFC module, providing simplified card reading
and cloning functionality.

**Header**: ``include/NFCReader.h``

**Public Methods**:

.. cpp:class:: NFCReader

   .. cpp:function:: NFCReader(NFCCommMode commMode = NFCCommMode::SPI, NFCReadMode readMode = NFCReadMode::IRQ)

      Constructor with communication and reading mode selection.
      
      :param commMode: Communication mode (I2C or SPI)
      :param readMode: Reading mode (POLLING or IRQ)

   .. cpp:function:: bool begin()

      Initializes the PN532 module.
      
      :return: ``true`` if initialization successful, ``false`` otherwise

   .. cpp:function:: NFCCardInfo readCard()

      Reads a card and retrieves its complete information.
      Automatically uses cloned UID if present, otherwise physical UID.
      
      :return: ``NFCCardInfo`` structure containing all card data

   .. cpp:function:: bool isCardPresent()

      Checks if a card is currently in the reader field.
      
      :return: ``true`` if card detected

   .. cpp:function:: void resetCardState()

      Resets the card detection state to allow re-reading the same card.
      Call this after processing a card to enable detection again.

   .. cpp:function:: NFCWriteResult writeString(const String& text, uint8_t startAddress = 0, bool verify = true)

      Writes a string to the card, auto-detecting card type.
      
      :param text: String to write
      :param startAddress: Starting address (block or page depending on card type)
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

   .. cpp:function:: NFCWriteResult writeData(const uint8_t* data, uint8_t dataLength, uint8_t startAddress = 0, bool verify = true)

      Writes binary data to the card, auto-detecting card type.
      
      :param data: Pointer to data buffer
      :param dataLength: Number of bytes to write
      :param startAddress: Starting address
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

   .. cpp:function:: NFCWriteResult writeNTAG(uint8_t page, const uint8_t* data, uint8_t dataLength, bool verify = true)

      Writes data to an NTAG or Ultralight card (page-based).
      
      :param page: Page number (use 4 or higher for user data)
      :param data: Pointer to data buffer (4 bytes per page)
      :param dataLength: Number of bytes to write
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

   .. cpp:function:: NFCWriteResult writeNTAGString(uint8_t startPage, const String& text, bool verify = true)

      Writes a string across multiple NTAG/Ultralight pages.
      
      :param startPage: Starting page number
      :param text: String to write
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

   .. cpp:function:: NFCWriteResult writeMifareClassic(uint8_t block, const uint8_t* data, uint8_t dataLength, const uint8_t* key = DEFAULT_KEY, bool useKeyB = false, bool verify = true)

      Writes data to a Mifare Classic card (block-based, requires authentication).
      
      :param block: Block number (use 4 or higher for user data)
      :param data: Pointer to data buffer (16 bytes per block)
      :param dataLength: Number of bytes to write
      :param key: Authentication key (6 bytes, default FF FF FF FF FF FF)
      :param useKeyB: Use Key B instead of Key A
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

   .. cpp:function:: NFCWriteResult writeMifareClassicString(uint8_t startBlock, const String& text, const uint8_t* key = DEFAULT_KEY, bool useKeyB = false, bool verify = true)

      Writes a string across multiple Mifare Classic blocks.
      
      :param startBlock: Starting block number
      :param text: String to write
      :param key: Authentication key
      :param useKeyB: Use Key B instead of Key A
      :param verify: Whether to verify the write operation
      :return: ``NFCWriteResult`` structure with success status

Enumerations
------------

NFCCommMode
^^^^^^^^^^^

PN532 communication modes.

.. code-block:: cpp

   enum class NFCCommMode {
       I2C,  // I2C communication (simpler wiring, slower)
       SPI   // SPI communication (more wires, faster - recommended)
   };

NFCReadMode
^^^^^^^^^^^

Card reading modes.

.. code-block:: cpp

   enum class NFCReadMode {
       POLLING,  // Continuous polling (~100ms interval)
       IRQ       // Interrupt-driven (faster, recommended)
   };

NFCCardType
^^^^^^^^^^^

Supported NFC card types.

.. code-block:: cpp

   enum class NFCCardType {
       UNKNOWN,             // Unknown or unsupported card type
       MIFARE_CLASSIC_1K,   // Mifare Classic 1K (4-byte UID, 1KB)
       MIFARE_CLASSIC_4K,   // Mifare Classic 4K (4-byte UID, 4KB)
       MIFARE_ULTRALIGHT,   // Mifare Ultralight (7-byte UID, 64 bytes)
       NTAG                 // NTAG213/215/216 (7-byte UID, varies)
   };

Data Structures
---------------

NFCCardInfo
^^^^^^^^^^^

Complete information about a detected NFC card.

.. code-block:: cpp

   struct NFCCardInfo {
       bool detected;              // True if card was detected
       uint8_t uid[7];            // Physical UID from manufacturer block
       uint8_t uidLength;         // UID length (4 or 7 bytes)
       NFCCardType cardType;      // Type of card detected
       uint32_t cardID;           // Numeric ID for 4-byte UIDs
       
       // Card cloning features (advanced)
       bool hasClonedUID;         // True if card has cloned UID in custom sector
       uint8_t clonedUID[7];      // Cloned UID data
       uint8_t clonedUIDLength;   // Length of cloned UID
       
       // Helper methods
       const uint8_t* getEffectiveUID() const;      // Returns cloned or physical UID
       uint8_t getEffectiveUIDLength() const;       // Returns effective UID length
   };

**Usage Example:**

.. code-block:: cpp

   NFCCardInfo card = nfcReader.readCard();
   
   if (card.detected) {
       // Use effective UID (cloned if available, otherwise physical)
       const uint8_t* uid = card.getEffectiveUID();
       uint8_t len = card.getEffectiveUIDLength();
       
       // Or access specific UIDs
       if (card.hasClonedUID) {
           // This card has been cloned
           Serial.print("Cloned UID: ");
           for (int i = 0; i < card.clonedUIDLength; i++) {
               Serial.print(card.clonedUID[i], HEX);
           }
       }
   }

NFCWriteResult
^^^^^^^^^^^^^^

Result of a write operation to an NFC card.

.. code-block:: cpp

   struct NFCWriteResult {
       bool success;         // True if write was successful
       bool verified;        // True if write was verified
       String errorMessage;  // Description of error (if success is false)
   };

**Usage Example:**

.. code-block:: cpp

   NFCWriteResult result = nfcReader.writeString("Hello NFC!", 4, true);
   
   if (result.success) {
       Serial.println("Write successful!");
       if (result.verified) {
           Serial.println("Data verified!");
       }
   } else {
       Serial.print("Write failed: ");
       Serial.println(result.errorMessage);
   }

SystemState
^^^^^^^^^^^

States of the access control system state machine.

.. code-block:: cpp

   enum class SystemState {
       IDLE,           // Ready to scan card
       ACCESS_GRANTED, // Card authorized, relay activated
       ACCESS_DENIED,  // Card not authorized
       MENU,           // In menu system
       REGISTERING,    // Registering new card
       DELETING,       // Deleting card
       LISTING_CARDS,  // Displaying stored cards
       CLONING_SOURCE, // Waiting for source card to clone
       CLONING_TARGET  // Waiting for target card to write
   };

State Machine Diagram
^^^^^^^^^^^^^^^^^^^^^

.. graphviz::
   :caption: System State Machine

   digraph state_machine {
       rankdir=LR;
       node [shape=box, style=rounded];
       
       IDLE [fillcolor=lightgreen, style=filled];
       MENU [fillcolor=lightblue, style=filled];
       ACCESS_GRANTED [fillcolor=green, style=filled, fontcolor=white];
       ACCESS_DENIED [fillcolor=red, style=filled, fontcolor=white];
       REGISTERING [fillcolor=yellow, style=filled];
       DELETING [fillcolor=orange, style=filled];
       LISTING_CARDS [fillcolor=cyan, style=filled];
       CLONING_SOURCE [fillcolor=violet, style=filled];
       CLONING_TARGET [fillcolor=violet, style=filled];
       
       IDLE -> ACCESS_GRANTED [label="Card\nAuthorized"];
       IDLE -> ACCESS_DENIED [label="Card\nUnauthorized"];
       IDLE -> MENU [label="SELECT\nButton"];
       
       ACCESS_GRANTED -> IDLE [label="Timeout\n(3s)"];
       ACCESS_DENIED -> IDLE [label="Timeout\n(2s)"];
       
       MENU -> REGISTERING [label="Select\nRegister"];
       MENU -> DELETING [label="Select\nDelete"];
       MENU -> LISTING_CARDS [label="Select\nList"];
       MENU -> CLONING_SOURCE [label="Select\nClone"];
       MENU -> IDLE [label="Exit Menu"];
       
       REGISTERING -> MENU [label="Card\nRegistered"];
       DELETING -> MENU [label="Card\nDeleted"];
       LISTING_CARDS -> MENU [label="BACK\nButton"];
       
       CLONING_SOURCE -> CLONING_TARGET [label="Source\nScanned"];
       CLONING_TARGET -> MENU [label="Clone\nComplete"];
       
       REGISTERING -> MENU [label="BACK"];
       DELETING -> MENU [label="BACK"];
       CLONING_SOURCE -> MENU [label="BACK"];
       CLONING_TARGET -> MENU [label="BACK"];
   }

MenuItem
^^^^^^^^

Main menu options available in the system.

.. code-block:: cpp

   enum class MenuItem {
       REGISTER_CARD,  // Add new authorized card
       DELETE_CARD,    // Remove authorized card
       LIST_CARDS,     // Display all authorized cards
       CLONE_CARD,     // Clone a card UID to another card
       SYSTEM_INFO,    // Show system information
       EXIT_MENU       // Return to IDLE mode
   };

Configuration Constants
-----------------------

Hardware Pin Definitions
^^^^^^^^^^^^^^^^^^^^^^^^

Located in ``include/Config.h``:

.. code-block:: cpp

   // PN532 NFC Module pins (SPI mode)
   #define PN532_SCK  (13)
   #define PN532_MISO (12)
   #define PN532_MOSI (11)
   #define PN532_SS   (10)
   #define PN532_IRQ  (2)
   #define PN532_RST  (3)

   // LCD Display pins (4-bit parallel)
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

System Parameters
^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   // Maximum number of authorized cards
   #define MAX_CARDS 40

   // Relay activation duration (milliseconds)
   #define ACCESS_GRANT_DURATION 3000

   // Button debounce delay (milliseconds)
   #define DEBOUNCE_DELAY 50

   // LCD dimensions
   #define LCD_COLS 16
   #define LCD_ROWS 2

   // UID length constants
   #define UID_LENGTH_4BYTE 4
   #define UID_LENGTH_7BYTE 7
   #define MAX_UID_LENGTH 7

EEPROM Memory Map
^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   // EEPROM addresses
   #define EEPROM_MAGIC_ADDR    0      // Magic number (2 bytes)
   #define EEPROM_COUNT_ADDR    2      // Card count (1 byte)
   #define EEPROM_CARDS_ADDR    3      // Start of card data
   
   // Card entry structure (8 bytes per card)
   // Byte 0: UID length (4 or 7)
   // Bytes 1-7: UID data (padded with 0xFF)

Custom Sector Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

For card cloning functionality:

.. code-block:: cpp

   // Custom sector for cloned UID storage
   #define CUSTOM_SECTOR 1
   #define CUSTOM_BLOCK_UID 4      // Block for UID storage
   #define CUSTOM_BLOCK_META 5     // Block for metadata
   #define CUSTOM_BLOCK_SPARE 6    // Spare block

   // Default Mifare Classic keys
   uint8_t defaultKeyA[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
   uint8_t defaultKeyB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

Example Usage
-------------

Basic Setup
^^^^^^^^^^^

.. code-block:: cpp

   #include <AccessControlSystem.h>

   AccessControlSystem accessControl;

   void setup() {
       Serial.begin(115200);
       accessControl.begin();
   }

   void loop() {
       accessControl.update();
   }

Custom Card Handling
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   #include <NFCReader.h>

   NFCReader reader(NFCCommMode::SPI, NFCReadMode::IRQ);
   uint8_t uid[7];
   uint8_t uidLength;

   void setup() {
       Serial.begin(115200);
       if (!reader.begin()) {
           Serial.println("NFC reader initialization failed!");
           while(1);
       }
   }

   void loop() {
       if (reader.readCard(uid, &uidLength)) {
           Serial.print("Card UID: ");
           for (uint8_t i = 0; i < uidLength; i++) {
               Serial.print(uid[i], HEX);
               Serial.print(" ");
           }
           Serial.println();
       }
       delay(100);
   }

Manual Card Registration
^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   void registerNewCard() {
       uint8_t uid[7];
       uint8_t uidLength;
       
       Serial.println("Present card to register...");
       
       if (reader.readCard(uid, &uidLength)) {
           if (accessControl.registerCard(uid, uidLength)) {
               Serial.println("Card registered successfully!");
           } else {
               Serial.println("Failed to register card");
           }
       }
   }

Return Values
-------------

Most methods return boolean values:

* ``true``: Operation successful
* ``false``: Operation failed or card not found

Check serial output for detailed error messages during development.

Memory Considerations
---------------------

EEPROM Usage
^^^^^^^^^^^^

* Magic number: 2 bytes
* Card count: 1 byte
* Card data: 8 bytes per card × 40 cards = 320 bytes
* **Total**: 323 bytes of 1024 bytes available

RAM Usage
^^^^^^^^^

Approximate RAM usage:

* LiquidCrystal library: ~100 bytes
* Adafruit_PN532 library: ~200 bytes
* System variables: ~150 bytes
* Stack and buffers: ~200 bytes
* **Total**: ~650 bytes of 2048 bytes available

Thread Safety
-------------

The system is single-threaded and designed for Arduino's cooperative
multitasking model. All operations are non-blocking when using IRQ mode.

Debugging
---------

Enable serial debugging in ``Config.h``:

.. code-block:: cpp

   #define DEBUG_MODE 1

This provides verbose logging of all operations.
