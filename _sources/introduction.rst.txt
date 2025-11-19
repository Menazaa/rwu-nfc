Introduction
============

The NFC Access Control System is a student project developed for the NFC Technology 
course at RWU (Ravensburg-Weingarten University). Starting with two fundamental course 
tasks—reading and writing NFC tags—this project evolved into a complete access control 
solution built on Arduino technology with the PN532 NFC module.

Project Evolution
-----------------

This project demonstrates a structured learning approach:

**Course Task 1: Tag Reading**
  Introduction to NFC communication by reading card UIDs and identifying different 
  card types. See :doc:`examples` for the basic read implementation.

**Course Task 2: Tag Writing**
  Building on Task 1 to write data to NFC cards, understanding memory structures, 
  and handling different card types. See :doc:`examples` for the write implementation.

**Extended Project: Full Access Control System**
  Integrate reading and writing capabilities into a complete access control solution 
  with card management, persistent storage, LCD interface, and physical access control.

The result is a robust, feature-rich platform suitable for both educational purposes 
and real-world access control applications.

What is NFC?
------------

Near Field Communication (NFC) is a set of communication protocols that enable
two electronic devices to communicate over a distance of 4 cm or less. NFC operates
at 13.56 MHz and is commonly used for:

* **Contactless payment systems** - Credit cards, mobile payments
* **Access control and security** - Building entry, time tracking
* **Data exchange between devices** - File sharing, pairing
* **Smart posters and advertising** - Interactive marketing, product information
* **Public transportation** - Ticket validation, fare collection

NFC builds on RFID (Radio-Frequency Identification) technology and supports multiple
communication modes, making it versatile for various applications.

System Architecture
-------------------

The system consists of several key components working together:

.. graphviz::
   :caption: System Architecture Overview

   digraph system_architecture {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       // User layer
       user [label="User/Card", shape=ellipse, fillcolor=lightblue, style=filled];
       
       // Hardware layer
       subgraph cluster_hardware {
           label="Hardware Layer";
           style=filled;
           color=lightgrey;
           
           nfc [label="PN532 NFC\nReader", fillcolor=lightyellow, style=filled];
           lcd [label="16x2 LCD\nDisplay", fillcolor=lightyellow, style=filled];
           buttons [label="Navigation\nButtons (4)", fillcolor=lightyellow, style=filled];
           relay [label="Relay\nModule", fillcolor=lightyellow, style=filled];
           arduino [label="Arduino Nano\n(ATmega328P)", fillcolor=lightgreen, style=filled];
       }
       
       // Software layer
       subgraph cluster_software {
           label="Software Layer";
           style=filled;
           color=lightgrey;
           
           acs [label="Access Control\nSystem", fillcolor=lightcyan, style=filled];
           nfclib [label="NFC Reader\nInterface", fillcolor=lightcyan, style=filled];
           db [label="Card Database\n(EEPROM)", fillcolor=lightcyan, style=filled];
           menu [label="Menu System", fillcolor=lightcyan, style=filled];
       }
       
       // Output
       lock [label="Door Lock", shape=ellipse, fillcolor=orange, style=filled];
       
       // Connections
       user -> nfc [label="Card\nPresent"];
       nfc -> arduino [label="SPI + IRQ"];
       lcd -> arduino [label="Parallel"];
       buttons -> arduino [label="Analog"];
       arduino -> relay [label="Digital"];
       relay -> lock [label="Power"];
       
       arduino -> acs [style=dashed];
       acs -> nfclib [style=dashed];
       acs -> db [style=dashed];
       acs -> menu [style=dashed];
       nfclib -> nfc [style=dashed, label="Control"];
       acs -> lcd [style=dashed, label="Display"];
       menu -> buttons [style=dashed, label="Input"];
   }

Hardware Layer
^^^^^^^^^^^^^^

* **Arduino Nano**: The main microcontroller (ATmega328P) running the access control logic
* **PN532 NFC Module**: Handles NFC communication with cards at 13.56 MHz
* **LCD Display**: 16x2 character display providing visual feedback to users
* **Button Interface**: Four push buttons for menu navigation (Up, Down, Select, Back)
* **Relay Module**: Controls physical door locks or access gates

Software Layer
^^^^^^^^^^^^^^

* **Access Control System**: Main application logic coordinating all components
* **NFC Reader Interface**: Abstraction layer for PN532 communication (read/write operations)
* **Card Database**: EEPROM-based persistent storage for authorized cards
* **Menu System**: User interface for system management and card operations

Design Philosophy
-----------------

The system is designed with several key principles in mind:

Security First
^^^^^^^^^^^^^^

* All card verification happens locally without network dependency
* Authorized cards are stored securely in non-volatile EEPROM
* Physical relay control prevents unauthorized access
* Custom sector technology for advanced card management

User Friendly
^^^^^^^^^^^^^

* Clear LCD feedback for all operations
* Intuitive button-based navigation
* Visual status indicators
* Minimal learning curve for end users

Educational Value
^^^^^^^^^^^^^^^^^

* Progressive learning from simple read/write to complete system
* Well-documented code with extensive comments
* Practical examples demonstrating key concepts
* Foundation for further NFC exploration

Flexibility
^^^^^^^^^^^

* Support for multiple card types (Mifare Classic, NTAG, Ultralight)
* Store up to 40 authorized cards
* Advanced card cloning for access duplication
* Easy card management through menu system
* Configurable access duration and system behavior

Reliability
^^^^^^^^^^^

* IRQ-based card detection for fast response (<100ms)
* Non-blocking operation for smooth user experience
* Robust error handling and recovery
* Persistent storage survives power cycles

Use Cases
---------

The NFC Access Control System is suitable for:

Educational Applications
^^^^^^^^^^^^^^^^^^^^^^^^

* Learning NFC fundamentals (Tasks 1 & 2)
* Embedded systems course projects
* RFID/NFC technology demonstrations
* Arduino programming education

Practical Applications
^^^^^^^^^^^^^^^^^^^^^^

* Office and laboratory access control
* Residential building security
* Storage room and restricted area management
* Maker space equipment access
* Prototyping commercial access control solutions
* Time and attendance tracking

Technical Specifications
------------------------

Hardware Requirements
^^^^^^^^^^^^^^^^^^^^^

* **Microcontroller**: Arduino Nano or compatible (ATmega328P)
* **NFC Module**: PN532 (SPI or I2C mode)
* **Display**: HD44780-compatible 16x2 LCD
* **Input**: 4 push buttons (active LOW with pull-up)
* **Output**: 5V relay module
* **Power**: 5V DC, ~200mA typical

Software Capabilities
^^^^^^^^^^^^^^^^^^^^^

* **Supported Cards**: Mifare Classic 1K/4K, Mifare Ultralight, NTAG213/215/216
* **Card Capacity**: Up to 40 authorized cards in EEPROM
* **Detection Time**: < 100ms with IRQ mode, ~100-200ms polling mode
* **UID Support**: 4-byte and 7-byte UIDs
* **Communication**: SPI or I2C (PN532), 4-bit parallel (LCD)
* **Memory Usage**: ~15KB flash, ~1KB SRAM
* **EEPROM**: Circular buffer for card storage with wear leveling

Performance Metrics
^^^^^^^^^^^^^^^^^^^

* **Read Response**: < 100ms from card detection to access decision
* **Write Speed**: ~50-100ms per block/page including verification
* **Menu Response**: Immediate button feedback
* **Relay Activation**: Configurable (default 3 seconds)
* **Power Consumption**: 
  
  * Idle: ~50mA
  * Reading: ~150mA
  * Relay active: ~200mA

Getting Started
---------------

To begin working with this system:

1. **Start with basics**: Complete Task 1 (reading) and Task 2 (writing) using the examples in :doc:`examples`
2. **Set up hardware**: Follow the wiring diagrams in :doc:`hardware`
3. **Install software**: See :doc:`installation` for PlatformIO setup
4. **Build the full system**: Review :doc:`usage` for complete system operation
5. **Explore advanced features**: Learn about card cloning in :doc:`card_cloning`

Project Structure
-----------------

The project is organized as follows:

* ``examples/`` - Simple examples for Tasks 1 & 2 (read and write)
* ``src/`` - Main application code (full access control system)
* ``include/`` - Header files and class definitions
* ``lib/`` - External libraries
* ``docs/`` - This documentation
* ``test/`` - Hardware test utilities

Each component is modular and well-documented, making it easy to understand and extend.

.. graphviz::
   :caption: System Architecture Overview

   digraph system_architecture {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       // User layer
       user [label="User/Card", shape=ellipse, fillcolor=lightblue, style=filled];
       
       // Hardware layer
       subgraph cluster_hardware {
           label="Hardware Layer";
           style=filled;
           color=lightgrey;
           
           nfc [label="PN532 NFC\nReader", fillcolor=lightyellow, style=filled];
           lcd [label="16x2 LCD\nDisplay", fillcolor=lightyellow, style=filled];
           buttons [label="Navigation\nButtons (4)", fillcolor=lightyellow, style=filled];
           relay [label="Relay\nModule", fillcolor=lightyellow, style=filled];
           arduino [label="Arduino Nano\n(ATmega328P)", fillcolor=lightgreen, style=filled];
       }
       
       // Software layer
       subgraph cluster_software {
           label="Software Layer";
           style=filled;
           color=lightgrey;
           
           acs [label="Access Control\nSystem", fillcolor=lightcyan, style=filled];
           nfclib [label="NFC Reader\nInterface", fillcolor=lightcyan, style=filled];
           db [label="Card Database\n(EEPROM)", fillcolor=lightcyan, style=filled];
           menu [label="Menu System", fillcolor=lightcyan, style=filled];
       }
       
       // Output
       lock [label="Door Lock", shape=ellipse, fillcolor=orange, style=filled];
       
       // Connections
       user -> nfc [label="Card\nPresent"];
       nfc -> arduino [label="SPI + IRQ"];
       lcd -> arduino [label="Parallel"];
       buttons -> arduino [label="Analog"];
       arduino -> relay [label="Digital"];
       relay -> lock [label="Power"];
       
       arduino -> acs [style=dashed];
       acs -> nfclib [style=dashed];
       acs -> db [style=dashed];
       acs -> menu [style=dashed];
       nfclib -> nfc [style=dashed, label="Control"];
       acs -> lcd [style=dashed, label="Display"];
       menu -> buttons [style=dashed, label="Input"];
   }

Hardware Layer
^^^^^^^^^^^^^^

* **Arduino Nano**: The main microcontroller running the access control logic
* **PN532 NFC Module**: Handles NFC communication with cards
* **LCD Display**: Provides visual feedback to users
* **Button Interface**: Allows navigation through menu system
* **Relay Module**: Controls physical door locks

Software Layer
^^^^^^^^^^^^^^

* **Access Control System**: Main application logic coordinating all components
* **NFC Reader Interface**: Abstraction layer for PN532 communication (read/write operations)
* **Card Database**: EEPROM-based persistent storage for authorized cards
* **Menu System**: User interface for system management and card operations

Design Philosophy
-----------------

The system is designed with several key principles in mind:

Security First
^^^^^^^^^^^^^^

* All card verification happens locally without network dependency
* Authorized cards are stored securely in EEPROM
* Physical relay control prevents unauthorized access

User Friendly
^^^^^^^^^^^^^

* Clear LCD feedback for all operations
* Intuitive button-based navigation
* Visual status indicators
* Minimal learning curve for end users

Educational Value
^^^^^^^^^^^^^^^^^

* Progressive learning from simple read/write to complete system
* Well-documented code with extensive comments
* Practical examples demonstrating key concepts
* Foundation for further NFC exploration

Flexibility
^^^^^^^^^^^

* Support for multiple card types (Mifare Classic, NTAG, Ultralight)
* Store up to 40 authorized cards
* Advanced card cloning for access duplication
* Easy card management through menu system
* Configurable access duration and system behavior

Reliability
^^^^^^^^^^^

* IRQ-based card detection for fast response (<100ms)
* Non-blocking operation for smooth user experience
* Robust error handling and recovery
* Persistent storage survives power cycles

Use Cases
---------

The NFC Access Control System is suitable for:

Educational Applications
^^^^^^^^^^^^^^^^^^^^^^^^

* Learning NFC fundamentals (Tasks 1 & 2)
* Embedded systems course projects
* RFID/NFC technology demonstrations
* Arduino programming education

Practical Applications
^^^^^^^^^^^^^^^^^^^^^^

* Office and laboratory access control
* Residential building security
* Storage room and restricted area management
* Maker space equipment access
* Prototyping commercial access control solutions
* Time and attendance tracking

Technical Specifications
------------------------

Hardware Requirements
^^^^^^^^^^^^^^^^^^^^^

* **Microcontroller**: Arduino Nano or compatible (ATmega328P)
* **NFC Module**: PN532 (SPI or I2C mode)
* **Display**: HD44780-compatible 16x2 LCD
* **Input**: 4 push buttons (active LOW with pull-up)
* **Output**: 5V relay module
* **Power**: 5V DC, ~200mA typical

Software Capabilities
^^^^^^^^^^^^^^^^^^^^^

* **Supported Cards**: Mifare Classic 1K/4K, Mifare Ultralight, NTAG213/215/216
* **Card Capacity**: Up to 40 authorized cards in EEPROM
* **Detection Time**: < 100ms with IRQ mode, ~100-200ms polling mode
* **UID Support**: 4-byte and 7-byte UIDs
* **Communication**: SPI or I2C (PN532), 4-bit parallel (LCD)
* **Memory Usage**: ~15KB flash, ~1KB SRAM
* **EEPROM**: Circular buffer for card storage with wear leveling

Performance Metrics
^^^^^^^^^^^^^^^^^^^

* **Read Response**: < 100ms from card detection to access decision
* **Write Speed**: ~50-100ms per block/page including verification
* **Menu Response**: Immediate button feedback
* **Relay Activation**: Configurable (default 3 seconds)
* **Power Consumption**: 
  
  * Idle: ~50mA
  * Reading: ~150mA
  * Relay active: ~200mA

Getting Started
---------------

To begin working with this system:

1. **Start with basics**: Complete Task 1 (reading) and Task 2 (writing) using the examples in :doc:`examples`
2. **Set up hardware**: Follow the wiring diagrams in :doc:`hardware`
3. **Install software**: See :doc:`installation` for PlatformIO setup
4. **Build the full system**: Review :doc:`usage` for complete system operation
5. **Explore advanced features**: Learn about card cloning in :doc:`card_cloning`

Project Structure
-----------------

The project is organized as follows:

* ``examples/`` - Simple examples for Tasks 1 & 2 (read and write)
* ``src/`` - Main application code (full access control system)
* ``include/`` - Header files and class definitions
* ``lib/`` - External libraries
* ``docs/`` - This documentation
* ``test/`` - Hardware test utilities

Each component is modular and well-documented, making it easy to understand and extend.
