This is an example program using libmanta to build a simple OSC router,
which sends OSC mesages in response to manta touches and receives OSC
messages to control the LEDs.


This example project sometimes drifts out-of-date with the rest of the
library, so if you have trouble building please file an issue on github
issues at https://github.com/ssfrr/libmanta/issues

Prerequisites
=============

* cmake
* liblo-dev
* libudev-dev


Build Instructions
==================

1. git clone https://github.com/ssfrr/libmanta.git
2. cd libmanta
3. git submodule update --init
4. cd MantaOSC
5. mkdir build
6. cd build
7. cmake ..
8. make
9. You'll find an executable called MantaOSC in your current directory

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

### "/manta/led/slider"

Takes a color string and two integers. The first integer is the slider
ID and the second is a bitmask on the slider to be set to the given
color.

### "/manta/led/button"

Takes a color string and an integer representing the pad LED to be set.
