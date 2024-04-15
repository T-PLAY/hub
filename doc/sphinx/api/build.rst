Building
========

Dependencies
------------

No dependency.

Quick Start
-----------

::

    git clone https://github.com/T-PLAY/hub.git
    cd hub
    mkdir build
    cmake -B build -DCMAKE_INSTALL_PREFIX=install
    cmake --build build
    cmake --install build


Prerequisites
-------------

hub uses CMake to generate project files for various platform.

Windows
~~~~~~~

* Git Bash (for the use of quick start lines)

* Visual Studio IDE:

    Open CMakeList.txt path (need CMake tool for Visual Studio)

* QtCreator

UWP
***

Add cmake options during the configuration.

::

    cmake -B build -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

MacOS
~~~~~

* CLI

* QtCreator

* Xcode

Linux
~~~~~

* CLI

* QtCreator

* Vscode


Installation
------------

Installed paths :

* lib/libhub.{so, a, dll}

* include/*.{hpp, inl}

* lib/cmake/hub/hubConfig.cmake


