This is an example program using libmanta to build a simple OSC router,
which sends OSC mesages in response to manta touches and receives OSC
messages to control the LEDs.

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

