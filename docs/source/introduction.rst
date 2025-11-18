Introduction
============

The NFC Access Control System is a professional-grade access control solution
designed for educational and commercial applications. Built using Arduino
technology and the industry-standard PN532 NFC module, it provides a robust
and feature-rich platform for managing physical access security.

What is NFC?
------------

Near Field Communication (NFC) is a set of communication protocols that enable
two electronic devices to communicate over a distance of 4 cm or less. NFC is
commonly used for:

* Contactless payment systems
* Access control and security
* Data exchange between devices
* Smart posters and advertising

System Architecture
-------------------

The system consists of several key components:

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

* **Access Control System**: Main application logic
* **NFC Reader Interface**: Abstraction layer for PN532 communication
* **Card Database**: EEPROM-based storage for authorized cards
* **Menu System**: User interface for system management

Design Philosophy
-----------------

The system is designed with several key principles:

Security First
^^^^^^^^^^^^^^

* All card verification happens locally without network dependency
* Authorized cards are stored securely in EEPROM
* Physical relay control prevents unauthorized access

User Friendly
^^^^^^^^^^^^^

* Clear LCD feedback for all operations
* Intuitive button-based navigation
* Visual and audio indicators for access status

Flexibility
^^^^^^^^^^^

* Support for up to 40 authorized cards
* Advanced card cloning for access duplication
* Easy card management through menu system

Reliability
^^^^^^^^^^^

* IRQ-based card detection for fast response
* Non-blocking operation for smooth user experience
* Robust error handling and recovery

Use Cases
---------

The NFC Access Control System is suitable for:

* Office and laboratory access control
* Residential building security
* Storage and restricted area management
* Educational demonstrations of NFC technology
* Prototyping commercial access control solutions

Technical Specifications
------------------------

* **Supported Cards**: Mifare Classic 1K/4K, Mifare Ultralight, NTAG
* **Card Capacity**: Up to 40 authorized cards
* **Detection Time**: < 100ms with IRQ mode
* **UID Length**: 4 or 7 bytes
* **Operating Voltage**: 5V DC
* **Power Consumption**: ~200mA typical
* **Communication**: SPI (PN532), 4-bit parallel (LCD)
