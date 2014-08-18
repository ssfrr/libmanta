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

Takes two strings. The first is a color string and the second is
interpreted as a set of bitmasks for all 6 pad rows.

### "/manta/led/slider"

Takes a color string and two integers. The first integer is the slider
ID and the second is a bitmask on the slider to be set to the given
color.

### "/manta/led/button"

Takes a color string and an integer representing the pad LED to be set.

Troubleshooting
===============

If the Manta is not recognised on your system, try to run the program with ```sudo```. If this works, it is a permission problem that can be fixed with a udev rule.

Linux (e.g. Ubuntu)
-------------------

Add a file to ```/etc/udev/rules.d``` called ```50-manta.conf``` with the following contents:
```
SUBSYSTEM=="usb", ATTR{idVendor}=="2424", ATTR{idProduct}=="2424", MODE="0666"
```

Unplug the Manta, run 
```
sudo /etc/init.d/udev restart
```

then plug in the Manta.


Arch Linux
----------

On Arch Linux, the udev system works a bit different: 

To see the used variables, attach a manta and run the following command
```
udevadm info -a -p /sys/class/hidraw/hidraw0
```

It shows all udev-related items attached to ´´´hidraw0´´´, The manta should show up here.
According to the udev system description on the [arch linux wiki](https://wiki.archlinux.org/index.php/Udev), config files have to end on ```.rules```. 
The config file therefore will be something like ```/etc/udev/rules.d/50-manta.rules```
According to the output of the ```udevadm info``` from above, its content should be something like
```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2424", ATTRS{idProduct}=="2424", MODE="0666"
```

To reload the udev rules, run
```
sudo udevadm control --reload-rules
```

Re-plug the manta and try MantaOSC again.
