# Project File Structure

## Complete OOP Architecture

```
rwu-nfc/
│
├── platformio.ini              # Build configuration & dependencies
├── README_ARCHITECTURE.md      # Complete architecture documentation
│
├── include/                    # Header files
│   ├── Config.h               # Hardware pins & constants
│   ├── CardData.h             # Card data structure
│   ├── DisplayManager.h       # TFT display interface
│   ├── NFCReader.h            # PN532 NFC operations
│   ├── StorageManager.h       # EEPROM persistence
│   ├── ButtonHandler.h        # Button input handling
│   └── MenuSystem.h           # Menu state machine
│
└── src/                        # Implementation files
    ├── main.cpp               # Entry point & FreeRTOS tasks
    ├── DisplayManager.cpp     # TFT rendering implementation
    ├── NFCReader.cpp          # NFC read/write logic
    ├── StorageManager.cpp     # EEPROM operations
    ├── ButtonHandler.cpp      # Button debouncing
    └── MenuSystem.cpp         # Menu navigation logic
```

## Class Diagram

```
┌─────────────┐
│   main.cpp  │  (FreeRTOS orchestration)
└──────┬──────┘
       │
       ├───► MenuSystem ────┬───► DisplayManager (TFT)
       │                    │
       │                    ├───► ButtonHandler (Input)
       │                    │
       │                    ├───► NFCReader (PN532)
       │                    │
       │                    └───► StorageManager (EEPROM)
       │
       └───► CardData (Shared data structure)
```

## Dependencies Flow

```
MenuSystem
    ↓ uses
DisplayManager ← uses → Config.h
    ↓ uses
CardData.h

MenuSystem
    ↓ uses
ButtonHandler ← uses → Config.h

MenuSystem
    ↓ uses
NFCReader ← uses → Config.h, CardData.h

MenuSystem
    ↓ uses
StorageManager ← uses → Config.h, CardData.h
```

## Key Design Patterns

### 1. **Composition over Inheritance**
- MenuSystem composes other managers
- No deep inheritance hierarchies
- Flexible and maintainable

### 2. **Dependency Injection**
```cpp
MenuSystem menu(display, buttons, nfcReader, storage);
```
- Objects passed to constructor
- Easy to test with mocks
- Loose coupling

### 3. **State Pattern**
```cpp
enum MenuState {
  MENU_MAIN,
  MENU_READ_CARD,
  // ...
};
```
- Clean state transitions
- Easy to add new states
- Clear control flow

### 4. **Single Responsibility Principle**
- Each class has ONE job
- DisplayManager → Display
- NFCReader → NFC operations
- StorageManager → EEPROM
- etc.

## Benefits of This Architecture

### ✅ Separation of Concerns
Each file handles a specific aspect of the system

### ✅ Easy to Understand
New developers can navigate the code quickly

### ✅ Maintainable
Changes are localized to specific classes

### ✅ Testable
Each class can be unit tested independently

### ✅ Scalable
Adding features doesn't require major refactoring

### ✅ Reusable
Classes can be used in other projects

## Compared to Monolithic Design

### Before (Monolithic):
```
main.cpp (650+ lines)
├── All NFC code
├── All display code
├── All button code
├── All storage code
└── All menu logic
```
**Problems:**
- Hard to find code
- Difficult to test
- Changes affect everything
- Code duplication

### After (OOP):
```
main.cpp (175 lines) - Just orchestration
├── DisplayManager.cpp (200 lines)
├── NFCReader.cpp (150 lines)
├── StorageManager.cpp (100 lines)
├── ButtonHandler.cpp (80 lines)
└── MenuSystem.cpp (150 lines)
```
**Benefits:**
- Clear organization
- Easy to locate bugs
- Testable components
- Minimal coupling

## Memory Footprint

### Flash (Program) Memory
```
Class                Size
─────────────────────────
DisplayManager       ~5KB
NFCReader           ~4KB
StorageManager      ~2KB
ButtonHandler       ~1KB
MenuSystem          ~3KB
main.cpp            ~2KB
FreeRTOS           ~10KB
Libraries          ~20KB
─────────────────────────
TOTAL              ~47KB
```

### SRAM (Runtime) Memory
```
Component           Size
─────────────────────────
Global Objects      ~2KB
FreeRTOS Kernel    ~1.5KB
Task Stacks        ~640B
Card Data          ~64B
─────────────────────────
TOTAL              ~4.2KB
```

**Note:** ATmega328P has 32KB Flash and 2KB SRAM. 
Memory optimization may be needed.

## Adding New Features

### Example: Add WiFi Card Upload

1. **Create new class:**
```
include/WiFiManager.h
src/WiFiManager.cpp
```

2. **Add to MenuSystem:**
```cpp
class MenuSystem {
  WiFiManager& wifi;  // Add reference
  // ...
};
```

3. **Add menu state:**
```cpp
enum MenuState {
  // ...
  MENU_WIFI_UPLOAD
};
```

4. **Implement handler:**
```cpp
void handleWiFiUpload(Button button) {
  wifi.uploadCard(lastScannedCard);
}
```

That's it! No other files need changes.

## Best Practices Applied

✅ **DRY** (Don't Repeat Yourself)
- Common code in reusable functions
- Configuration centralized in Config.h

✅ **KISS** (Keep It Simple, Stupid)
- Simple class interfaces
- Clear function names
- Minimal complexity

✅ **SOLID Principles**
- Single Responsibility
- Open/Closed (extend via composition)
- Dependency Inversion (inject dependencies)

✅ **Clean Code**
- Meaningful names
- Small functions
- Comments where needed
- Consistent formatting

## Future Improvements

### Code Quality
- [ ] Add unit tests
- [ ] Add integration tests
- [ ] Document with Doxygen
- [ ] Static analysis (cppcheck)

### Features
- [ ] Write to NFC cards
- [ ] NDEF message parsing
- [ ] Card history tracking
- [ ] Backup/restore via Serial

### Optimization
- [ ] Reduce memory usage
- [ ] Optimize task priorities
- [ ] Profile performance
- [ ] Remove unused library features

## Conclusion

This OOP architecture transforms a complex embedded system into a **maintainable**, **testable**, and **scalable** codebase. Each component has a clear purpose, making development and debugging significantly easier.
