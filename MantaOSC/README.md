This is an example program using libmanta to build a simple OSC router, which sends OSC messages in response to manta touches and receives OSC messages to control the LEDs.

Prerequisites
=============

* cmake
* liblo-dev (linux/MacOS)
* libudev-dev (linux/MacOS)


Build Instructions
==================

## Linux and MacOS

1. git clone https://github.com/ssfrr/libmanta.git
2. cd libmanta
3. git submodule update --init
4. cd MantaOSC
5. mkdir build
6. cd build
7. cmake ..
8. make
9. You'll find an executable called MantaOSC in your current directory

## Windows

1. Install msys2 from http://www.msys2.org/. Follow the instructions on that page to make sure the system is up-to-date.
2. Install the mingw64 toolchain and some more necessary packages by running
`pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake base-devel`
3. Make sure your shell is starting with the `MSYSTEM=MINGW64` environment variable set, which will set your paths to use the ming64 compiler. `which gcc` should show you `/mingw64/bin/gcc` or similar.
4. Download a `PKGBUILD` file for `liblo`:
    `wget https://raw.githubusercontent.com/guysherman/MINGW-packages/guypkgs/mingw-w64-liblo/PKGBUILD`
5. from the folder with the `PKGBUILD` file, run `makepkg-mingw -si`. You might get some complaints about not having the i686 toolchain, but that's OK.
6. from the `MantaOSC` directory (in this repository) create a build directory and `cd` into it, e.g. `mkdir build-win64; cd build-win64`
7. run `cmake -G"MSYS Makefiles" ..`
8. run `make`

## Build Troubleshooting:

###   file failed to open for writing (Invalid argument):

This can happen while running `cmake` on Windows. I'm not sure if it's a path length issue or a windows/unix path issue, but moving the `libmanta` project directory to within my MSYS2 home directory solved it.


Usage
=====

MantaOSC sends to localhost on port 31416 and receives commands on port
31417.

Continuous messages are sent whenever the value changes, and velocity
messages are sent on initial press, and again on release (with a
velocity of zero)

Sent Addresses
--------------

The following addresses each send an ID and value. The IDs are
zero-indexed.

* "/manta/continuous/pad"
* "/manta/continuous/slider"
* "/manta/continuous/button"
* "/manta/velocity/pad"
* "/manta/velocity/button"


Receive Addresses
-----------------

For commands that take color strings, the value can be either "red",
"amber", or "off".

### "/manta/ledcontrol"

Takes a string and an integer. The string can have the values
"padandbutton", "slider" or "button", and the integer is a 1 to enable
and 0 to disable OSC control of those LEDs. When not under OSC control
the LEDs are lit automatically on touch by the internal firmware of the
Manta.

### "/manta/led/pad"

Takes a color string and an integer representing the pad LED to be set.

### "/manta/led/pad/row"

Takes a color string and two integers. The first indicates the row to be
set, and the second represents a bitmask of which pads in the row should
be set to the given color.

### "/manta/led/pad/column"

Takes a color string and two integers. The first indicates the column to be
set, and the second represents a bitmask of which pads in the column should
be set to the given color.

### "/manta/led/pad/frame"

Takes a string and a binary blob. The first is a color string and the second is
interpreted as a set of bitmasks for all 6 pad rows. If fewer than 6 bytes are
given, the rest are considered zero. If more are given they are ignored.

If instead of a color the first argument is "all", the binary blob should be 12
bytes and sets both the amber and red LEDs (in that order). Any bits set to zero
in both masks cause the corresponding LED to be turned off.

### "/manta/led/slider"

Takes a color string and two integers. The first integer is the slider
ID and the second is a bitmask on the slider to be set to the given
color.

### "/manta/led/button"

Takes a color string and an integer representing the pad LED to be set.
