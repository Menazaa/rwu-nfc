Hardware Setup
==============

This section describes the hardware components required for the NFC Access
Control System and provides detailed wiring instructions.

Required Components
-------------------

Core Components
^^^^^^^^^^^^^^^

* **Arduino Nano** (ATmega328P)
  
  - Microcontroller board based on ATmega328P
  - Operating Voltage: 5V
  - Digital I/O Pins: 14
  - Analog Input Pins: 8

* **PN532 NFC Module**
  
  - NFC/RFID controller
  - Operating frequency: 13.56 MHz
  - Communication: SPI mode (with IRQ support)
  - Support for Mifare Classic, Mifare Ultralight, NTAG

* **16x2 LCD Display**
  
  - Alphanumeric LCD with 16 columns and 2 rows
  - HD44780 compatible controller
  - 4-bit parallel interface mode
  - Backlight: LED (optional)

* **Push Buttons** (4 pieces)
  
  - Momentary tactile switches
  - Used for: UP, DOWN, SELECT, BACK navigation

* **Relay Module**
  
  - 5V single-channel relay
  - Used to control door lock or strike
  - Supports both NO and NC configurations

Additional Components
^^^^^^^^^^^^^^^^^^^^^

* Breadboard or custom PCB
* Jumper wires (male-to-male, male-to-female)
* 10kΩ resistors (4 pieces) for button pull-downs
* 220Ω resistor for LCD backlight (if needed)
* 10kΩ potentiometer for LCD contrast adjustment
* 5V power supply (capable of 500mA minimum)

Pin Configuration
-----------------

PN532 NFC Module (SPI Mode)
^^^^^^^^^^^^^^^^^^^^^^^^^^^

Connect the PN532 module to the Arduino Nano as follows:

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - PN532 Pin
     - Arduino Pin
     - Description
   * - VCC
     - 5V
     - Power supply
   * - GND
     - GND
     - Ground
   * - SCK
     - D13
     - SPI Clock
   * - MISO
     - D12
     - SPI Master In Slave Out
   * - MOSI
     - D11
     - SPI Master Out Slave In
   * - SS
     - D10
     - SPI Chip Select
   * - IRQ
     - D2
     - Interrupt Request
   * - RST
     - D3
     - Reset

**Important**: Ensure the PN532 module is configured for SPI mode by setting
the appropriate DIP switches or solder jumpers on the module.

LCD Display (4-bit Parallel)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 20 20 60

   * - LCD Pin
     - Arduino Pin
     - Description
   * - VSS
     - GND
     - Ground
   * - VDD
     - 5V
     - Power supply
   * - V0
     - POT
     - Contrast (connect to potentiometer)
   * - RS
     - D4
     - Register Select
   * - RW
     - GND
     - Read/Write (tied to ground for write-only)
   * - E
     - D5
     - Enable
   * - D4
     - D6
     - Data bit 4
   * - D5
     - D7
     - Data bit 5
   * - D6
     - D8
     - Data bit 6
   * - D7
     - D9
     - Data bit 7
   * - A
     - 5V
     - Backlight anode (via 220Ω resistor)
   * - K
     - GND
     - Backlight cathode

Navigation Buttons
^^^^^^^^^^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 30 30 40

   * - Button
     - Arduino Pin
     - Function
   * - UP
     - A0
     - Navigate up in menus
   * - DOWN
     - A1
     - Navigate down in menus
   * - SELECT
     - A2
     - Select menu item
   * - BACK
     - A3
     - Return to previous menu

Each button should be connected between the Arduino pin and ground, with a
10kΩ pull-down resistor. Internal pull-ups are enabled in software.

Relay Module
^^^^^^^^^^^^

.. list-table::
   :header-rows: 1
   :widths: 30 30 40

   * - Relay Pin
     - Arduino Pin
     - Description
   * - VCC
     - 5V
     - Power supply
   * - GND
     - GND
     - Ground
   * - IN
     - A4
     - Control signal

Wiring Diagram
--------------

.. graphviz::
   :caption: Hardware Connection Diagram

   digraph hardware_connections {
       rankdir=LR;
       node [shape=record];
       
       // Arduino
       arduino [label="{Arduino Nano|{<d2>D2 (IRQ)|<d3>D3 (RST)|<d4>D4|<d5>D5|<d6>D6|<d7>D7|<d8>D8|<d9>D9|<d10>D10 (SS)|<d11>D11 (MOSI)|<d12>D12 (MISO)|<d13>D13 (SCK)|<a0>A0|<a1>A1|<a2>A2|<a3>A3|<a4>A4|<5v>5V|<gnd>GND}}", fillcolor=lightgreen, style=filled];
       
       // PN532
       pn532 [label="{PN532 NFC Module|{<irq>IRQ|<rst>RST|<ss>SS|<mosi>MOSI|<miso>MISO|<sck>SCK|<vcc>VCC|<gnd>GND}}", fillcolor=lightyellow, style=filled];
       
       // LCD
       lcd [label="{16x2 LCD Display|{<rs>RS|<e>E|<d4>D4|<d5>D5|<d6>D6|<d7>D7|<vdd>VDD|<vss>VSS|<v0>V0}}", fillcolor=lightblue, style=filled];
       
       // Buttons
       btn_up [label="Button UP", shape=box, fillcolor=orange, style=filled];
       btn_down [label="Button DOWN", shape=box, fillcolor=orange, style=filled];
       btn_select [label="Button SELECT", shape=box, fillcolor=orange, style=filled];
       btn_back [label="Button BACK", shape=box, fillcolor=orange, style=filled];
       
       // Relay
       relay [label="{Relay Module|{<in>IN|<vcc>VCC|<gnd>GND}}", fillcolor=pink, style=filled];
       
       // Potentiometer
       pot [label="10kΩ POT\n(Contrast)", shape=ellipse, fillcolor=lightgray, style=filled];
       
       // Power
       power_5v [label="5V", shape=ellipse, fillcolor=red, style=filled];
       power_gnd [label="GND", shape=ellipse, fillcolor=black, style=filled, fontcolor=white];
       
       // Connections
       arduino:d2 -> pn532:irq [label="IRQ"];
       arduino:d3 -> pn532:rst [label="RST"];
       arduino:d10 -> pn532:ss [label="SS"];
       arduino:d11 -> pn532:mosi [label="MOSI"];
       arduino:d12 -> pn532:miso [label="MISO"];
       arduino:d13 -> pn532:sck [label="SCK"];
       
       arduino:d4 -> lcd:rs [label="RS"];
       arduino:d5 -> lcd:e [label="E"];
       arduino:d6 -> lcd:d4 [label="D4"];
       arduino:d7 -> lcd:d5 [label="D5"];
       arduino:d8 -> lcd:d6 [label="D6"];
       arduino:d9 -> lcd:d7 [label="D7"];
       
       arduino:a0 -> btn_up [label="UP"];
       arduino:a1 -> btn_down [label="DOWN"];
       arduino:a2 -> btn_select [label="SELECT"];
       arduino:a3 -> btn_back [label="BACK"];
       
       arduino:a4 -> relay:in [label="Control"];
       
       arduino:5v -> power_5v;
       arduino:gnd -> power_gnd;
       
       power_5v -> pn532:vcc;
       power_5v -> lcd:vdd;
       power_5v -> relay:vcc;
       
       power_gnd -> pn532:gnd;
       power_gnd -> lcd:vss;
       power_gnd -> relay:gnd;
       
       pot -> lcd:v0 [label="Contrast"];
   }

.. note::
   A complete wiring diagram in Fritzing format is available in the
   ``hardware/`` directory of the project repository.

Power Considerations
--------------------

* The complete system draws approximately 200mA under normal operation
* Peak current during relay activation can reach 300mA
* Use a stable 5V power supply rated for at least 500mA
* If using USB power, ensure your USB port can supply sufficient current
* For production use, consider an external 5V power adapter

Assembly Tips
-------------

1. **Test Components Individually**: Before final assembly, test each component
   separately to ensure they work correctly.

2. **Use a Breadboard First**: Prototype on a breadboard before creating a
   permanent solution.

3. **Check Connections**: Double-check all wiring before applying power,
   especially power and ground connections.

4. **PN532 Mode Configuration**: Verify the PN532 is in SPI mode before
   connecting.

5. **LCD Contrast**: Adjust the contrast potentiometer so LCD text is clearly
   visible.

6. **Button Debouncing**: The software includes debouncing, but clean button
   contacts improve reliability.

Troubleshooting Hardware Issues
--------------------------------

LCD Not Displaying
^^^^^^^^^^^^^^^^^^

* Check power connections (VDD and GND)
* Adjust contrast potentiometer
* Verify data pin connections (D4-D7)
* Check RS and E pin connections

PN532 Not Detected
^^^^^^^^^^^^^^^^^^

* Verify PN532 is in SPI mode
* Check SPI connections (SCK, MISO, MOSI, SS)
* Ensure power supply is adequate
* Try resetting the module (RST pin)

Buttons Not Responding
^^^^^^^^^^^^^^^^^^^^^^^

* Verify button wiring and pull-down resistors
* Check for proper ground connection
* Test buttons with a multimeter
* Ensure analog pins are correctly defined in Config.h

Relay Not Activating
^^^^^^^^^^^^^^^^^^^^^

* Check relay control pin connection
* Verify relay power supply
* Test relay module independently
* Check if relay is configured correctly (NO vs NC)