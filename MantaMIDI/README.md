# MantaMIDI

This is an example program using libmanta to build a simple MIDI device, which sends MIDI messages in response to manta touches and receives MIDI messages to control the LEDs.

On Linux and MacOS this app should create a virtual MIDI device that's accessible from any of your other music software. On Windows it needs to connect to an existing MIDI device. This could be a hardware device or a loopback virtual device that you can use to route MIDI to your other applications. [loopMIDI](https://www.tobias-erichsen.de/software/loopmidi.html) is a good choice.

## Prerequisites

* cmake
* rtmidi-dev (linux/MacOS)
* libudev-dev (linux/MacOS)


## Build Instructions

(WARNING: Linux/MacOS instructions are currently untested - please let me know if you get a chance to test them!)

### Linux and MacOS

1. git clone https://github.com/ssfrr/libmanta.git
2. cd libmanta
3. git submodule update --init
4. cd MantaMIDI
5. mkdir build
6. cd build
7. cmake ..
8. make
9. You'll find an executable called MantaMIDI in your current directory

### Windows

1. Install msys2 from http://www.msys2.org/. Follow the instructions on that page to make sure the system is up-to-date.
2. Install the mingw64 toolchain and some more necessary packages by running
`pacman -S mingw-w64-x86_64-toolchain mingw-w64-x86_64-cmake base-devel`
3. Make sure your shell is starting with the `MSYSTEM=MINGW64` environment variable set, which will set your paths to use the ming64 compiler. `which gcc` should show you `/mingw64/bin/gcc` or similar.
4. from the `MantaMIDI` directory (in this repository) create a build directory and `cd` into it, e.g. `mkdir build-win64; cd build-win64`
5. run `cmake -G"MSYS Makefiles" ..`
6. run `make`

### Build Troubleshooting:

####   file failed to open for writing (Invalid argument):

This can happen while running `cmake` on Windows. I'm not sure if it's a path length issue or a windows/unix path issue, but moving the `libmanta` project directory to within my MSYS2 home directory solved it.
