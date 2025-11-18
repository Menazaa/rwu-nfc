Card Cloning Technology
========================

This section provides detailed technical information about the advanced card
cloning capabilities of the NFC Access Control System.

Overview
--------

The system implements a sophisticated card cloning mechanism that allows
copying NFC card credentials without requiring special "magic" or "UID
changeable" cards. This is achieved through custom sector technology that
stores cloned UIDs in a writable sector of standard Mifare Classic cards.

How It Works
------------

Dual UID System
^^^^^^^^^^^^^^^

Every Mifare Classic card has two potential UIDs:

1. **Physical UID** (Block 0, Sector 0)
   
   * Manufacturer-assigned unique identifier
   * Stored in Block 0 (read-only on standard cards)
   * Cannot be changed on regular cards
   * 4 or 7 bytes in length

2. **Cloned UID** (Block 4, Sector 1)
   
   * Custom UID stored in writable sector
   * Stored in our custom sector (Sector 1)
   * Can be written to any standard Mifare Classic card
   * Emulates a different card's identity

Reading Priority
^^^^^^^^^^^^^^^^

When scanning a card, the system:

1. First attempts to read the cloned UID from Sector 1
2. If a cloned UID exists and is valid, it uses that
3. If no cloned UID exists, it falls back to the physical UID
4. This allows cloned cards to impersonate their source cards

Custom Sector Structure
------------------------

Sector 1 Layout
^^^^^^^^^^^^^^^

The system uses Sector 1 (blocks 4-7) for cloning data:

.. graphviz::
   :caption: Mifare Classic Memory Structure with Custom Sector

   digraph memory_structure {
       rankdir=TB;
       node [shape=record];
       
       subgraph cluster_sector0 {
           label="Sector 0 (Manufacturer)";
           style=filled;
           color=lightcoral;
           
           block0 [label="{Block 0|Physical UID\n(Read-Only)|Manufacturer Data}", fillcolor=pink, style=filled];
           block1 [label="{Block 1|Data}", fillcolor=white, style=filled];
           block2 [label="{Block 2|Data}", fillcolor=white, style=filled];
           block3 [label="{Block 3|Sector Trailer\n(Keys \& Access)}", fillcolor=lightgray, style=filled];
       }
       
       subgraph cluster_sector1 {
           label="Sector 1 (Custom - Used for Cloning)";
           style=filled;
           color=lightgreen;
           
           block4 [label="{Block 4|Cloned UID Storage\nByte 0: Length (4/7)\nBytes 1-7: UID Data}", fillcolor=yellow, style=filled];
           block5 [label="{Block 5|Metadata\nTimestamp, Flags\nChecksum}", fillcolor=lightyellow, style=filled];
           block6 [label="{Block 6|Reserved\n(Future Use)}", fillcolor=white, style=filled];
           block7 [label="{Block 7|Sector Trailer\n(Keys \& Access)}", fillcolor=lightgray, style=filled];
       }
       
       subgraph cluster_other {
           label="Sectors 2-15 (Standard Data Storage)";
           style=filled;
           color=lightblue;
           
           others [label="Blocks 8-63\nAvailable for\nOther Applications", fillcolor=lightcyan, style=filled];
       }
       
       block0 -> block1 -> block2 -> block3 [style=invis];
       block3 -> block4 [label="Sector Boundary", style=dashed];
       block4 -> block5 -> block6 -> block7 [style=invis];
       block7 -> others [label="More Sectors...", style=dashed];
   }

.. list-table::
   :header-rows: 1
   :widths: 20 80

   * - Block
     - Purpose
   * - Block 4
     - Cloned UID storage (7 bytes) + length marker (1 byte)
   * - Block 5
     - Metadata (timestamp, flags, checksum)
   * - Block 6
     - Reserved for future use
   * - Block 7
     - Sector trailer (access bits and keys)

Block 4 Structure (UID Block)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: text

   Byte 0: UID Length (0x04 or 0x07)
   Bytes 1-7: Cloned UID data (padded with 0xFF if < 7 bytes)
   Bytes 8-15: Reserved (0xFF)

Example for 4-byte UID::

   04 AB 12 CD 34 FF FF FF FF FF FF FF FF FF FF FF

Example for 7-byte UID::

   07 04 AB 12 34 56 CD EF FF FF FF FF FF FF FF FF

Block 5 Structure (Metadata Block)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: text

   Bytes 0-3: Clone timestamp (Unix time, optional)
   Byte 4: Clone flags (validity, version)
   Byte 5: Checksum of UID block
   Bytes 6-15: Reserved

Cloning Process
---------------

Step-by-Step Procedure
^^^^^^^^^^^^^^^^^^^^^^^

.. graphviz::
   :caption: Card Cloning Process Flow

   digraph cloning_process {
       rankdir=TB;
       node [shape=box, style=rounded];
       
       start [label="Start Cloning", shape=ellipse, fillcolor=lightgreen, style=filled];
       read_source [label="1. Read Source Card\nGet Physical UID", fillcolor=lightblue, style=filled];
       store_source [label="Store Source UID\nin Memory", fillcolor=cyan, style=filled];
       read_target [label="2. Read Target Card\nGet Physical UID", fillcolor=lightblue, style=filled];
       verify_target [label="3. Verify Target is\nMifare Classic", fillcolor=yellow, style=filled];
       auth [label="4. Authenticate\nto Sector 1\n(Default Key A)", fillcolor=orange, style=filled];
       write_uid [label="5. Write Source UID\nto Block 4", fillcolor=pink, style=filled];
       write_meta [label="6. Write Metadata\nto Block 5", fillcolor=pink, style=filled];
       verify [label="7. Verify Write\nRead Back Block 4", fillcolor=violet, style=filled];
       success [label="Clone Successful", shape=ellipse, fillcolor=green, style=filled, fontcolor=white];
       fail [label="Clone Failed", shape=ellipse, fillcolor=red, style=filled, fontcolor=white];
       
       start -> read_source;
       read_source -> store_source;
       store_source -> read_target;
       read_target -> verify_target;
       verify_target -> auth [label="Valid"];
       verify_target -> fail [label="Not Mifare\nClassic"];
       auth -> write_uid [label="Success"];
       auth -> fail [label="Auth Failed"];
       write_uid -> write_meta [label="Success"];
       write_uid -> fail [label="Write Failed"];
       write_meta -> verify;
       verify -> success [label="Match"];
       verify -> fail [label="Mismatch"];
   }

1. **Read Source Card**
   
   * User presents the source card to be cloned
   * System reads the physical UID
   * UID is stored in memory temporarily

2. **Read Target Card**
   
   * User presents the target card (destination)
   * System reads the target's physical UID
   * Verifies it's a Mifare Classic card

3. **Authenticate to Sector 1**
   
   * System authenticates using default Key A (0xFFFFFFFFFFFF)
   * If authentication fails with default key, cloning cannot proceed
   * Most new cards use default keys

4. **Write Cloned UID**
   
   * Writes source UID to Block 4 of target card
   * Writes metadata to Block 5
   * Preserves original sector trailer (Block 7)

5. **Verify Write**
   
   * Reads back Block 4 to verify successful write
   * Compares with intended UID
   * Reports success or failure

6. **Result**
   
   * Target card now has both its physical UID and the cloned UID
   * When scanned, system will read cloned UID first
   * Card behaves as if it has the source card's identity

Code Implementation
-------------------

NFCReader::cloneCard() Method
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   bool NFCReader::cloneCard(uint8_t* sourceUID, uint8_t sourceLength,
                             uint8_t* targetUID, uint8_t targetLength) {
       // 1. Prepare data block
       uint8_t blockData[16];
       memset(blockData, 0xFF, 16);
       blockData[0] = sourceLength;
       memcpy(&blockData[1], sourceUID, sourceLength);
       
       // 2. Authenticate to custom sector
       uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
       if (!pn532.mifareclassic_AuthenticateBlock(targetUID, targetLength,
                                                    CUSTOM_BLOCK_UID, 0, key)) {
           return false;
       }
       
       // 3. Write UID to block
       if (!pn532.mifareclassic_WriteDataBlock(CUSTOM_BLOCK_UID, blockData)) {
           return false;
       }
       
       // 4. Write metadata block
       uint8_t metaData[16];
       memset(metaData, 0x00, 16);
       metaData[4] = 0x01;  // Valid flag
       if (!pn532.mifareclassic_WriteDataBlock(CUSTOM_BLOCK_META, metaData)) {
           return false;
       }
       
       // 5. Verify
       uint8_t verifyData[16];
       if (!pn532.mifareclassic_ReadDataBlock(CUSTOM_BLOCK_UID, verifyData)) {
           return false;
       }
       
       return (memcmp(blockData, verifyData, 16) == 0);
   }

Reading Cloned UID
^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

   bool NFCReader::readClonedUID(uint8_t* uid, uint8_t* uidLength) {
       uint8_t blockData[16];
       uint8_t key[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
       
       // Authenticate to custom sector
       if (!pn532.mifareclassic_AuthenticateBlock(currentUID, currentUIDLength,
                                                    CUSTOM_BLOCK_UID, 0, key)) {
           return false;
       }
       
       // Read UID block
       if (!pn532.mifareclassic_ReadDataBlock(CUSTOM_BLOCK_UID, blockData)) {
           return false;
       }
       
       // Check if valid cloned UID exists
       if (blockData[0] != 0x04 && blockData[0] != 0x07) {
           return false;  // No valid cloned UID
       }
       
       // Extract UID
       *uidLength = blockData[0];
       memcpy(uid, &blockData[1], *uidLength);
       
       return true;
   }

Security Considerations
-----------------------

Authentication Keys
^^^^^^^^^^^^^^^^^^^

The cloning system relies on default Mifare Classic keys:

* **Key A**: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
* **Key B**: 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF

**Security Implications**:

* Cards using default keys can be cloned by anyone with this knowledge
* For production systems, change sector keys after cloning
* Consider using more secure NFC technologies (NTAG, DESFire)

Access Control
^^^^^^^^^^^^^^

**Important Notes**:

* Cloning should only be used for authorized purposes
* Original card owners should be notified
* Maintain logs of all cloning operations
* Consider legal and ethical implications

Physical Security
^^^^^^^^^^^^^^^^^

* Protect cards from unauthorized cloning
* Use tamper-evident holders for critical access cards
* Implement time-limited access credentials
* Monitor for duplicate card usage

Limitations
-----------

Card Compatibility
^^^^^^^^^^^^^^^^^^

The cloning feature works only with:

* **Mifare Classic 1K**: Full support
* **Mifare Classic 4K**: Full support (uses Sector 1)
* **Mifare Ultralight**: Not supported (no sector structure)
* **NTAG**: Not supported (different memory structure)
* **DESFire**: Not supported (encrypted memory)

Key Requirements
^^^^^^^^^^^^^^^^

* Target card must use default keys for Sector 1
* If sector keys have been changed, cloning will fail
* No method to recover custom keys without card manufacturer access

Write Limitations
^^^^^^^^^^^^^^^^^

* Standard cards can be written only ~100,000 times
* Excessive cloning operations may degrade card memory
* Always verify writes to ensure data integrity

Advantages Over Magic Cards
----------------------------

Traditional "Magic" Cards
^^^^^^^^^^^^^^^^^^^^^^^^^^

Traditional UID-changeable cards (Chinese Magic Cards):

* Expensive and harder to source
* Block 0 is writable, which is abnormal
* Easily detected by security systems
* Limited availability and reliability

Our Custom Sector Approach
^^^^^^^^^^^^^^^^^^^^^^^^^^^

* Works with any standard Mifare Classic card
* Cards are readily available and inexpensive
* Maintains normal Block 0 (appears as legitimate card)
* More flexible - can store multiple identities
* Reversible - can erase cloned UID and reuse card

Best Practices
--------------

Cloning Workflow
^^^^^^^^^^^^^^^^

1. **Verify Source Card**: Ensure source card is valid and readable
2. **Use New Target Cards**: Fresh cards more reliable than reused ones
3. **Test After Cloning**: Always verify cloned card works correctly
4. **Document Clones**: Maintain registry of cloned cards and their sources
5. **Limit Scope**: Clone only when necessary, prefer registering physical UIDs

Error Handling
^^^^^^^^^^^^^^

Common errors and solutions:

* **Authentication Failed**: Target card may use custom keys
* **Write Failed**: Card may be damaged or write-protected
* **Verify Failed**: Retry cloning operation
* **Invalid UID**: Source UID format may be unsupported

Storage Management
^^^^^^^^^^^^^^^^^^

* Mark cloned cards visually (sticker, marking)
* Store clones separately from originals
* Implement clone expiration dates in metadata
* Periodic verification of cloned card functionality

Advanced Topics
---------------

Multi-Identity Cards
^^^^^^^^^^^^^^^^^^^^

Future enhancement: Store multiple cloned UIDs in different sectors:

* Sector 1: Primary cloned identity
* Sector 2: Secondary identity
* Sector 3: Tertiary identity
* Switch between identities via menu option

Encrypted Clones
^^^^^^^^^^^^^^^^

Potential security enhancement:

* Encrypt cloned UID with card-specific key
* Decrypt during read operation
* Prevents simple sector dumps from revealing UID
* Requires custom key management

Time-Limited Clones
^^^^^^^^^^^^^^^^^^^

Use metadata timestamp:

* Store clone creation time
* Check expiration during read
* Automatically invalidate old clones
* Useful for temporary access credentials

Example Use Cases
-----------------

Backup Access Card
^^^^^^^^^^^^^^^^^^

Clone an office access card to create a backup:

1. Clone office card to blank card
2. Store backup securely at home
3. Use if primary card is lost
4. Invalidate backup when primary is recovered

Temporary Access
^^^^^^^^^^^^^^^^

Grant temporary access without issuing new cards:

1. Clone authorized person's card
2. Give clone to temporary worker/visitor
3. Collect clone when access expires
4. Erase cloned UID for reuse

Multi-Location Access
^^^^^^^^^^^^^^^^^^^^^

Single card for multiple facilities:

1. Clone cards from different locations
2. Store in different sectors
3. Use appropriate identity per location
4. Maintain audit trail of usage

Testing and Validation
-----------------------

After cloning, verify:

* Cloned card grants access correctly
* Original card still functions
* Both cards recognized by system
* Access logs show correct attribution

Troubleshooting Cloning Issues
-------------------------------

See :doc:`troubleshooting` for detailed cloning error resolution.
