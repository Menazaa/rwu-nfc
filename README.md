# NFC Access Control System

> **A professional-grade, Arduino-based access control solution featuring advanced NFC card cloning technology and comprehensive card management capabilities.**

[![Platform](https://img.shields.io/badge/Platform-Arduino-blue.svg)](https://www.arduino.cc/)
[![Framework](https://img.shields.io/badge/Framework-PlatformIO-orange.svg)](https://platformio.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Documentation](https://img.shields.io/badge/Docs-Sphinx-brightgreen.svg)](https://menazaa.github.io/rwu-nfc/)

## 📖 Description

The NFC Access Control System is an embedded security solution designed for educational institutions, laboratories, and small-to-medium scale access control applications. Built on the Arduino platform with the industry-standard PN532 NFC module, it combines simplicity with advanced features like intelligent card cloning, multi-card authorization, and real-time access management.

### What Makes This Project Unique?

Unlike traditional access control systems, this project implements **custom sector card cloning technology** that works with standard Mifare Classic cards—no expensive "magic cards" required. The system intelligently manages both physical and cloned UIDs, providing flexible access control without the limitations of conventional NFC systems.

### Key Highlights

- 🔒 **Secure**: Local card database with EEPROM persistence, no network dependencies
- ⚡ **Fast**: IRQ-based card detection with <100ms response time
- 🎯 **Smart**: Automatic UID selection (cloned vs. physical)
- 🖥️ **User-Friendly**: Interactive LCD menu with intuitive button navigation
- 🔧 **Flexible**: Support for up to 40 authorized cards with easy management
- 📚 **Well-Documented**: Comprehensive Sphinx documentation with diagrams
- 🛠️ **Professional**: Built with PlatformIO for modern embedded development

### Perfect For

- **Educational Projects**: Learn about NFC technology, embedded systems, and access control
- **Laboratory Security**: Manage access to restricted areas in research facilities
- **Maker Spaces**: Control access to shared equipment and facilities
- **Office Access**: Small office door control with card management
- **Prototype Development**: Foundation for commercial access control products

## 🚀 Quick Start

### Prerequisites
- Arduino Nano or compatible board
- PN532 NFC module
- 16x2 LCD display
- 4 push buttons
- Relay module
- PlatformIO IDE or CLI

### Installation

```bash
# Clone the repository
git clone https://github.com/Menazaa/rwu-nfc.git
cd rwu-nfc

# Build the project
pio run

# Upload to your Arduino (adjust COM port as needed)
pio run --target upload --upload-port COM13

# Monitor serial output
pio device monitor --baud 115200
```

### First Use

1. **Power up the system** - LCD displays "NFC Access Control"
2. **Press SELECT** to enter menu
3. **Select "Register Card"** 
4. **Scan your first card** - It's now authorized
5. **Test access** - Scan the card to unlock the relay

That's it! Your access control system is ready.

## 📑 Table of Contents

- [Description](#-description)
- [Quick Start](#-quick-start)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Setup](#software-setup)
- [How It Works](#how-it-works)
- [Menu System](#menu-system)
- [Configuration](#configuration)
- [EEPROM Storage](#eeprom-storage)
- [Documentation](#-documentation)
- [Troubleshooting](#troubleshooting)
- [Contributing](#-contributing)
- [Security](#-security-considerations)
- [License & Credits](#license--credits)

## 🛠️ Technology Stack

| Component | Technology | Purpose |
|-----------|-----------|---------|
| **Microcontroller** | ATmega328P (Arduino Nano) | Main processing unit |
| **NFC Module** | PN532 @ 13.56MHz | Card reading/writing |
| **Communication** | SPI + IRQ | Fast, non-blocking I/O |
| **Display** | HD44780 (16x2 LCD) | User interface |
| **Storage** | EEPROM (1KB) | Persistent card database |
| **Build System** | PlatformIO | Modern embedded development |
| **Language** | C++11 | Firmware programming |
| **Documentation** | Sphinx + Graphviz | Technical documentation |
| **Libraries** | Adafruit PN532, LiquidCrystal | Hardware abstraction |

## Features

### Core Functionality
- **Multi-card Support**: Store up to 40 authorized cards in EEPROM
- **Smart Card Cloning**: Clone any Mifare Classic card to any other card using custom sector
- **Real-time Access Control**: Instant card verification with relay output
- **Interactive Menu System**: Full-featured LCD menu with button navigation
- **IRQ-based Detection**: Fast, non-blocking card detection using interrupts

### Advanced Card Management
- **Custom Sector Technology**: Uses Sector 1 (blocks 4-7) for cloned UID storage
- **Dual UID System**: 
  - Physical UID (read-only manufacturer block)
  - Cloned UID (writable custom sector)
- **Automatic UID Selection**: Uses cloned UID if present, falls back to physical UID
- **No Magic Cards Required**: Clone to any standard Mifare Classic 1K/4K card

## Hardware Requirements

### Components
- **Microcontroller**: Arduino Nano (ATmega328P)
- **NFC Module**: PN532 (SPI mode with IRQ)
- **Display**: 16x2 LCD (4-bit parallel mode)
- **Buttons**: 4 push buttons (UP, DOWN, SELECT, BACK)
- **Output**: Relay module for door lock control

### Pin Configuration

#### PN532 NFC Module (SPI)
| Pin | Arduino | Description |
|-----|---------|-------------|
| SCK | D13 | SPI Clock |
| MISO | D12 | SPI MISO |
| MOSI | D11 | SPI MOSI |
| SS | D10 | SPI Chip Select |
| IRQ | D2 | Interrupt Request |
| RST | D3 | Reset |

#### LCD Display (4-bit Parallel)
| Pin | Arduino | Description |
|-----|---------|-------------|
| RS | D4 | Register Select |
| EN | D5 | Enable |
| D4 | D6 | Data 4 |
| D5 | D7 | Data 5 |
| D6 | D8 | Data 6 |
| D7 | D9 | Data 7 |

#### Buttons & Output
| Component | Arduino | Type |
|-----------|---------|------|
| BTN_UP | A0 | Active LOW, internal pullup |
| BTN_DOWN | A1 | Active LOW, internal pullup |
| BTN_SELECT | A2 | Active LOW, internal pullup |
| BTN_BACK | A3 | Active LOW, internal pullup |
| RELAY | A6 | Configurable HIGH/LOW |

## Software Setup

### PlatformIO Installation
1. Install [VS Code](https://code.visualstudio.com/)
2. Install PlatformIO extension
3. Clone this repository
4. Open folder in VS Code
5. PlatformIO will auto-install dependencies

### Build & Upload
```bash
# Build firmware
pio run

# Upload to Arduino (adjust COM port)
pio run --target upload --upload-port COM13

# Open serial monitor (115200 baud)
pio device monitor --baud 115200
```

### Dependencies
- Adafruit PN532 v1.3.4
- LiquidCrystal v1.0.7
- SPI (built-in)
- Wire (built-in)
- EEPROM (built-in)

## How It Works

### Card Detection & Custom Sector
When a card is scanned:
1. **Read Physical UID** from block 0 (manufacturer, read-only)
2. **Read Custom Sector 1** (blocks 4-7) for cloned UID
3. **Use Effective UID** for access control:
   - If custom sector has cloned UID → use cloned UID
   - Otherwise → use physical manufacturer UID

### Custom Sector Structure (Block 4)
```
Byte 0-1:   Magic bytes (0xAC, 0xDB) - Identifies initialized card
Byte 2:     UID length (0-7)
Byte 3-9:   Cloned UID (up to 7 bytes)
Byte 10-15: Reserved for future use
```

### Card Cloning Process
1. **Select Clone from Menu**
2. **Scan Source Card**
   - System reads effective UID (cloned if present, otherwise physical)
   - Display shows "Remove & scan new"
3. **Remove Source Card**
   - Automatic timeout (1 second) detects removal
   - IRQ detection restarts for next card
4. **Scan Target Card** (must be Mifare Classic 1K/4K)
   - System writes source UID to target's Sector 1
   - Verifies write operation
   - Target now acts as source card

### Access Control Logic
```cpp
// Card scanned
if (hasClonedUID) {
    checkUID = clonedUID;  // From Sector 1
} else {
    checkUID = physicalUID;  // From block 0
}

if (isAuthorized(checkUID)) {
    grantAccess();
    unlockDoor(3000ms);
}
```

## Menu System

### Main Menu (Long-press SELECT)
- **Register Card**: Add new card to authorized list
- **Delete Card**: Remove card from authorized list
- **List Cards**: Browse stored cards (UP/DOWN to navigate)
- **Clone Card**: Copy UID from one card to another
- **Settings**: System configuration
- **Clear All**: Remove all authorized cards
- **Exit Menu**: Return to scan mode

### Navigation
- **UP/DOWN**: Navigate menu items or list cards
- **SELECT**: Confirm selection
- **BACK**: Return to previous screen/exit menu
- **LONG PRESS SELECT**: Open menu from main screen

## Serial Monitor Output

### Normal Card Scan
```
Physical UID: 12 34 56 78
Access GRANTED
```

### Cloned Card Scan
```
Physical UID: AA BB CC DD
Cloned UID: 12 34 56 78 (from Sector 1)
Access GRANTED
```

### Clone Operation
```
Clone source UID: 12 34 56 78
(Using physical manufacturer UID)
Remove source card and scan target card...
Target card detected, cloning to custom sector...
Cloning UID to sector 1: 12 34 56 78
Writing cloned UID to block 4: 12 34 56 78
Clone write SUCCESS
Clone VERIFIED!
NFC reader reset, ready for next card
```

## Configuration

Edit `include/Config.h` to customize:

```cpp
// Access Control
#define RELAY_ACTIVE_HIGH  true    // Relay trigger level
#define DOOR_UNLOCK_TIME   3000    // Door unlock duration (ms)
#define MAX_STORED_CARDS   40      // Maximum authorized cards

// Timing
#define MESSAGE_DISPLAY_TIME  2000  // Status message duration
#define MENU_TIMEOUT          30000 // Auto-exit menu timeout
#define CARD_TIMEOUT          1000  // Card removal detection

// NFC Mode
#define NFC_COMM_MODE  NFC_COMM_SPI     // SPI or I2C
#define NFC_READ_MODE  NFC_READ_IRQ     // IRQ or POLLING
```

## EEPROM Storage

### Memory Map
| Address | Content | Size |
|---------|---------|------|
| 0-1 | Magic number (0xABCD) | 2 bytes |
| 2 | Card count | 1 byte |
| 4+ | Card storage | 9 bytes/card |

### Card Storage Format (per card)
```
Byte 0:   UID length
Byte 1:   Active flag
Byte 2-8: UID data (7 bytes max)
```

## Troubleshooting

### Card Not Detected
- Check PN532 wiring (especially SPI pins)
- Verify IRQ pin connected to D2
- Check Serial Monitor for "PN532 NOT FOUND!"
- Ensure card is Mifare Classic 1K/4K or NTAG

### Clone Not Working
- **Target must be Mifare Classic 1K or 4K**
- NTAG/Ultralight cards not supported for clone target
- Check authentication (default key: FF FF FF FF FF FF)
- Verify custom sector is writable

### Reading Stops After First Card
- Remove card completely before scanning again
- Wait 1 second for card removal detection
- Check Serial Monitor for "Card removed, restarting detection"

### Display Issues
- Check LCD contrast (potentiometer adjustment)
- Verify 4-bit parallel wiring
- Check power supply (5V, stable)

### Buttons Not Responding
- Verify active LOW configuration
- Check internal pullups enabled
- Test button continuity
- Adjust debounce time if needed

## Technical Details

### Non-Blocking Architecture
- All operations use `millis()` for timing
- No `delay()` calls in main loop
- IRQ-driven card detection
- Maintains responsive button handling

### Memory Optimization
- Flash: ~20KB / 30KB (66%)
- RAM: ~1.1KB / 2KB (55%)
- PROGMEM used for strings
- Efficient EEPROM usage

### Security Considerations
- Default Mifare key used (FF FF...)
- Physical UID remains untouched
- Custom sector can be overwritten
- Not suitable for high-security applications
- Designed for access convenience, not cryptographic security

## Advanced Features

### Multi-Clone Support
- Clone one card to multiple targets
- All clones act as original card
- Each clone retains unique physical UID
- Effective for distributing access

### Card Re-cloning
- Can clone a cloned card
- System automatically uses effective UID
- Enables card duplication chains

### Blank Card Detection
- System detects uninitialized cards
- Shows physical UID for new cards
- Custom sector created on first clone

## Development

### Project Structure
```
rwu-nfc/
├── include/
│   ├── AccessControlSystem.h  # Main system class
│   ├── NFCReader.h            # NFC operations
│   └── Config.h               # Hardware config
├── src/
│   ├── AccessControlSystem.cpp
│   ├── NFCReader.cpp
│   └── main.cpp
├── platformio.ini             # Build configuration
└── README.md
```

### Adding Features
1. Modify classes in `include/` and `src/`
2. Update configuration in `Config.h`
3. Build and test: `pio run`
4. Upload: `pio run --target upload`

### Debugging
Enable verbose output in Serial Monitor (115200 baud):
- Card detection events
- UID comparisons
- Sector read/write operations
- State transitions
- Error messages

## License & Credits

**Author**: Mohamed Aly  
**Organization**: RWU NFC Lab  
**Platform**: Arduino + PlatformIO  
**Libraries**: Adafruit PN532, Arduino LiquidCrystal

This project demonstrates advanced NFC card management without requiring special magic/writable UID cards by using custom data sectors.

## 📚 Documentation

Comprehensive documentation is available online at **[https://menazaa.github.io/rwu-nfc/](https://menazaa.github.io/rwu-nfc/)**

- **[Introduction](https://menazaa.github.io/rwu-nfc/introduction.html)** - System overview and architecture diagrams
- **[Hardware Setup](https://menazaa.github.io/rwu-nfc/hardware.html)** - Wiring diagrams and component details
- **[Installation Guide](https://menazaa.github.io/rwu-nfc/installation.html)** - Development environment setup
- **[Usage Guide](https://menazaa.github.io/rwu-nfc/usage.html)** - Operating instructions and menu navigation
- **[API Reference](https://menazaa.github.io/rwu-nfc/api.html)** - Class documentation and code examples
- **[EEPROM Layout](https://menazaa.github.io/rwu-nfc/eeprom.html)** - Memory structure and management
- **[Card Cloning](https://menazaa.github.io/rwu-nfc/card_cloning.html)** - Technical details of cloning technology
- **[Troubleshooting](https://menazaa.github.io/rwu-nfc/troubleshooting.html)** - Common issues and solutions

### Building Documentation Locally

To build the documentation from source:

```bash
cd docs
pip install -r requirements.txt
make html  # or .\make.bat html on Windows
```

Open `docs/build/html/index.html` in your browser.

## 🤝 Contributing

Contributions are welcome! Here's how you can help:

1. **Report Bugs**: Open an issue with detailed reproduction steps
2. **Suggest Features**: Share your ideas for improvements
3. **Submit Pull Requests**: Fork, implement, and submit PRs
4. **Improve Documentation**: Fix typos, add examples, or clarify instructions
5. **Share Use Cases**: Tell us how you're using this project

### Development Guidelines

- Follow existing code style and conventions
- Test thoroughly before submitting PRs
- Update documentation for new features
- Add comments for complex logic
- Keep commits focused and descriptive

## 🔒 Security Considerations

**Important**: This system is designed for educational and convenience purposes. For production deployments:

- ✅ Use encrypted card data storage
- ✅ Implement access logging and audit trails
- ✅ Add physical tamper detection
- ✅ Use secure communication protocols for remote management
- ✅ Regular security audits and updates
- ✅ Consider upgrading to DESFire or other secure card types

## 📊 Project Status

- ✅ Core access control functionality
- ✅ Card registration and deletion
- ✅ Advanced card cloning with custom sectors
- ✅ IRQ-based fast card detection
- ✅ Interactive LCD menu system
- ✅ Comprehensive Sphinx documentation with diagrams
- 🚧 Network connectivity (future)
- 🚧 Mobile app integration (future)
- 🚧 Time-based access rules (future)

## 🌟 Acknowledgments

- **Adafruit** for the excellent PN532 library
- **PlatformIO** for modern embedded development tools
- **Arduino Community** for extensive resources and support
- **RWU NFC Lab** for project sponsorship

## Support

For issues and questions:
1. Check Serial Monitor output (115200 baud)
2. Verify hardware connections against wiring diagram
3. Review [Troubleshooting Documentation](https://menazaa.github.io/rwu-nfc/troubleshooting.html)
4. Check card compatibility (Mifare Classic 1K/4K required for cloning)
5. Open an issue on GitHub with detailed information

---

**⭐ If you find this project useful, please consider giving it a star!**

*Made with ❤️ for the maker community*
