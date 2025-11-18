Troubleshooting
===============

This guide helps diagnose and resolve common issues with the NFC Access
Control System.

Hardware Issues
---------------

PN532 Module Not Detected
^^^^^^^^^^^^^^^^^^^^^^^^^^

**Symptoms**:

* Serial output: "Failed to initialize PN532"
* System hangs during initialization
* No response from NFC reader

**Solutions**:

1. **Check SPI Mode Configuration**
   
   * Verify PN532 DIP switches are set for SPI mode
   * Most modules: Switch 1 ON, Switch 2 OFF
   * Consult your module's documentation

2. **Verify Connections**
   
   * Check all SPI pins (SCK, MISO, MOSI, SS)
   * Ensure IRQ and RST pins are connected
   * Test continuity with multimeter

3. **Power Supply**
   
   * Verify PN532 receives 5V on VCC pin
   * Check ground connection
   * Ensure power supply provides adequate current (>300mA)

4. **Test with Different Pins**
   
   * Try different Arduino pins for SS, IRQ, RST
   * Update pin definitions in Config.h accordingly

5. **Module Reset**
   
   * Disconnect power for 10 seconds
   * Reconnect and restart

LCD Display Issues
^^^^^^^^^^^^^^^^^^

**LCD Shows Nothing**

Causes and fixes:

* **No Power**: Check VDD and VSS connections
* **Contrast Too Low**: Adjust potentiometer on V0 pin
* **Wrong Wiring**: Verify RS, E, and D4-D7 connections
* **Initialization Failed**: Check pin definitions in Config.h

**LCD Shows Random Characters**

Causes and fixes:

* **Loose Connections**: Check all data pins (D4-D7)
* **Timing Issues**: Add delay in initialization code
* **Power Issues**: Ensure stable 5V supply
* **Bad LCD**: Test with known working display

**Backlight Not Working**

Causes and fixes:

* **No Connection**: Check A and K pins
* **Missing Resistor**: Add 220Ω resistor to anode
* **LED Burned Out**: Replace backlight or LCD

Button Problems
^^^^^^^^^^^^^^^

**Buttons Not Responding**

Causes and fixes:

1. **Check Wiring**: Verify connections to analog pins
2. **Test Buttons**: Use multimeter to check continuity
3. **Pull-down Resistors**: Ensure 10kΩ resistors are in place
4. **Pin Configuration**: Check pin definitions in Config.h

**Multiple Button Presses Detected**

Causes and fixes:

* **Bouncing**: Increase DEBOUNCE_DELAY in Config.h
* **Electrical Noise**: Add capacitors across button terminals
* **Wiring Issues**: Shorten button wires, avoid parallel routing

**Wrong Buttons Activated**

* **Pin Mapping**: Verify analog pin assignments
* **Reversed Connections**: Check button-to-pin mapping

Relay Issues
^^^^^^^^^^^^

**Relay Not Activating**

Causes and fixes:

1. **Check Relay Module**
   
   * Test relay independently with direct 5V/GND
   * Verify LED indicator on relay module
   * Try different relay module

2. **Wiring**
   
   * Verify IN pin connection to A4
   * Check power supply to relay
   * Ensure common ground with Arduino

3. **Insufficient Power**
   
   * Relay may draw more current than Arduino can provide
   * Use external power supply for relay
   * Add flyback diode if using bare relay

4. **Software Issue**
   
   * Check RELAY_PIN definition
   * Verify relay logic (active high vs low)
   * Add debug Serial.println() in grantAccess()

**Relay Stuck On/Off**

* **Software Bug**: Check state machine logic
* **Hardware Fault**: Replace relay module
* **Power Issue**: Verify power supply stability

Card Reading Issues
-------------------

Cards Not Detected
^^^^^^^^^^^^^^^^^^

**No Cards Detected**

Causes and fixes:

1. **IRQ Mode Issues**
   
   * Verify IRQ pin connected to D2
   * Check IRQ pin is configured as interrupt
   * Try POLLING mode as test

2. **Reader Sensitivity**
   
   * Hold card closer (< 2cm)
   * Try different card orientations
   * Test with multiple cards

3. **Communication Problems**
   
   * Check SPI connections
   * Verify PN532 initialization succeeds
   * Monitor serial output for errors

4. **Firmware Issue**
   
   * Re-upload firmware
   * Check PN532 library version
   * Update to latest Adafruit_PN532 library

**Intermittent Detection**

Causes and fixes:

* **Loose Connections**: Check and resolder SPI pins
* **Power Fluctuations**: Add decoupling capacitors
* **EMI**: Keep wires short, add shielding
* **Card Damage**: Test with known working cards

**Wrong UID Read**

Causes and fixes:

* **Read Errors**: Implement UID verification reads
* **Bit Errors**: Check SPI signal integrity
* **Cloned UID Issues**: See cloning troubleshooting below

Specific Card Types
^^^^^^^^^^^^^^^^^^^

**Mifare Classic Not Reading**

* Verify card is 13.56MHz (not 125kHz)
* Check card is not damaged or demagnetized
* Try different Mifare Classic cards

**Ultralight/NTAG Issues**

* These cards have different memory structure
* Cloning not supported for these types
* Can still register physical UID for access control

Software Issues
---------------

Compilation Errors
^^^^^^^^^^^^^^^^^^

**Library Not Found**

.. code-block:: text

   fatal error: Adafruit_PN532.h: No such file or directory

Solution::

    pio lib install "Adafruit PN532@^1.2.2"

**Multiple Library Versions**

* Remove duplicate libraries
* Use PlatformIO's lib_deps in platformio.ini
* Clear .pio folder and rebuild

**Compilation Warnings**

* Update code to fix warnings
* Can ignore most warnings initially
* Fix before production deployment

Upload Failures
^^^^^^^^^^^^^^^

**Port Not Found**

Windows::

    # Check Device Manager for COM port
    # Install CH340 drivers if needed

Linux::

    ls /dev/ttyUSB*
    sudo chmod 666 /dev/ttyUSB0

**Permission Denied**

Linux::

    sudo usermod -a -G dialout $USER
    # Log out and back in

**Upload Timeout**

* Press reset button during upload
* Try lower upload speed in platformio.ini
* Check USB cable quality

Runtime Errors
^^^^^^^^^^^^^^

**System Hangs at Startup**

Check serial output for last message:

* "Initializing LCD...": LCD connection issue
* "Initializing PN532...": NFC reader issue
* No output: Serial not initialized or board not starting

**Random Resets**

Causes:

* **Power Issues**: Insufficient power supply
* **Memory Overflow**: Check RAM usage
* **Stack Overflow**: Reduce local variable sizes
* **Watchdog Timer**: Disable if enabled

**EEPROM Corruption**

Symptoms: All cards lost, wrong card count

Solution::

    // Reset EEPROM
    void resetEEPROM() {
        for (int i = 0; i < 512; i++) {
            EEPROM.write(i, 0xFF);
        }
        EEPROM.write(0, 0xAB);  // Magic byte
        EEPROM.write(1, 0xCD);  // Magic byte
        EEPROM.write(2, 0);     // Card count
    }

Cloning Issues
--------------

Authentication Failed
^^^^^^^^^^^^^^^^^^^^^

**Error**: "Failed to authenticate to sector"

Causes and fixes:

1. **Non-Default Keys**
   
   * Target card uses custom keys
   * Cannot clone without knowing keys
   * Use different card with default keys

2. **Card Type Incompatible**
   
   * Not a Mifare Classic card
   * Use only Mifare Classic 1K or 4K
   * Check card type before cloning

3. **Damaged Card**
   
   * Sector 1 may be corrupted
   * Try different target card
   * Test card with NFC Tools app

Write Failed
^^^^^^^^^^^^

**Error**: "Failed to write UID block"

Causes and fixes:

* **Write-Protected**: Card sector is locked
* **Card Full**: Sector has been written too many times
* **Power Issue**: Voltage drop during write
* **Damaged Card**: Replace target card

Verification Failed
^^^^^^^^^^^^^^^^^^^

**Error**: "Verification failed after write"

Causes and fixes:

1. **Intermittent Connection**
   
   * Hold card steady during entire operation
   * Improve antenna coupling
   * Reduce distance to reader

2. **Write Corruption**
   
   * Retry cloning operation
   * Use fresh target card
   * Check power supply stability

3. **Memory Degradation**
   
   * Card nearing end of write life
   * Use new card
   * Avoid excessive rewrites

Cloned Card Not Working
^^^^^^^^^^^^^^^^^^^^^^^^

**Cloned Card Not Recognized**

Diagnostics:

1. **Read Cloned UID Directly**
   
   * Use serial monitor to check UID
   * Verify cloned UID matches source
   * Check UID length is correct

2. **Check Metadata**
   
   * Read Block 5 for flags
   * Verify valid flag is set
   * Check for corruption

3. **Re-clone**
   
   * Try cloning operation again
   * Use different target card
   * Verify source card still readable

Access Control Issues
---------------------

Authorized Card Denied
^^^^^^^^^^^^^^^^^^^^^^

**Registered Card Not Granting Access**

Diagnostics:

1. **Check Card Registration**
   
   * Use "List Cards" menu
   * Verify card is in database
   * Check UID matches

2. **UID Reading**
   
   * Read card multiple times
   * Check for consistent UID
   * Try re-registering

3. **EEPROM Issues**
   
   * Check stored card count
   * Verify EEPROM not corrupted
   * Reset and re-register all cards

Unauthorized Card Granted
^^^^^^^^^^^^^^^^^^^^^^^^^^

**Unknown Card Getting Access**

This is a security issue:

1. **Check All Registered Cards**
   
   * List all stored UIDs
   * Remove unknown entries
   * Investigate how card was added

2. **UID Collision**
   
   * Very unlikely with 4-byte UIDs
   * Check for partial UID matching bug
   * Verify comparison logic

3. **Cloning Conflict**
   
   * Check for duplicate cloned UIDs
   * Verify cloned cards are authorized
   * Review cloning logs

Relay Timing Issues
^^^^^^^^^^^^^^^^^^^

**Door Not Opening Long Enough**

* Increase ACCESS_GRANT_DURATION in Config.h
* Verify relay wiring (NO vs NC)
* Check door strike specifications

**Relay Stays Active**

* Check for software logic errors
* Verify relay deactivation code
* Add watchdog timer for safety

Menu System Issues
------------------

Menu Not Accessible
^^^^^^^^^^^^^^^^^^^

* **SELECT Button**: Verify button works
* **State Machine**: Check current system state
* **Software Bug**: Review state transition logic

Cannot Exit Menu
^^^^^^^^^^^^^^^^

* **BACK Button**: Test button functionality
* **Logic Error**: Check exit menu code
* **Workaround**: Power cycle to return to IDLE

Cards Not Listing
^^^^^^^^^^^^^^^^^

* **No Cards**: Register at least one card
* **Display Issue**: Check LCD connections
* **EEPROM Read**: Verify EEPROM data integrity

Diagnostic Tools
----------------

Serial Monitor Output
^^^^^^^^^^^^^^^^^^^^^

Enable debug mode in Config.h::

    #define DEBUG_MODE 1

This provides detailed logging:

* Card UIDs as hex
* Authentication attempts
* Read/write operations
* State transitions
* Error messages

Example output::

    [DEBUG] Card detected
    [DEBUG] Physical UID: AB 12 CD 34
    [DEBUG] Checking for cloned UID...
    [DEBUG] No cloned UID found
    [DEBUG] Using physical UID
    [DEBUG] Checking authorization...
    [DEBUG] Card found at index 5
    [DEBUG] Access granted

Testing Individual Components
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Create test sketches for each component:

**LCD Test** (already in test/lcd_test.cpp):

* Displays test patterns
* Verifies all LCD functions
* Tests backlight and contrast

**Button Test**::

    void loop() {
        Serial.print("Buttons: ");
        Serial.print(digitalRead(BTN_UP));
        Serial.print(digitalRead(BTN_DOWN));
        Serial.print(digitalRead(BTN_SELECT));
        Serial.println(digitalRead(BTN_BACK));
        delay(100);
    }

**NFC Test**::

    void loop() {
        uint8_t uid[7];
        uint8_t uidLen;
        if (reader.readCard(uid, &uidLen)) {
            Serial.print("UID: ");
            for (int i = 0; i < uidLen; i++) {
                Serial.print(uid[i], HEX);
                Serial.print(" ");
            }
            Serial.println();
        }
        delay(500);
    }

Common Error Codes
------------------

.. list-table::
   :header-rows: 1
   :widths: 30 70

   * - Error Message
     - Meaning
   * - "Failed to initialize PN532"
     - PN532 not responding on SPI bus
   * - "Authentication failed"
     - Cannot authenticate to card sector
   * - "Write failed"
     - Cannot write data to card
   * - "Verification failed"
     - Written data doesn't match read-back
   * - "Card already registered"
     - Attempting to register duplicate card
   * - "Storage full"
     - Maximum 40 cards already stored
   * - "Card not found"
     - Attempting to delete non-existent card

Getting Help
------------

Before Seeking Support
^^^^^^^^^^^^^^^^^^^^^^

1. Check this troubleshooting guide
2. Review serial monitor output
3. Test individual components
4. Verify all connections
5. Try with fresh Arduino/cards

Information to Provide
^^^^^^^^^^^^^^^^^^^^^^^

When reporting issues:

* Exact error messages
* Serial monitor output
* Hardware configuration
* Steps to reproduce
* PlatformIO/library versions
* Card types being used

Support Channels
^^^^^^^^^^^^^^^^

* GitHub Issues: https://github.com/Menazaa/rwu-nfc/issues
* Project documentation: :doc:`index`
* Arduino forums: https://forum.arduino.cc
* PlatformIO community: https://community.platformio.org

Additional Resources
--------------------

* PN532 Datasheet: NXP PN532 documentation
* Mifare Classic Specification: NXP Mifare Classic datasheet
* Arduino Reference: https://www.arduino.cc/reference/
* PlatformIO Docs: https://docs.platformio.org
