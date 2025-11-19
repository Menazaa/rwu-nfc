Usage Guide
===========

This guide explains how to operate the NFC Access Control System, including
card management, menu navigation, and day-to-day usage.

System Startup
--------------

When the system powers on:

1. The LCD displays a welcome message: "NFC Access Control"
2. After initialization, it enters IDLE mode
3. The display shows: "Ready to scan..."
4. The system is now ready to read NFC cards

Normal Operation
----------------

Scanning Cards
^^^^^^^^^^^^^^

When in IDLE mode:

1. Hold an NFC card near the PN532 reader (within 4 cm)
2. The system reads the card UID
3. It checks if the card is authorized

**If card is authorized**:

* LCD displays: "Access Granted" with card ID
* Relay activates for the configured duration (default: 3 seconds)
* Green LED or buzzer indicates success (if configured)
* System returns to IDLE mode

**If card is not authorized**:

* LCD displays: "Access Denied"
* Relay remains inactive
* Red LED or buzzer indicates failure (if configured)
* System returns to IDLE mode after 2 seconds

Menu System
-----------

Accessing the Menu
^^^^^^^^^^^^^^^^^^

From IDLE mode, press the **SELECT** button to enter the menu system.

.. graphviz::
   :caption: Menu Navigation Flow

   digraph menu_flow {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       idle [label="IDLE Mode", fillcolor=lightgreen, style=filled];
       menu [label="Main Menu", fillcolor=lightblue, style=filled];
       
       register [label="Register Card", fillcolor=yellow, style=filled];
       delete [label="Delete Card", fillcolor=orange, style=filled];
       list [label="List Cards", fillcolor=cyan, style=filled];
       clone [label="Clone Card", fillcolor=violet, style=filled];
       info [label="System Info", fillcolor=pink, style=filled];
       
       clone_src [label="Scan Source\nCard", fillcolor=lightviolet, style=filled];
       clone_tgt [label="Scan Target\nCard", fillcolor=lightviolet, style=filled];
       
       idle -> menu [label="SELECT"];
       menu -> idle [label="Exit Menu"];
       
       menu -> register [label="SELECT on\nRegister"];
       menu -> delete [label="SELECT on\nDelete"];
       menu -> list [label="SELECT on\nList"];
       menu -> clone [label="SELECT on\nClone"];
       menu -> info [label="SELECT on\nInfo"];
       
       register -> menu [label="Done/BACK"];
       delete -> menu [label="Done/BACK"];
       list -> menu [label="BACK"];
       info -> menu [label="BACK"];
       
       clone -> clone_src;
       clone_src -> clone_tgt [label="Source\nScanned"];
       clone_tgt -> menu [label="Done/BACK"];
       clone_src -> menu [label="BACK"];
   }

Menu Navigation
^^^^^^^^^^^^^^^

* **UP button**: Move to previous menu item
* **DOWN button**: Move to next menu item
* **SELECT button**: Choose current menu item
* **BACK button**: Return to previous screen or exit to IDLE

Main Menu Items
^^^^^^^^^^^^^^^

The main menu includes:

1. Register Card
2. Delete Card
3. List Cards
4. Clone Card
5. System Info
6. Exit Menu

Card Management
---------------

Registering a New Card
^^^^^^^^^^^^^^^^^^^^^^

To add a card to the authorized list:

1. Navigate to "Register Card" and press SELECT
2. LCD displays: "Present card to register"
3. Hold the card to the reader
4. System checks if card is already registered
5. If not registered:
   
   * Card UID is stored in EEPROM
   * LCD shows: "Card registered" with card number
   * System returns to menu after 2 seconds

6. If already registered:
   
   * LCD shows: "Card already exists"
   * No changes are made

**Capacity**: The system supports up to 40 cards.

Deleting a Card
^^^^^^^^^^^^^^^

To remove a card from the authorized list:

1. Navigate to "Delete Card" and press SELECT
2. LCD displays: "Present card to delete"
3. Hold the card to the reader
4. System searches for the card in EEPROM
5. If found:
   
   * Card is removed from storage
   * LCD shows: "Card deleted"
   * Remaining cards are reorganized

6. If not found:
   
   * LCD shows: "Card not found"
   * No changes are made

Listing Stored Cards
^^^^^^^^^^^^^^^^^^^^

To view all registered cards:

1. Navigate to "List Cards" and press SELECT
2. LCD displays cards one at a time
3. Each card shows:
   
   * Card number (1-40)
   * Card UID in hexadecimal

4. Use UP/DOWN to scroll through cards
5. Press BACK to exit

**Format**::

    Card 01:
    AB 12 CD 34

Advanced Features
-----------------

Card Cloning
^^^^^^^^^^^^

The system supports advanced card cloning using custom sector technology.

**To clone a card**:

1. Navigate to "Clone Card" and press SELECT
2. LCD displays: "Present source card"
3. Hold the source card (the card to copy from) to the reader
4. System reads the source UID
5. LCD displays: "Present target card"
6. Hold the target card (the card to write to) to the reader
7. System performs the cloning operation:
   
   * Authenticates to Sector 1 of target card
   * Writes source UID to Block 4 of target card
   * Writes metadata to Block 5
   * Verifies the write operation

8. If successful:
   
   * LCD shows: "Clone successful"
   * Target card now responds with source UID

9. If failed:
   
   * LCD shows error message
   * Target card is unchanged

**Important Notes**:

* Only works with Mifare Classic 1K/4K cards
* Uses default keys (all 0xFF) for authentication
* Target card must be writable
* Source card can be any NFC card type

See :doc:`card_cloning` for technical details.

System Information
^^^^^^^^^^^^^^^^^^

View system status:

1. Navigate to "System Info" and press SELECT
2. Display shows:
   
   * Firmware version
   * Total cards registered
   * Free card slots
   * EEPROM usage

3. Press BACK to return to menu

Customization
-------------

Access Duration
^^^^^^^^^^^^^^^

The relay activation time can be configured in ``include/Config.h``::

    #define ACCESS_GRANT_DURATION 3000  // milliseconds

Modify this value and rebuild the firmware.

Button Configuration
^^^^^^^^^^^^^^^^^^^^

Button pins are defined in ``include/Config.h``. Change if using different pins::

    #define BTN_UP     (A0)
    #define BTN_DOWN   (A1)
    #define BTN_SELECT (A2)
    #define BTN_BACK   (A3)

Card Capacity
^^^^^^^^^^^^^

Maximum number of cards is set in ``include/Config.h``::

    #define MAX_CARDS 40

Increase or decrease based on your EEPROM size (1KB for Arduino Nano).

Best Practices
--------------

Regular Operation
^^^^^^^^^^^^^^^^^

* Keep the reader area clear of metal objects
* Present cards squarely to the reader
* Don't move the card during reading
* Wait for confirmation before removing card

Card Management
^^^^^^^^^^^^^^^

* Periodically review registered cards
* Remove lost or stolen cards immediately
* Keep a backup list of authorized card UIDs
* Test new cards after registration

System Maintenance
^^^^^^^^^^^^^^^^^^

* Check power supply stability
* Clean the NFC reader antenna periodically
* Verify button responsiveness
* Monitor serial output for errors

Security Considerations
-----------------------

Physical Security
^^^^^^^^^^^^^^^^^

* Mount reader in a protected location
* Protect Arduino and power supply from tampering
* Use tamper-evident enclosures
* Consider backup power for critical applications

Access Control
^^^^^^^^^^^^^^

* Limit physical access to the menu system
* Regularly audit registered cards
* Use card cloning only for authorized purposes
* Keep default authentication keys secure

Serial Output
-------------

The system provides detailed logging via serial port at 115200 baud:

* Card scan events
* Authorization results
* Menu actions
* Error messages
* System diagnostics

Example output::

    NFC Access Control System
    Firmware v1.0.0
    Initializing PN532...
    PN532 initialized in IRQ mode
    System ready
    
    Card detected: AB 12 CD 34
    Card authorized - Access granted
    Relay activated
    
    Entering menu mode
    Menu item selected: Register Card
    Card registered: #15

Troubleshooting Common Issues
------------------------------

Card Not Reading
^^^^^^^^^^^^^^^^

* Hold card closer to reader (< 4 cm)
* Try different card orientation
* Check if card is supported type
* Verify PN532 is powered and configured

False Readings
^^^^^^^^^^^^^^

* Keep metal objects away from reader
* Check for electrical interference
* Verify IRQ pin connection
* Reduce reader sensitivity if needed

Menu Not Responding
^^^^^^^^^^^^^^^^^^^^

* Check button connections
* Verify button pins in Config.h
* Test buttons individually
* Check for button debouncing issues

Relay Not Activating
^^^^^^^^^^^^^^^^^^^^^

* Verify relay pin connection
* Check relay power supply
* Test relay independently
* Review relay configuration

For more troubleshooting, see :doc:`troubleshooting`.
