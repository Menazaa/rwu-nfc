EEPROM Memory Layout
====================

This section describes how the system stores authorized cards in the Arduino's EEPROM memory.

Memory Map Overview
-------------------

.. graphviz::
   :caption: EEPROM Memory Organization (1024 bytes total)

   digraph eeprom_layout {
       rankdir=TB;
       node [shape=record];
       
       eeprom [label="{EEPROM Memory Layout|{<magic>Address 0-1\nMagic Number\n0xABCD|<count>Address 2\nCard Count\n(0-40)|<card0>Address 3-10\nCard 1\n(8 bytes)|<card1>Address 11-18\nCard 2\n(8 bytes)|<card2>Address 19-26\nCard 3\n(8 bytes)|<dots>...|<card39>Address 315-322\nCard 40\n(8 bytes)|<free>Address 323-1023\nFree Space\n(701 bytes)}}", fillcolor=lightblue, style=filled];
   }

The EEPROM uses a simple flat structure optimized for fast sequential access and minimal wear.

Memory Regions
--------------

Header Section
^^^^^^^^^^^^^^

**Addresses 0-2** (3 bytes)

.. graphviz::

   digraph header {
       rankdir=LR;
       node [shape=record];
       
       header [label="<addr0>Addr 0\nMagic\nHigh Byte\n(0xAB)|<addr1>Addr 1\nMagic\nLow Byte\n(0xCD)|<addr2>Addr 2\nCard\nCount\n(0-40)", fillcolor=yellow, style=filled];
   }

* **Magic Number** (0xABCD): Validates EEPROM has been initialized
* **Card Count**: Number of stored cards (0-40)

Card Data Section
^^^^^^^^^^^^^^^^^

**Addresses 3-322** (320 bytes = 40 cards × 8 bytes)

Each card entry is 8 bytes:

.. graphviz::

   digraph card_entry {
       rankdir=LR;
       node [shape=record];
       
       entry [label="<b0>Byte 0\nUID Length\n(4 or 7)|<b1>Byte 1\nUID[0]|<b2>Byte 2\nUID[1]|<b3>Byte 3\nUID[2]|<b4>Byte 4\nUID[3]|<b5>Byte 5\nUID[4]\nor 0xFF|<b6>Byte 6\nUID[5]\nor 0xFF|<b7>Byte 7\nUID[6]\nor 0xFF", fillcolor=lightgreen, style=filled];
   }

Example 4-byte UID entry::

   04 AB 12 CD 34 FF FF FF

Example 7-byte UID entry::

   07 04 AB 12 34 56 CD EF

Memory Operations
-----------------

Read Operation Flow
^^^^^^^^^^^^^^^^^^^

.. graphviz::
   :caption: Card Authorization Check Process

   digraph read_flow {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       start [label="Card Scanned", shape=ellipse, fillcolor=lightgreen, style=filled];
       read_magic [label="Read Magic Number\n(Addr 0-1)", fillcolor=lightblue, style=filled];
       check_magic [label="Magic == 0xABCD?", shape=diamond, fillcolor=yellow, style=filled];
       read_count [label="Read Card Count\n(Addr 2)", fillcolor=lightblue, style=filled];
       loop_start [label="For i = 0 to Count-1", fillcolor=orange, style=filled];
       read_entry [label="Read Card Entry\n(Addr 3 + i*8)", fillcolor=lightblue, style=filled];
       compare [label="UID Match?", shape=diamond, fillcolor=yellow, style=filled];
       granted [label="Access Granted", shape=ellipse, fillcolor=green, style=filled, fontcolor=white];
       denied [label="Access Denied", shape=ellipse, fillcolor=red, style=filled, fontcolor=white];
       
       start -> read_magic;
       read_magic -> check_magic;
       check_magic -> read_count [label="Yes"];
       check_magic -> denied [label="No\n(Not Init)"];
       read_count -> loop_start;
       loop_start -> read_entry;
       read_entry -> compare;
       compare -> granted [label="Yes"];
       compare -> loop_start [label="No\nNext Card"];
       loop_start -> denied [label="All Checked\nNo Match"];
   }

Write Operation Flow
^^^^^^^^^^^^^^^^^^^^

.. graphviz::
   :caption: Card Registration Process

   digraph write_flow {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       start [label="Register Card", shape=ellipse, fillcolor=lightgreen, style=filled];
       read_count [label="Read Current Count", fillcolor=lightblue, style=filled];
       check_full [label="Count < 40?", shape=diamond, fillcolor=yellow, style=filled];
       check_exists [label="Card Already\nRegistered?", shape=diamond, fillcolor=yellow, style=filled];
       calc_addr [label="Calculate Address\n3 + Count * 8", fillcolor=orange, style=filled];
       write_uid [label="Write UID Entry\n(8 bytes)", fillcolor=pink, style=filled];
       inc_count [label="Increment Count\nWrite to Addr 2", fillcolor=pink, style=filled];
       success [label="Registration\nSuccessful", shape=ellipse, fillcolor=green, style=filled, fontcolor=white];
       fail_full [label="Storage Full", shape=ellipse, fillcolor=red, style=filled, fontcolor=white];
       fail_exists [label="Already Exists", shape=ellipse, fillcolor=red, style=filled, fontcolor=white];
       
       start -> read_count;
       read_count -> check_full;
       check_full -> check_exists [label="Yes"];
       check_full -> fail_full [label="No"];
       check_exists -> calc_addr [label="No"];
       check_exists -> fail_exists [label="Yes"];
       calc_addr -> write_uid;
       write_uid -> inc_count;
       inc_count -> success;
   }

Memory Wear Leveling
--------------------

The Arduino Nano's EEPROM (ATmega328P) has a rated endurance of 100,000 write cycles per cell.

Write Frequency Estimation
^^^^^^^^^^^^^^^^^^^^^^^^^^

.. graphviz::

   digraph wear_analysis {
       rankdir=LR;
       node [shape=box, style=rounded];
       
       magic [label="Magic Number\n(Addr 0-1)\nWritten: Once", fillcolor=green, style=filled];
       count [label="Card Count\n(Addr 2)\nWritten: Each add/delete", fillcolor=yellow, style=filled];
       cards [label="Card Entries\n(Addr 3-322)\nWritten: Once per card", fillcolor=green, style=filled];
       
       magic -> count [style=invis];
       count -> cards [style=invis];
   }

**Most-Written Locations**:

* **Address 2** (Card Count): Modified on every card add/delete operation
  
  - Assume 10 cards added/deleted per day
  - 100,000 writes / 10 writes per day = ~27 years lifespan

**Least-Written Locations**:

* Card entry addresses: Written only when card is registered
* Deleted cards are marked invalid, not erased (reduces writes)

Best Practices
^^^^^^^^^^^^^^

To maximize EEPROM lifespan:

1. **Minimize Deletions**: Prefer keeping cards registered when possible
2. **Batch Operations**: Register multiple cards in one session
3. **Avoid Frequent Resets**: Don't clear EEPROM unnecessarily
4. **Monitor Usage**: Track write cycles in critical applications

Memory Initialization
---------------------

First-Time Setup
^^^^^^^^^^^^^^^^

.. graphviz::

   digraph init_flow {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       boot [label="System Boot", shape=ellipse, fillcolor=lightgreen, style=filled];
       read_magic [label="Read Magic Number", fillcolor=lightblue, style=filled];
       check [label="Magic Valid?", shape=diamond, fillcolor=yellow, style=filled];
       normal [label="Normal Operation", fillcolor=green, style=filled];
       init [label="Initialize EEPROM", fillcolor=orange, style=filled];
       write_magic [label="Write Magic 0xABCD", fillcolor=pink, style=filled];
       zero_count [label="Set Count to 0", fillcolor=pink, style=filled];
       clear_cards [label="Clear Card Area\n(Fill with 0xFF)", fillcolor=pink, style=filled];
       
       boot -> read_magic;
       read_magic -> check;
       check -> normal [label="Yes"];
       check -> init [label="No"];
       init -> write_magic;
       write_magic -> zero_count;
       zero_count -> clear_cards;
       clear_cards -> normal;
   }

Code Example
^^^^^^^^^^^^

Initialization code in ``AccessControlSystem.cpp``:

.. code-block:: cpp

   void AccessControlSystem::initEEPROM() {
       // Check magic number
       uint16_t magic = (EEPROM.read(0) << 8) | EEPROM.read(1);
       
       if (magic != 0xABCD) {
           // First time - initialize
           EEPROM.write(0, 0xAB);
           EEPROM.write(1, 0xCD);
           EEPROM.write(2, 0);  // Zero cards
           
           // Clear card storage area
           for (int i = 3; i < 323; i++) {
               EEPROM.write(i, 0xFF);
           }
       }
   }

Memory Debugging
----------------

To inspect EEPROM contents, add debug code:

.. code-block:: cpp

   void dumpEEPROM() {
       Serial.println("=== EEPROM Dump ===");
       
       // Header
       Serial.print("Magic: 0x");
       Serial.print(EEPROM.read(0), HEX);
       Serial.println(EEPROM.read(1), HEX);
       Serial.print("Count: ");
       Serial.println(EEPROM.read(2));
       
       // Cards
       uint8_t count = EEPROM.read(2);
       for (uint8_t i = 0; i < count; i++) {
           int addr = 3 + (i * 8);
           Serial.print("Card ");
           Serial.print(i + 1);
           Serial.print(": ");
           
           uint8_t len = EEPROM.read(addr);
           Serial.print("Len=");
           Serial.print(len);
           Serial.print(" UID=");
           
           for (uint8_t j = 0; j < 7; j++) {
               Serial.print(EEPROM.read(addr + 1 + j), HEX);
               Serial.print(" ");
           }
           Serial.println();
       }
   }

Summary
-------

The EEPROM storage system provides:

* **Simple Structure**: Easy to understand and debug
* **Efficient Access**: Direct addressing for fast lookups
* **Durable**: Designed for long-term reliability
* **Scalable**: Supports up to 40 cards with room for expansion
* **Robust**: Magic number validation prevents corruption

Total memory usage: **323 bytes of 1024 bytes (31.5%)**
