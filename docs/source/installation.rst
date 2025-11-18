Installation
============

This guide will help you set up the development environment and build the
NFC Access Control System firmware.

Prerequisites
-------------

Required Software
^^^^^^^^^^^^^^^^^

* **PlatformIO**: Modern IDE for embedded development
  
  - PlatformIO Core (CLI) or PlatformIO IDE
  - Recommended: PlatformIO IDE for VS Code

* **USB Drivers**: CH340/FT232 drivers for Arduino Nano communication

* **Git**: For cloning the repository (optional)

Recommended Tools
^^^^^^^^^^^^^^^^^

* **Visual Studio Code**: With PlatformIO extension
* **Serial Monitor**: For debugging (built into PlatformIO)
* **NFC Tools**: Mobile app for testing NFC cards

Setting Up PlatformIO
----------------------

Option 1: PlatformIO IDE (Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Install Visual Studio Code from https://code.visualstudio.com/

2. Install the PlatformIO extension:
   
   - Open VS Code
   - Go to Extensions (Ctrl+Shift+X)
   - Search for "PlatformIO IDE"
   - Click Install

3. Wait for PlatformIO to complete its installation

Option 2: PlatformIO Core (CLI)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Install Python 3.6 or later

2. Install PlatformIO Core::

    pip install platformio

3. Verify installation::

    pio --version

Getting the Source Code
------------------------

Clone from Repository
^^^^^^^^^^^^^^^^^^^^^

If the project is in a Git repository::

    git clone https://github.com/Menazaa/rwu-nfc.git
    cd rwu-nfc

Download ZIP
^^^^^^^^^^^^

1. Download the project ZIP file
2. Extract to your desired location
3. Navigate to the project directory

Project Structure
-----------------

The project follows the PlatformIO structure::

    rwu-nfc/
    ├── doc/                    # Documentation (Sphinx)
    ├── examples/               # Example code
    ├── include/                # Header files
    │   ├── AccessControlSystem.h
    │   ├── Config.h
    │   └── NFCReader.h
    ├── lib/                    # Project libraries
    ├── src/                    # Source files
    │   ├── main.cpp
    │   ├── AccessControlSystem.cpp
    │   └── NFCReader.cpp
    ├── test/                   # Unit tests
    ├── platformio.ini          # PlatformIO configuration
    └── README.md

Configuration
-------------

The ``platformio.ini`` file contains the build configuration. Default settings::

    [env:nanoatmega328]
    platform = atmelavr
    board = nanoatmega328
    framework = arduino
    lib_deps = 
        adafruit/Adafruit PN532@^1.2.2
        arduino-libraries/LiquidCrystal@^1.0.7

Pin Configuration
^^^^^^^^^^^^^^^^^

Default pin assignments are defined in ``include/Config.h``. Modify if needed::

    // PN532 NFC Module pins
    #define PN532_SCK  (13)
    #define PN532_MISO (12)
    #define PN532_MOSI (11)
    #define PN532_SS   (10)
    #define PN532_IRQ  (2)
    #define PN532_RST  (3)

    // LCD Display pins
    #define LCD_RS (4)
    #define LCD_EN (5)
    #define LCD_D4 (6)
    #define LCD_D5 (7)
    #define LCD_D6 (8)
    #define LCD_D7 (9)

    // Button pins
    #define BTN_UP     (A0)
    #define BTN_DOWN   (A1)
    #define BTN_SELECT (A2)
    #define BTN_BACK   (A3)

    // Relay pin
    #define RELAY_PIN (A4)

Building the Firmware
----------------------

Using PlatformIO IDE
^^^^^^^^^^^^^^^^^^^^

1. Open the project in VS Code:
   
   - File → Open Folder
   - Select the project directory

2. PlatformIO will automatically detect the project

3. Build the project:
   
   - Click the Build icon (✓) in the status bar
   - Or use: Ctrl+Alt+B
   - Or from terminal: ``pio run``

Using PlatformIO CLI
^^^^^^^^^^^^^^^^^^^^

Navigate to the project directory and run::

    pio run

This will:

* Download required libraries
* Compile the source code
* Generate firmware binary

Uploading to Arduino
---------------------

Preparing the Arduino
^^^^^^^^^^^^^^^^^^^^^

1. Connect Arduino Nano to your computer via USB
2. Install CH340 or FT232 drivers if needed
3. Verify the port is detected

Using PlatformIO IDE
^^^^^^^^^^^^^^^^^^^^

1. Connect the Arduino Nano

2. Upload the firmware:
   
   - Click the Upload icon (→) in the status bar
   - Or use: Ctrl+Alt+U
   - Or from terminal: ``pio run --target upload``

3. PlatformIO will automatically detect the port

Specifying Upload Port
^^^^^^^^^^^^^^^^^^^^^^

If multiple devices are connected, specify the port in ``platformio.ini``::

    [env:nanoatmega328]
    upload_port = COM3        # Windows
    ; upload_port = /dev/ttyUSB0  # Linux
    ; upload_port = /dev/cu.usbserial  # macOS

Or via command line::

    pio run --target upload --upload-port COM3

Monitoring Serial Output
-------------------------

Using PlatformIO
^^^^^^^^^^^^^^^^

Open the serial monitor:

* Click the Serial Monitor icon (🔌) in the status bar
* Or from terminal: ``pio device monitor``

The default baud rate is 115200 (configured in ``platformio.ini``).

Using Arduino IDE Serial Monitor
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1. Open Arduino IDE
2. Select the correct port
3. Set baud rate to 115200
4. Open Serial Monitor

Troubleshooting Installation
-----------------------------

Build Errors
^^^^^^^^^^^^

**Library not found**::

    # Clean and rebuild
    pio run --target clean
    pio lib install
    pio run

**Compiler errors**:

* Check that you're using the correct platform (Arduino)
* Verify all header files are in the ``include/`` directory

Upload Errors
^^^^^^^^^^^^^

**Port not found**:

* Check USB cable connection
* Install/update USB drivers
* Try a different USB port
* Check Device Manager (Windows) or ``dmesg`` (Linux)

**Upload timeout**:

* Press reset on Arduino right before upload
* Check that no serial monitor is open
* Verify upload speed in ``platformio.ini``

**Permission denied** (Linux)::

    sudo usermod -a -G dialout $USER
    # Log out and log back in

Verification
------------

After successful upload:

1. The LCD should display "NFC Access Control"
2. Press buttons to navigate the menu
3. Hold an NFC card to the reader
4. Check serial monitor for detailed logs

Next Steps
----------

* See :doc:`usage` for operating instructions
* Review :doc:`hardware` for wiring details
* Explore :doc:`api` for customization
