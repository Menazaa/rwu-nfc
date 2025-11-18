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
doc/
├── source/              # Source .rst files
│   ├── conf.py         # Sphinx configuration
│   ├── index.rst       # Main documentation page
│   ├── introduction.rst
│   ├── hardware.rst
│   ├── installation.rst
│   ├── usage.rst
│   ├── api.rst
│   ├── card_cloning.rst
│   ├── troubleshooting.rst
│   ├── _static/        # Static files (CSS, images, etc.)
│   └── _templates/     # Custom templates
├── build/              # Generated documentation (not in git)
├── Makefile           # Build script for Unix/Linux/macOS
├── make.bat           # Build script for Windows
├── requirements.txt   # Python dependencies
└── README.md          # This file
```

## Viewing the Documentation

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

- **Introduction**: Overview and system architecture
- **Hardware**: Component requirements and wiring diagrams
- **Installation**: Setting up the development environment
- **Usage**: Operating instructions and menu navigation
- **API Reference**: Detailed class and method documentation
- **Card Cloning**: Technical details of cloning functionality
- **Troubleshooting**: Common issues and solutions

## Contributing

When adding new documentation:

1. Create or edit `.rst` files in `source/`
2. Use reStructuredText format
3. Add new pages to the table of contents in `index.rst`
4. Build and verify the documentation locally
5. Commit both source files and updated index

## Resources

- [Sphinx Documentation](https://www.sphinx-doc.org/)
- [reStructuredText Primer](https://www.sphinx-doc.org/en/master/usage/restructuredtext/basics.html)
- [Read the Docs Theme](https://sphinx-rtd-theme.readthedocs.io/)
- [Breathe Documentation](https://breathe.readthedocs.io/) (for C++ API docs)
