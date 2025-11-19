Examples
========

This section provides practical examples for the two main course tasks: reading and writing NFC tags. These examples demonstrate the fundamental NFC operations and serve as the foundation for building more complex applications.

Course Tasks Overview
---------------------

This project was developed as part of an NFC course with the following progression:

1. **Task 1: Tag Reading** - Learn to detect and read NFC card information
2. **Task 2: Tag Writing** - Learn to write data to NFC cards
3. **Extended Project: Access Control System** - Full implementation combining both tasks

Task 1: Reading NFC Tags
-------------------------

The first course task focuses on reading NFC card UIDs and information. This is the foundation of any NFC application.

Learning Objectives
^^^^^^^^^^^^^^^^^^^

* Initialize and configure the PN532 NFC reader
* Detect when a card enters the reader field
* Read card UID (Unique Identifier)
* Identify different card types (Mifare Classic, NTAG, etc.)
* Display and process card information

Example Code: read_example.cpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Location: ``examples/read_example.cpp``

**Complete Source Code**

The full example code is shown below. It demonstrates configurable communication modes (I2C/SPI) and 
reading modes (Polling/IRQ), along with comprehensive card type detection and UID display.

.. literalinclude:: ../../examples/read_example.cpp
   :language: cpp
   :linenos:
   :caption: examples/read_example.cpp - Complete NFC Tag Reading Example

**Key Features**

* Configurable communication mode (I2C or SPI via ``USE_I2C`` define)
* Configurable reading mode (Polling or IRQ via ``USE_IRQ_MODE`` define)
* Comprehensive card type detection (Mifare Classic 1K/4K, NTAG, Ultralight)
* Formatted UID display with byte count
* Auto-detection and card state reset for continuous reading
* Detailed serial output for debugging and learning

Key Concepts
^^^^^^^^^^^^

**NFCCardInfo Structure**

The ``NFCCardInfo`` structure contains all information about a detected card:

.. code-block:: cpp

   struct NFCCardInfo {
     bool detected;              // True if card was found
     uint8_t uid[7];            // Physical UID (manufacturer)
     uint8_t uidLength;         // UID length (4 or 7 bytes)
     NFCCardType cardType;      // Card type identifier
     uint32_t cardID;           // Numeric ID (for 4-byte UIDs)
     
     // Advanced features (card cloning)
     bool hasClonedUID;         // True if card has cloned UID
     uint8_t clonedUID[7];      // Cloned UID data
     uint8_t clonedUIDLength;
   };

**Communication Modes**

* **I2C Mode**: Simpler wiring, shared bus, slower
* **SPI Mode**: More wires, dedicated bus, faster (recommended)

**Reading Modes**

* **POLLING**: Actively checks for cards at intervals (~100ms)
* **IRQ**: Interrupt-based detection, faster and more efficient (recommended)

Running the Example
^^^^^^^^^^^^^^^^^^^

**Step 1: Hardware Setup**
   
   * Connect PN532 module to Arduino Nano:
     
     * **SPI mode (recommended)**:
       
       * SCK → Pin 13
       * MISO → Pin 12
       * MOSI → Pin 11
       * SS → Pin 10
       * IRQ → Pin 2 (optional, for IRQ mode)
       * RST → Pin 3
       * VCC → 3.3V or 5V (check your module)
       * GND → GND
   
   * Connect Arduino to computer via USB

**Step 2: Build and Upload**
   
   The examples can be built directly from the ``examples/`` directory using PlatformIO environments. This keeps your main system code intact.
   
   **Method 1: VS Code PlatformIO Extension (Recommended)**
   
   1. Open project folder in VS Code
   2. Option A: Use PlatformIO Sidebar
      
      * Click PlatformIO icon in left sidebar
      * Expand ``PROJECT TASKS`` → ``task1_read``
      * Click ``Upload`` (builds and uploads automatically)
      * Click ``Monitor`` to view serial output
   
   3. Option B: Use VS Code Tasks
      
      * Press Ctrl+Shift+P (Cmd+Shift+P on macOS)
      * Type "Tasks: Run Task"
      * Select ``PlatformIO: Upload Task 1 (Read)``
      * Then select ``PlatformIO: Monitor Task 1``
   
   **Method 2: Command Line with Python Module (Always Works)**
   
   .. code-block:: bash
   
      # Build and upload read example
      python -m platformio run -e task1_read --target upload
      
      # Open serial monitor
      python -m platformio device monitor --baud 115200
   
   **Method 3: Command Line (if pio is in PATH)**
   
   .. code-block:: bash
   
      # Build and upload read example
      pio run -e task1_read --target upload
      
      # Open serial monitor
      pio device monitor --baud 115200
   
   **Method 4: With specific COM port**
   
   .. code-block:: bash
   
      # Add --upload-port to any of the above commands
      python -m platformio run -e task1_read --target upload --upload-port COM13
      pio run -e task1_read --target upload --upload-port COM13
   
   .. note::
      
      **Changing Default Environment for Upload Button**: 
      
      The PlatformIO extension's upload button builds the default environment specified in ``platformio.ini``.
      To change which example is uploaded by the button:
      
      1. Open ``platformio.ini``
      2. Modify the ``default_envs`` line:
         
         .. code-block:: ini
         
            [platformio]
            default_envs = task1_read  # Change to: task1_read, task2_write, or nanoatmega328
      
      3. Save and use the upload button
      
      Alternatively, use VS Code tasks or terminal commands to specify the environment explicitly.

**Step 3: Finding Your COM Port**
   
   Windows:
   
   .. code-block:: powershell
   
      # List available COM ports
      pio device list
      
      # Or check Device Manager → Ports (COM & LPT)
   
   Linux/macOS:
   
   .. code-block:: bash
   
      # List USB serial devices
      pio device list
      
      # Or
      ls /dev/tty.*        # macOS
      ls /dev/ttyUSB*      # Linux
      ls /dev/ttyACM*      # Linux (alternative)

**Step 4: Open Serial Monitor**
   
   After uploading, open the serial monitor to view output:
   
   .. code-block:: bash
   
      # Monitor with correct baud rate (115200)
      pio device monitor --baud 115200
      
      # Or if using Python module
      python -m platformio device monitor --baud 115200
   
   **Important**: The examples use 115200 baud rate. If you see garbled text, verify the baud rate is correct.

**Step 5: Testing**
   
   * Place an NFC card near the reader
   * Observe the UID and card type in serial output
   * Try different card types
   * Press Ctrl+C to exit the monitor

Expected Output
^^^^^^^^^^^^^^^

.. code-block:: text

   ═══════════════════════════════════════════
        NFC TAG READ EXAMPLE - TASK 1         
   ═══════════════════════════════════════════
   
   Configuration:
     Communication: SPI
     Read Mode: IRQ (Interrupt-based)
   
   ✓ NFC reader initialized successfully!
   Ready! Place a card near the reader...
   
   ┌─────────────────────────────────────┐
   │       CARD DETECTED                 │
   ├─────────────────────────────────────┤
   │ Card Type: NTAG (213/215/216)
   │ Physical UID: 04 A2 3C 12 5E 4F 80
   │ UID Length: 7 bytes
   └─────────────────────────────────────┘

Task 2: Writing to NFC Tags
----------------------------

The second course task focuses on writing data to NFC cards. Different card types require different write approaches.

.. note::
   
   After uploading the write example, always open the serial monitor to see the results:
   
   .. code-block:: bash
   
      pio device monitor --baud 115200
   
   The baud rate **must** be 115200, otherwise you'll see garbled output.

.. warning::
   
   **Memory Usage Note**: The write example uses more RAM than the read example due to write verification
   and multiple String objects. If you encounter "data size greater than maximum allowed" errors:
   
   1. The code will still work but without buffer overflow protection
   2. Reduce serial output strings if needed
   3. Use the optimized build flags (already included in platformio.ini)
   4. Consider simplifying the example by removing some write operations
   
   Arduino Nano has only 2KB RAM. The examples are designed to be educational with verbose output,
   which uses more memory than a production system would.

Learning Objectives
^^^^^^^^^^^^^^^^^^^

* Understand the difference between block-based and page-based storage
* Write text strings to NFC cards
* Write binary data to specific addresses
* Verify written data
* Handle authentication (for Mifare Classic)
* Avoid common mistakes (writing to protected areas)

Example Code: write_example.cpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Location: ``examples/write_example.cpp``

**Complete Source Code**

The full example code demonstrates writing to both NTAG and Mifare Classic cards with verification 
and multiple write operation examples.

.. literalinclude:: ../../examples/write_example.cpp
   :language: cpp
   :linenos:
   :caption: examples/write_example.cpp - Complete NFC Tag Writing Example

**Key Features**

This example demonstrates writing to both NTAG and Mifare Classic cards:

.. code-block:: cpp

.. literalinclude:: ../../examples/write_example.cpp
   :language: cpp
   :linenos:
   :caption: examples/write_example.cpp - Complete NFC Tag Writing Example

**Key Features**

* Auto-detect card type and write appropriately
* Multiple write examples (string, binary data, type-specific)
* Write verification to ensure data integrity
* Mifare Classic authentication with default keys
* Safe memory area usage (avoids manufacturer blocks and sector trailers)
* Comprehensive error reporting
* F() macro for flash string storage (memory optimization)

Card Storage Structures
^^^^^^^^^^^^^^^^^^^^^^^

**NTAG / Mifare Ultralight (Page-based)**

* **Page size**: 4 bytes
* **User pages**: Page 4 onwards (pages 0-3 are reserved)
* **No authentication** required
* **Example**: NTAG213 has 45 pages (180 bytes user memory)

.. code-block:: text

   Page 0:  [Manufacturer Data - DO NOT WRITE]
   Page 1:  [Internal Data - DO NOT WRITE]
   Page 2:  [Lock Bytes - DO NOT WRITE]
   Page 3:  [Capability Container]
   Page 4:  [User Data] ← Safe to write
   Page 5:  [User Data] ← Safe to write
   ...

**Mifare Classic (Block-based)**

* **Block size**: 16 bytes
* **Sector structure**: 4 blocks per sector
* **User blocks**: Block 4+ (avoid sector trailers)
* **Authentication required**: Default key FF FF FF FF FF FF

.. code-block:: text

   Sector 0:
     Block 0: [Manufacturer Block - DO NOT WRITE]
     Block 1: [Data]
     Block 2: [Data]
     Block 3: [Sector Trailer - Contains keys]
   
   Sector 1:
     Block 4: [Data] ← Safe to write
     Block 5: [Data] ← Safe to write
     Block 6: [Data] ← Safe to write
     Block 7: [Sector Trailer - DO NOT WRITE]

Write Functions Reference
^^^^^^^^^^^^^^^^^^^^^^^^^^

**Auto-detect Functions**

.. code-block:: cpp

   // Automatically detects card type and writes appropriately
   NFCWriteResult writeString(const String& text, 
                              uint8_t startAddress = 0, 
                              bool verify = true);
   
   NFCWriteResult writeData(const uint8_t* data, 
                           uint8_t dataLength, 
                           uint8_t startAddress = 0, 
                           bool verify = true);

**NTAG/Ultralight Specific**

.. code-block:: cpp

   // Write to specific page (4 bytes)
   NFCWriteResult writeNTAG(uint8_t page, 
                           const uint8_t* data, 
                           uint8_t dataLength, 
                           bool verify = true);
   
   // Write string across multiple pages
   NFCWriteResult writeNTAGString(uint8_t startPage, 
                                 const String& text, 
                                 bool verify = true);

**Mifare Classic Specific**

.. code-block:: cpp

   // Write to specific block (16 bytes)
   NFCWriteResult writeMifareClassic(uint8_t block, 
                                     const uint8_t* data, 
                                     uint8_t dataLength,
                                     const uint8_t* key = DEFAULT_KEY,
                                     bool useKeyB = false,
                                     bool verify = true);
   
   // Write string across multiple blocks
   NFCWriteResult writeMifareClassicString(uint8_t startBlock, 
                                           const String& text,
                                           const uint8_t* key = DEFAULT_KEY,
                                           bool useKeyB = false,
                                           bool verify = true);

Running the Example
^^^^^^^^^^^^^^^^^^^

1. **Prepare a card**: Use a spare NFC card (not your important cards!)

2. **Build and upload**:
   
   Using PlatformIO environments (does not overwrite main system):
   
   .. code-block:: bash
   
      # Build and upload write example
      python -m platformio run -e task2_write --target upload
      
      # Open serial monitor (REQUIRED to see output)
      python -m platformio device monitor --baud 115200
   
   With specific port:
   
   .. code-block:: bash
   
      # Windows
      python -m platformio run -e task2_write --target upload --upload-port COM13
      python -m platformio device monitor --baud 115200
      
      # Linux/macOS
      python -m platformio run -e task2_write --target upload --upload-port /dev/ttyUSB0
      python -m platformio device monitor --baud 115200
   
   Using VS Code tasks:
   
   * Press Ctrl+Shift+P → ``Tasks: Run Task`` → ``PlatformIO: Upload Task 2 (Write)``
   * Then: Ctrl+Shift+P → ``Tasks: Run Task`` → ``PlatformIO: Monitor Task 2``
   
   Using PlatformIO sidebar:
   
   * Change ``default_envs`` in platformio.ini to ``task2_write``
   * Click Upload button in VS Code status bar
   * Click Monitor button to view output

3. **Test writing**:
   
   * Place card on reader
   * Observe write operations in serial output
   * Verify data was written correctly
   * Press Ctrl+C to exit monitor

.. note::
   
   **Baud Rate is Critical**: The examples use **115200 baud**. If you see garbled characters in the 
   serial monitor, verify you're using the correct baud rate. Default Arduino serial monitor often 
   uses 9600 baud which will not work.

Safety Tips
^^^^^^^^^^^

.. warning::
   
   **DO NOT attempt to write to these areas:**
   
   * **Block 0** (Manufacturer block) - **READ-ONLY**, cannot be written (contains UID and manufacturer data)
   * **Sector trailers** (blocks 3, 7, 11, 15, etc.) - Contains authentication keys, writing incorrect data will lock the sector
   * **Lock bytes** (NTAG/Ultralight) - May permanently lock pages, making them read-only
   * **OTP (One-Time Programmable) areas** - Can only be written once
   
   **Always use safe areas:**
   
   * Mifare Classic: Blocks 4, 5, 6, 8, 9, 10, etc. (user data blocks, avoid sector trailers)
   * NTAG/Ultralight: Pages 4 and above (check datasheet for lock byte locations)

Common Issues and Solutions
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

**Authentication Failed (Mifare Classic)**

* **Cause**: Wrong authentication key
* **Solution**: Try default key ``FF FF FF FF FF FF`` or obtain the correct key

**Write Failed - Card Not Found**

* **Cause**: Card moved during write operation
* **Solution**: Hold card steady on reader during entire operation

**Verification Failed**

* **Cause**: Data written doesn't match expected
* **Solution**: Check card is not write-protected, verify correct address

**RAM Usage Warning ("data size greater than maximum")**

* **Cause**: Write example uses verbose String output and multiple features
* **Impact**: Code will still work, but stack overflow protection is reduced
* **Solutions**:
  
  * The optimized build flags in platformio.ini already help
  * Reduce serial debugging output if needed
  * Remove some example write operations
  * This is expected for educational examples with verbose output
  * Production code would use less RAM by reducing debug strings

**Card Appears Bricked / Sector Locked**

* **Cause**: Attempted to write to Block 0 (will fail, but card is fine) OR wrote incorrect data to sector trailer
* **Note**: Block 0 is read-only and cannot be damaged by write attempts
* **Solution for locked sector**: Sector may be permanently locked if trailer was corrupted - use a new card or different sector

Advanced Topics
---------------

Card Cloning
^^^^^^^^^^^^

The access control system includes advanced card cloning features (covered in :doc:`card_cloning`):

* Clone UIDs to standard cards (no magic cards needed)
* Store cloned UID in custom sector
* Automatic UID selection for access control

NDEF Messages
^^^^^^^^^^^^^

For advanced users, NDEF (NFC Data Exchange Format) can be used to create standard NFC tags readable by smartphones:

* URL records
* Text records
* Smart poster
* vCard

See ``Adafruit_PN532`` library documentation for NDEF support.

Next Steps
----------

* Explore the full access control system in :doc:`usage`
* Learn about card cloning in :doc:`card_cloning`
* Review the API documentation in :doc:`api`
* Study the hardware setup in :doc:`hardware`

Tips for Development
--------------------

1. **Always test with spare cards** - Don't use important cards during development
2. **Enable serial debugging** - Monitor operations in real-time
3. **Use verification** - Always verify writes to catch errors early
4. **Start simple** - Begin with reading before attempting writes
5. **Document your addresses** - Keep track of what data is stored where
6. **Handle errors gracefully** - Check return values and display meaningful messages

See Also
--------

* :doc:`api` - Complete API reference
* :doc:`hardware` - Hardware setup and wiring
* :doc:`troubleshooting` - Common problems and solutions
* :doc:`card_cloning` - Advanced card cloning features
