# NFC Access Control System Documentation

This directory contains the Sphinx documentation for the NFC Access Control System.

## Building the Documentation

### Prerequisites

Install Python 3.6 or later, then install the required packages:

```bash
pip install -r requirements.txt
```

### Build HTML Documentation

**On Windows:**
```powershell
.\make.bat html
```

**On Linux/macOS:**
```bash
make html
```

The generated HTML documentation will be in `build/html/index.html`.

**Using Virtual Environment (Recommended):**
```powershell
# Activate the virtual environment (Windows)
..\.venv\Scripts\Activate.ps1

# Install dependencies
pip install -r requirements.txt

# Build documentation
.\make.bat html
```

### Build PDF Documentation

**On Windows:**
```powershell
.\make.bat latexpdf
```

**On Linux/macOS:**
```bash
make latexpdf
```

This requires LaTeX to be installed on your system.

### Other Build Formats

View all available formats:
```bash
make help
```

Available formats include:
- `html` - HTML pages (default)
- `dirhtml` - HTML pages with single directory per document
- `singlehtml` - Single HTML page with entire documentation
- `epub` - EPUB e-book format
- `latex` - LaTeX files
- `latexpdf` - PDF via LaTeX
- `text` - Plain text
- `man` - Manual pages
- `linkcheck` - Check all external links

## Documentation Structure

```
docs/
├── source/              # Source .rst files
│   ├── conf.py         # Sphinx configuration
│   ├── index.rst       # Main documentation page
│   ├── introduction.rst    # System overview with architecture diagrams
│   ├── hardware.rst        # Component details with wiring diagrams
│   ├── installation.rst    # Setup and build instructions
│   ├── usage.rst           # Operating guide with menu flowcharts
│   ├── api.rst             # API reference with state machine diagrams
│   ├── eeprom.rst          # Memory layout with detailed diagrams
│   ├── card_cloning.rst    # Cloning technology documentation
│   ├── troubleshooting.rst # Common issues and solutions
│   ├── quickstart.rst      # Quick reference guide
│   ├── _static/        # Static files (CSS, images, etc.)
│   └── _templates/     # Custom templates
├── build/              # Generated documentation (not in git)
├── Makefile           # Build script for Unix/Linux/macOS
├── make.bat           # Build script for Windows
├── requirements.txt   # Python dependencies (includes graphviz)
├── .gitignore         # Excludes build artifacts
└── README.md          # This file
```

## Viewing the Documentation

### Online (Recommended)

The documentation is deployed and available online at:

**🌐 [https://menazaa.github.io/rwu-nfc/](https://menazaa.github.io/rwu-nfc/)**

### Local Build

After building, open the documentation in your browser:

**On Windows:**
```powershell
start build\html\index.html
```

**On Linux:**
```bash
xdg-open build/html/index.html
```

**On macOS:**
```bash
open build/html/index.html
```

## Updating Documentation

1. Edit the `.rst` files in the `source/` directory
2. Rebuild the documentation: `make html` or `.\make.bat html`
3. Refresh your browser to see changes

## Documentation Sections

- **Introduction**: Overview and system architecture with architecture diagrams
- **Hardware**: Component requirements with detailed wiring diagrams (Graphviz)
- **Installation**: Setting up the development environment and PlatformIO
- **Usage**: Operating instructions with menu navigation flowcharts
- **API Reference**: Detailed class and method documentation with state machine diagrams
- **EEPROM Layout**: Memory structure with organization and operation flow diagrams
- **Card Cloning**: Technical details of cloning technology with process flowcharts
- **Troubleshooting**: Common issues and solutions
- **Quick Start**: Quick reference for building documentation

## Features

### Interactive Diagrams

This documentation includes **8+ professional Graphviz diagrams**:

- System architecture overview
- State machine with color-coded states
- Hardware wiring connections
- Menu navigation flows
- Card cloning process
- Mifare Classic memory structure
- EEPROM layout and operations
- Read/write operation flows

All diagrams are automatically generated from embedded Graphviz dot language.

## Contributing

When adding new documentation:

1. Create or edit `.rst` files in `source/`
2. Use reStructuredText format
3. Add new pages to the table of contents in `index.rst`
4. Build and verify the documentation locally
5. Commit both source files and updated index

## Dependencies

This documentation uses the following Python packages (see `requirements.txt`):

- **sphinx** (>=5.0.0) - Documentation generator
- **sphinx-rtd-theme** (>=1.2.0) - Read the Docs theme
- **breathe** (>=4.35.0) - C++ API documentation support
- **graphviz** (>=0.20) - Diagram generation

**Note**: Graphviz executable must also be installed on your system for diagram generation.

## Resources

- [Sphinx Documentation](https://www.sphinx-doc.org/)
- [reStructuredText Primer](https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html)
- [Read the Docs Theme](https://sphinx-rtd-theme.readthedocs.io/)
- [Breathe Documentation](https://breathe.readthedocs.io/) (for C++ API docs)
- [Graphviz Documentation](https://graphviz.org/documentation/)
- [Online Documentation](https://menazaa.github.io/rwu-nfc/)
