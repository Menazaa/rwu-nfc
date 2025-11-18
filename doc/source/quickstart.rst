Quick Start - Building Documentation
====================================

This is a quick reference for building the Sphinx documentation.

Installation
------------

1. Install Python 3.6+ from https://www.python.org/

2. Install documentation dependencies::

    cd doc
    pip install -r requirements.txt

Building
--------

HTML Documentation (Recommended)
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Windows::

    cd doc
    .\make.bat html

Linux/macOS::

    cd doc
    make html

The documentation will be generated in ``doc/build/html/``.

PDF Documentation
^^^^^^^^^^^^^^^^^

Requires LaTeX installation.

Windows::

    .\make.bat latexpdf

Linux/macOS::

    make latexpdf

The PDF will be in ``doc/build/latex/NFCAccessControl.pdf``.

Viewing
-------

Open the generated documentation:

Windows::

    start build\html\index.html

Linux::

    xdg-open build/html/index.html

macOS::

    open build/html/index.html

Cleaning
--------

Remove generated files::

    # Windows
    .\make.bat clean

    # Linux/macOS
    make clean

Common Issues
-------------

ImportError: No module named sphinx
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install Sphinx::

    pip install -r requirements.txt

sphinx-build command not found
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Ensure Python Scripts directory is in PATH, or use::

    python -m sphinx -M html source build

Theme not found
^^^^^^^^^^^^^^^

Install the Read the Docs theme::

    pip install sphinx-rtd-theme

LaTeX errors when building PDF
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Install a LaTeX distribution:

- Windows: MiKTeX or TeX Live
- Linux: ``sudo apt-get install texlive-full``
- macOS: MacTeX

Tips
----

* Use ``make html`` for quick iterations
* Add ``-W`` flag to treat warnings as errors (production builds)
* Use ``make linkcheck`` to verify external links
* Rebuild documentation after editing ``.rst`` files

For more information, see ``doc/README.md``.
