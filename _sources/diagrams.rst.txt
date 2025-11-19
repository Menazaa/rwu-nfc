Class Diagrams
==============

This section provides visual diagrams of the NFC Access Control System architecture using both Graphviz and PlantUML.

System Architecture Overview
----------------------------

.. graphviz::
   :caption: System Component Diagram

   digraph system_overview {
       rankdir=TB;
       node [shape=box, style=filled, fillcolor=lightblue];
       
       main [label="main.cpp"];
       acs [label="AccessControlSystem"];
       nfc [label="NFCReader"];
       pn532 [label="Adafruit_PN532"];
       lcd [label="LiquidCrystal"];
       eeprom [label="EEPROM"];
       
       main -> acs [label="creates"];
       main -> nfc [label="creates"];
       acs -> nfc [label="uses"];
       acs -> lcd [label="controls"];
       acs -> eeprom [label="stores data"];
       nfc -> pn532 [label="uses"];
   }

AccessControlSystem Class
--------------------------

.. uml::
   :caption: AccessControlSystem Class Diagram

   @startuml
   scale max 1024 width
   skinparam classAttributeIconSize 0
   
   enum SystemState {
       IDLE
       ACCESS_GRANTED
       ACCESS_DENIED
       MENU
       REGISTERING
       DELETING
       LISTING_CARDS
       CLONING_SOURCE
       CLONING_TARGET
   }
   
   enum MenuItem {
       REGISTER_CARD
       DELETE_CARD
       LIST_CARDS
       CLONE_CARD
       SETTINGS
       CLEAR_ALL
       EXIT_MENU
       MENU_COUNT
   }
   
   class StoredCard {
       + uint8_t uid[MAX_UID_LENGTH]
       + uint8_t uidLength
       + bool active
   }
   
   class AccessControlSystem {
       - NFCReader& reader
       - LiquidCrystal lcd
       - SystemState currentState
       - MenuItem currentMenuItem
       - unsigned long lastStateChange
       - uint8_t cloneSourceUID[7]
       - uint8_t cloneSourceLength
       - int totalCardsStored
       - int currentListIndex
       
       + AccessControlSystem(NFCReader& nfcReader)
       + void begin()
       + void update()
       + void handleButtons()
       
       - void handleIdleState()
       - void handleMenuState()
       - void handleRegisteringState()
       - void handleDeletingState()
       - void handleListingCardsState()
       - void handleCloningSourceState()
       - void handleCloningTargetState()
       
       - bool isCardAuthorized(uint8_t* uid, uint8_t uidLength)
       - bool registerCard(uint8_t* uid, uint8_t uidLength)
       - bool deleteCard(uint8_t* uid, uint8_t uidLength)
       - int findCard(uint8_t* uid, uint8_t uidLength)
       - void grantAccess()
       - void denyAccess()
       - void displayMenu()
       - void executeMenuItem()
       
       - void initializeEEPROM()
       - void loadCardsFromEEPROM()
       - void saveCardToEEPROM(int index)
       - void clearAllCards()
       - int countStoredCards()
   }
   
   AccessControlSystem --> SystemState
   AccessControlSystem --> MenuItem
   AccessControlSystem --> StoredCard
   AccessControlSystem ..> NFCReader : uses
   
   @enduml

NFCReader Class
---------------

.. uml::
   :caption: NFCReader Class Diagram

   @startuml
   scale max 1024 width
   skinparam classAttributeIconSize 0
   
   enum NFCCommMode {
       I2C
       SPI
   }
   
   enum NFCReadMode {
       POLLING
       IRQ
   }
   
   enum NFCCardType {
       UNKNOWN
       MIFARE_CLASSIC_1K
       MIFARE_CLASSIC_4K
       MIFARE_ULTRALIGHT
       NTAG
   }
   
   class NFCCardInfo {
       + bool detected
       + uint8_t uid[7]
       + uint8_t uidLength
       + NFCCardType cardType
       + uint32_t cardID
       + bool hasClonedUID
       + uint8_t clonedUID[7]
       + uint8_t clonedUIDLength
       + uint32_t timestamp
   }
   
   class NFCReader {
       - Adafruit_PN532* pn532
       - NFCCommMode commMode
       - NFCReadMode readMode
       - int irqPin
       - int resetPin
       - bool initialized
       - NFCCardInfo lastCard
       - uint8_t keyA[6]
       - uint8_t keyB[6]
       
       + NFCReader(NFCCommMode mode, NFCReadMode readMode)
       + NFCReader(int irq, int reset)
       + ~NFCReader()
       
       + bool begin()
       + NFCCardInfo readCard()
       + NFCCardInfo getLastCardInfo()
       + bool isCardPresent()
       
       + bool authenticateBlock(uint8_t blockNumber, uint8_t* key)
       + bool readBlock(uint8_t blockNumber, uint8_t* data)
       + bool writeBlock(uint8_t blockNumber, uint8_t* data)
       + bool formatSector(uint8_t sectorNumber)
       
       + bool initializeCardForCloning(uint8_t* uid, uint8_t uidLength)
       + bool writeClonedUID(uint8_t* uid, uint8_t uidLength)
       + bool readClonedUID(uint8_t* uid, uint8_t* uidLength)
       + bool hasClonedData()
       + bool clearClonedData()
       
       + bool waitForCardRemoval(unsigned long timeout)
       + void printCardInfo(NFCCardInfo& card)
       + String getCardTypeString(NFCCardType type)
       
       - NFCCardType identifyCardType(uint8_t* atqa, uint8_t sak)
       - void setupKeys()
   }
   
   NFCReader --> NFCCommMode
   NFCReader --> NFCReadMode
   NFCReader --> NFCCardType
   NFCReader --> NFCCardInfo
   NFCReader ..> "Adafruit_PN532" : uses
   
   @enduml

Complete System Relationships
------------------------------

.. graphviz::
   :caption: Complete Class Relationships

   digraph class_diagram {
       rankdir=BT;
       node [shape=record, style=filled];
       
       // Classes
       AccessControlSystem [
           fillcolor=lightblue,
           label="{AccessControlSystem|+ begin()\l+ update()\l+ handleButtons()\l|
           - handleIdleState()\l- handleMenuState()\l- registerCard()\l
           - deleteCard()\l- grantAccess()\l- denyAccess()\l}"
       ];
       
       NFCReader [
           fillcolor=lightgreen,
           label="{NFCReader|+ begin()\l+ readCard()\l+ isCardPresent()\l|
           + authenticateBlock()\l+ readBlock()\l+ writeBlock()\l
           + initializeCardForCloning()\l+ writeClonedUID()\l}"
       ];
       
       NFCCardInfo [
           fillcolor=lightyellow,
           label="{NFCCardInfo|+ uid[7]\l+ uidLength\l+ cardType\l+ hasClonedUID\l+ clonedUID[7]\l}"
       ];
       
       StoredCard [
           fillcolor=lightyellow,
           label="{StoredCard|+ uid[]\l+ uidLength\l+ active\l}"
       ];
       
       SystemState [
           fillcolor=lightcoral,
           shape=ellipse,
           label="SystemState\n(enum)"
       ];
       
       MenuItem [
           fillcolor=lightcoral,
           shape=ellipse,
           label="MenuItem\n(enum)"
       ];
       
       NFCCardType [
           fillcolor=lightcoral,
           shape=ellipse,
           label="NFCCardType\n(enum)"
       ];
       
       // Relationships
       AccessControlSystem -> NFCReader [label="uses", style=dashed];
       AccessControlSystem -> SystemState [label="has"];
       AccessControlSystem -> MenuItem [label="has"];
       AccessControlSystem -> StoredCard [label="manages", style=dashed];
       NFCReader -> NFCCardInfo [label="returns"];
       NFCReader -> NFCCardType [label="uses"];
       NFCCardInfo -> NFCCardType [label="contains"];
   }

State Machine
-------------

.. uml::
   :caption: System State Machine

   @startuml
   scale max 800 width
   skinparam state {
     BackgroundColor LightBlue
     BorderColor Black
   }
   
   [*] --> IDLE
   
   IDLE : Waiting for card or button press
   ACCESS_GRANTED : Display success message
   ACCESS_DENIED : Display error message
   MENU : Navigate menu options
   REGISTERING : Register new card
   DELETING : Delete existing card
   LISTING_CARDS : Display stored cards
   CLONING_SOURCE : Read source card to clone
   CLONING_TARGET : Write to target card
   
   IDLE --> ACCESS_GRANTED : Authorized card detected
   IDLE --> ACCESS_DENIED : Unauthorized card detected
   IDLE --> MENU : Menu button pressed
   
   ACCESS_GRANTED --> IDLE : Timeout (3s)
   ACCESS_DENIED --> IDLE : Timeout (3s)
   
   MENU --> REGISTERING : Select REGISTER_CARD
   MENU --> DELETING : Select DELETE_CARD
   MENU --> LISTING_CARDS : Select LIST_CARDS
   MENU --> CLONING_SOURCE : Select CLONE_CARD
   MENU --> IDLE : Select EXIT_MENU
   
   REGISTERING --> IDLE : Card registered or cancelled
   DELETING --> IDLE : Card deleted or cancelled
   LISTING_CARDS --> IDLE : Done viewing or cancelled
   
   CLONING_SOURCE --> CLONING_TARGET : Source card read
   CLONING_SOURCE --> IDLE : Cancelled
   CLONING_TARGET --> IDLE : Clone complete or cancelled
   
   @enduml

Card Registration Flow
-----------------------

.. uml::
   :caption: Card Registration Sequence

   @startuml
   scale max 1024 width
   skinparam sequenceMessageAlign center
   
   actor User
   participant "AccessControlSystem" as ACS
   participant "NFCReader" as Reader
   participant "PN532" as Hardware
   participant "EEPROM" as Storage
   
   User -> ACS: Press menu button
   activate ACS
   ACS -> ACS: Enter MENU state
   
   User -> ACS: Navigate to REGISTER_CARD
   ACS -> ACS: Enter REGISTERING state
   ACS -> ACS: Display "Place card"
   
   User -> Reader: Present NFC card
   activate Reader
   Reader -> Hardware: Read card UID
   activate Hardware
   Hardware --> Reader: Return UID
   deactivate Hardware
   Reader --> ACS: Return NFCCardInfo
   deactivate Reader
   
   ACS -> ACS: Check if card exists
   alt Card not already registered
       ACS -> Storage: Save card to EEPROM
       activate Storage
       Storage --> ACS: Confirm save
       deactivate Storage
       ACS -> ACS: Display "Card registered"
   else Card already exists
       ACS -> ACS: Display "Card exists"
   end
   
   ACS -> ACS: Return to IDLE state
   deactivate ACS
   
   @enduml

Card Cloning Flow
------------------

.. uml::
   :caption: Card Cloning Sequence

   @startuml
   scale max 1024 width
   skinparam sequenceMessageAlign center
   
   actor User
   participant "AccessControlSystem" as ACS
   participant "NFCReader" as Reader
   participant "PN532" as Hardware
   
   User -> ACS: Select CLONE_CARD
   activate ACS
   ACS -> ACS: Enter CLONING_SOURCE state
   ACS -> ACS: Display "Scan source card"
   
   User -> Reader: Present source card
   activate Reader
   Reader -> Hardware: Read physical UID
   activate Hardware
   Hardware --> Reader: Return UID
   deactivate Hardware
   Reader --> ACS: Return source UID
   deactivate Reader
   
   ACS -> ACS: Store source UID
   ACS -> ACS: Enter CLONING_TARGET state
   ACS -> ACS: Display "Scan target card"
   
   User -> Reader: Present target card
   activate Reader
   Reader -> Hardware: Read target card
   activate Hardware
   Hardware --> Reader: Target ready
   deactivate Hardware
   
   Reader -> Reader: Authenticate sector
   Reader -> Hardware: Write cloned UID to custom sector
   activate Hardware
   Hardware --> Reader: Write successful
   deactivate Hardware
   
   Reader --> ACS: Clone complete
   deactivate Reader
   
   ACS -> ACS: Display "Clone successful"
   ACS -> ACS: Return to IDLE state
   deactivate ACS
   
   @enduml

MIFARE Memory Layout
--------------------

.. graphviz::
   :caption: MIFARE Classic Memory Layout for Card Cloning

   digraph memory_layout {
       rankdir=LR;
       node [shape=record, style=filled];
       
       subgraph cluster_sector0 {
           label="Sector 0\n(Manufacturer)";
           fillcolor=lightgray;
           style=filled;
           
           block0 [fillcolor=pink, label="Block 0\nManufacturer UID\n(Read-only)"];
           block1 [fillcolor=white, label="Block 1\nData"];
           block2 [fillcolor=white, label="Block 2\nData"];
           block3 [fillcolor=yellow, label="Block 3\nSector Trailer"];
       }
       
       subgraph cluster_sector1 {
           label="Sector 1\n(Custom Clone Data)";
           fillcolor=lightblue;
           style=filled;
           
           block4 [fillcolor=lightgreen, label="Block 4\nCloned UID (7 bytes)\n+ Magic Bytes"];
           block5 [fillcolor=lightgreen, label="Block 5\nTimestamp\n+ Metadata"];
           block6 [fillcolor=white, label="Block 6\nReserved"];
           block7 [fillcolor=yellow, label="Block 7\nSector Trailer\n(Keys)"];
       }
       
       subgraph cluster_sector2 {
           label="Sector 2-15\n(Available)";
           fillcolor=lightgray;
           style=filled;
           
           other [label="Other Sectors...", shape=box];
       }
       
       block0 -> block1 -> block2 -> block3;
       block3 -> block4;
       block4 -> block5 -> block6 -> block7;
       block7 -> other;
   }

Installation and Usage
----------------------

To use these diagram features in your documentation:

**Prerequisites:**

1. **For Graphviz**: Install Graphviz on your system
   
   - Windows: Download from https://graphviz.org/download/
   - Linux: ``sudo apt-get install graphviz``
   - macOS: ``brew install graphviz``

2. **For PlantUML**: Install Java and download PlantUML jar
   
   - Install Java JRE/JDK
   - Download plantuml.jar from https://plantuml.com/download
   - Place it in your PATH or update ``conf.py`` with the full path

   Alternatively, you can use the PlantUML server by modifying ``conf.py``:
   
   .. code-block:: python
   
      plantuml = 'http://www.plantuml.com/plantuml'

3. **Install Python dependencies**:

   .. code-block:: bash
   
      pip install -r requirements.txt

**Building the documentation**:

.. code-block:: bash

   cd docs
   make html

**Graphviz Syntax Example**:

.. code-block:: rst

   .. graphviz::
      :caption: My Diagram
      
      digraph MyGraph {
          A -> B;
          B -> C;
      }

**PlantUML Syntax Example**:

.. code-block:: rst

   .. uml::
      :caption: My UML Diagram
      
      @startuml
      class MyClass {
          + publicMethod()
          - privateMethod()
      }
      @enduml

See Also
--------

- :doc:`api` - Detailed API documentation
- :doc:`hardware` - Hardware setup and connections
- :doc:`card_cloning` - Card cloning implementation details
