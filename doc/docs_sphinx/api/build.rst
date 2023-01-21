Building
========

Dependencies
------------

No dependency.

Quick Start
-----------

::

    git clone git@gitlab.irit.fr:storm/repos/projects/private/dcs/plateforme-experimentale/hub.git
    cd hub
    type=Release
    mkdir build-$type
    cmake -B build-$type -DCMAKE_BUILD_TYPE=$type -DCMAKE_INSTALL_PREFIX=install
    cmake --build build-$type --config $type
    cmake --install build-$type --config $type


Prerequisites
-------------

hub uses CMake to generate project files for various platform.

Windows
~~~~~~~

* Git Bash (for the use of quick start lines)

or

Visual Studio IDE:

Open CMakeList.txt path (need CMake tool for Visual Studio)

Xcode
~~~~~

UWP
~~~

Add cmake options during the configuration.

::

    cmake -B build -DCMAKE_BUILD_TYPE=Release -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION=10.0

Installation
------------

Installed paths :

* lib/libhub.{so, a, dll}

* include/*.{hpp, inl}

* lib/cmake/hub/hubConfig.cmake


Tools
-----

* bin/app-server (server hub)


