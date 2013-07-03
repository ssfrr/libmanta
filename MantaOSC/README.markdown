This is an example program using libmanta to build a simple OSC router,
which sends OSC mesages in response to manta touches and receives OSC
messages to control the LEDs.


This example project sometimes drifts out-of-date with the rest of the
library, so if you have trouble building please file an issue on github
issues at https://github.com/ssfrr/libmanta/issues

Prerequisites
-------------

* cmake
* liblo-dev
* libudev-dev


Build Instructions
------------------

1. git clone https://github.com/ssfrr/libmanta.git
2. cd libmanta
3. git submodule update --init
4. cd MantaOSC
5. mkdir build
6. cd build
7. cmake ..
8. make
9. You'll find an executable called MantaOSC in your current directory
