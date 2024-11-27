# Template objective

This is a template project for a program that uses the raylib library.

The cmake file is configured to also use googletest for unit testing.

The project shares the same sources files between the main program and the unit tests, except
for the main.cpp and the test_main.cpp files, which are the entry points that differ between
the main program and the unit tests program.

# Prerequisites:
   
## Linux

* cmake 3.11+
* Development tools:

    ```sudo apt install build-essential git```

* Development libraries:
    * Ubuntu:
    
        ```sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev```
    * Fedora:
    
      ```sudo dnf install alsa-lib-devel mesa-libGL-devel libX11-devel libXrandr-devel libXi-devel libXcursor-devel libXinerama-devel```

* NOTE: Only tested on Ubuntu 22.04

## Windows

(not tested recently)

* A compiler
    * VS2019
    * Or use the Windows Installer: https://raysan5.itch.io/raylib
* cmake 3.11+
* git

* NOTE: (was) Only tested on Windows 10
