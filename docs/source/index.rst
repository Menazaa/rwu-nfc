.. NFC Access Control System documentation master file

NFC Access Control System Documentation
========================================

Welcome to the documentation for the NFC Access Control System, a professional
access control solution using the PN532 NFC reader with advanced card cloning
capabilities.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   introduction
   hardware
   installation
   usage
   api
   eeprom
   card_cloning
   troubleshooting
   quickstart
   data_read
   data_write

Overview
--------

The NFC Access Control System is a comprehensive solution for managing access
control using NFC technology. Built on Arduino Nano with the PN532 NFC module,
it provides features including:

* Multi-card support (up to 40 authorized cards)
* Advanced card cloning capabilities
* Real-time access control with relay output
* Interactive LCD menu system
* IRQ-based fast card detection

Key Features
------------

Core Functionality
^^^^^^^^^^^^^^^^^^

* **Multi-card Support**: Store up to 40 authorized cards in EEPROM
* **Smart Card Cloning**: Clone any Mifare Classic card using custom sector technology
* **Real-time Access Control**: Instant card verification with relay output
* **Interactive Menu System**: Full-featured LCD menu with button navigation
* **IRQ-based Detection**: Fast, non-blocking card detection using interrupts

Advanced Card Management
^^^^^^^^^^^^^^^^^^^^^^^^

* **Custom Sector Technology**: Uses Sector 1 (blocks 4-7) for cloned UID storage
* **Dual UID System**: Supports both physical and cloned UIDs
* **Automatic UID Selection**: Intelligently selects appropriate UID
* **No Magic Cards Required**: Clone to any standard Mifare Classic 1K/4K card

Quick Start
-----------

1. See :doc:`hardware` for component requirements and wiring diagrams
2. Follow :doc:`installation` to set up the development environment
3. Read :doc:`usage` for operating instructions
4. Explore :doc:`api` for technical details

Indices and tables
==================

* :ref:`genindex`
* :ref:`search`
