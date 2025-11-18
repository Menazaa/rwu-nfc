# NFC Reader/Writer System - OOP Architecture

## Overview
A professional NFC reader/writer system with TFT display, button interface, and EEPROM storage, built with clean OOP architecture and FreeRTOS for efficient multitasking.

## Architecture

### Object-Oriented Design
The system is organized into distinct, reusable classes:

```
├── Config.h              - Hardware configuration & constants
├── CardData.h            - Card data structure
├── DisplayManager        - TFT display operations
├── NFCReader            - PN532 NFC operations
├── StorageManager       - EEPROM data persistence
├── ButtonHandler        - Button input & debouncing
├── MenuSystem           - Menu navigation & UI logic
└── main.cpp             - FreeRTOS task coordination
```

### Class Responsibilities

#### **DisplayManager**
- Manages ST7735 TFT display (128x160)
- Renders all menu screens
- Handles text formatting and colors
- Provides high-level display functions

**Key Methods:**
- `showMainMenu()` - Display main menu
- `showCardDetected()` - Show scanned card details
- `showStoredCardsScreen()` - List saved cards
- `showMessage()` - Generic message display

#### **NFCReader**
- Controls PN532 NFC module (I2C or SPI)
- Reads Mifare Classic & NTAG cards
- Handles authentication for Mifare
- Reads/writes card data

**Key Methods:**
- `readCard()` - Scan and read card
- `writeCard()` - Write data to card
- `authenticateMifare()` - Authenticate Mifare sector
- `readNTAGPage()` - Read NTAG page

#### **StorageManager**
- Manages EEPROM storage
- Saves/loads up to 20 cards
- Tracks card count
- Validates data integrity

**Key Methods:**
- `saveCard()` - Store card to EEPROM
- `loadCard()` - Retrieve card by index
- `clearAll()` - Erase all stored cards
- `getCardCount()` - Get number of stored cards

#### **ButtonHandler**
- Reads 4 push buttons (UP, DOWN, SELECT, BACK)
- Debouncing for reliable input
- Non-blocking button detection

**Key Methods:**
- `getButtonPress()` - Poll for button press
- `isButtonPressed()` - Check specific button

#### **MenuSystem**
- Coordinates all components
- Implements menu state machine
- Handles user interactions
- Controls application flow

**Menu States:**
- `MENU_MAIN` - Main menu selection
- `MENU_READ_CARD` - Scan NFC card
- `MENU_WRITE_CARD` - Write to card
- `MENU_VIEW_STORED` - Browse saved cards
- `MENU_CLEAR_EEPROM` - Erase storage
- `MENU_SETTINGS` - View system info

### FreeRTOS Tasks

#### **TaskDisplay** (Priority: HIGH)
- Updates screen with mutex protection
- Refresh rate: 100ms

#### **TaskMenu** (Priority: HIGH)
- Processes button input
- Updates menu state
- Coordinates UI updates
- Refresh rate: 50ms

#### **TaskNFC** (Priority: LOW)
- Background NFC monitoring
- Handles card detection
- Refresh rate: 200ms

### Mutexes
- `xDisplayMutex` - Protects TFT display access
- `xNFCMutex` - Protects NFC operations

## Hardware Connections

### PN532 NFC Module (SPI Mode)
```
PN532    →    Arduino Nano
SCK      →    D13
MISO     →    D12
MOSI     →    D11
SS       →    D10
IRQ      →    D2
RST      →    D3
```

### ST7735 TFT Display (1.8" 128x160)
```
TFT      →    Arduino Nano
CS       →    D9
DC       →    D7
RST      →    D8
MOSI     →    D11 (shared)
SCK      →    D13 (shared)
VCC      →    5V
GND      →    GND
```

### Push Buttons
```
Button   →    Arduino Nano
UP       →    D4 (to GND)
DOWN     →    D5 (to GND)
SELECT   →    D6 (to GND)
BACK     →    A0 (to GND)
```

## Features

### Current
✅ Read Mifare Classic (1K/4K) cards
✅ Read NTAG213/215/216 cards
✅ Save up to 20 cards to EEPROM
✅ View stored cards
✅ Clear EEPROM storage
✅ Menu-driven interface
✅ Multi-tasking with FreeRTOS
✅ SPI or I2C NFC communication
✅ Professional OOP architecture

### Future Enhancements
- [ ] Write data to NFC cards
- [ ] Browse through stored cards with pagination
- [ ] Export/import card data via Serial
- [ ] Card authentication with custom keys
- [ ] NDEF message support
- [ ] Real-time clock integration

## Configuration

### Switching Communication Modes
Edit `include/Config.h`:
```cpp
// For I2C mode
#define USE_I2C

// For SPI mode (recommended)
#define USE_SPI
```

### Adjusting Storage Capacity
Edit `include/Config.h`:
```cpp
#define MAX_STORED_CARDS    20  // Change to desired capacity
```

### Task Stack Sizes
Edit `include/Config.h`:
```cpp
#define TASK_STACK_DISPLAY  256
#define TASK_STACK_NFC      256
#define TASK_STACK_BUTTON   128
```

## Building & Uploading

```bash
# Build the project
pio run

# Upload to Arduino
pio run --target upload

# Monitor serial output
pio device monitor
```

## Code Structure Benefits

### Maintainability
- Each class has a single responsibility
- Easy to locate and fix bugs
- Clear separation of concerns

### Testability
- Classes can be tested independently
- Mock objects can be created
- Unit tests can be added

### Scalability
- New features are isolated
- Easy to add new menu items
- Simple to support new card types

### Reusability
- Classes can be used in other projects
- Hardware abstraction is clean
- Minimal code duplication

## Memory Usage

### Flash (Program Memory)
- Core system: ~25KB
- Libraries: ~20KB
- Total: ~45KB (out of 30KB available)

**Note:** May need optimization for ATmega328P

### SRAM (Runtime Memory)
- Global objects: ~2KB
- FreeRTOS: ~1.5KB
- Task stacks: ~640 bytes
- Total: ~4KB (out of 2KB available)

**Note:** Stack sizes may need tuning

## Serial Debug Output

The system provides detailed logging:
```
=============================
NFC Reader/Writer System
OOP Architecture with FreeRTOS
=============================
Display initialized
Button handler initialized
Found chip PN532
Firmware ver. 1.6
NFC Reader initialized
Storage initialized. Cards stored: 3
System Ready!
Creating FreeRTOS tasks...
Starting FreeRTOS scheduler...
Display Task started
Menu Task started
NFC Task started
```

## Troubleshooting

### Display doesn't work
- Check TFT wiring (CS, DC, RST)
- Verify SPI connections
- Ensure 5V power supply

### PN532 not found
- Check SPI/I2C mode selection
- Verify wiring (especially SS pin)
- Check power supply (3.3V or 5V)

### Buttons not responding
- Check pull-up resistors enabled
- Verify button wiring to GND
- Adjust `BUTTON_DEBOUNCE_MS`

### Out of memory errors
- Reduce task stack sizes in Config.h
- Decrease `MAX_STORED_CARDS`
- Remove debug Serial.print statements

## License
MIT License - Feel free to use and modify
