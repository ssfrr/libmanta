libmanta
=========

Introduction
------------

libmanta is a library and set of example programs, and a PD/Max object for
interfacing with the Snyderphonics Manta, a touch-sensitive usb controller with
LED feedback.

libmanta is intended for use by developers to build applications that talk to
the Manta. The [manta] object for Pure Data and Max/MSP are user-facing
release released in both source and binary forms.

[manta] the pd/max object is built on an abstraction layer called flext by
Thomas Grill. It allows an external to be compiled for both PD and Max and on
OS X, Linux, and Windows. You will need to build and install flext using the
included instructions before building and installing the external.

MantaOSC is a simple command-line application that allows you to interface with your manta using Open Sound Control (OSC). See the [MantaOSC Readme](https://github.com/ssfrr/libmanta/blob/master/MantaOSC/README.markdown) for more details on building and running it.

Documentation
-------------

If you'd like to develop an application using libmanta, there's developer
documentation available at http://ssfrr.github.io/libmanta/.

Prerequisites
-------------

liblo (only for MantaOSC example)
CMake (to build standalone library or non-flext examples)
flext (for the PD/Max object) is included with this source package.
   If you have another version installed feel free to use that instead, and
   modify the instructions accordingly.

PureData / Max/MSP Object Build and Install Instructions
--------------------------------------------------------

Note: commands to be typed into the terminal will appear in single quotes with
no other instruction.

### PureData on Linux or OS X

1. Download the pd source from http://puredata.info/downloads and remember
   where you extract it.
2. Navigate to extern/flext in the libmanta directory
3. './build.sh pd gcc'
4. Edit buildsys/config-lnx-pd-gcc.txt (or config-mac-pd-gcc.txt) and change:
    1. (Linux ONLY) the last line to OFLAGS+=-march=native
    2. PDPATH to point to the "pd" subdirectory of the pd source package you
       extracted in step 2
    3. INSTPATH to point to where your PD externals live
5. './build.sh pd gcc'
6. (Mac Only) Edit config.txt and comment out the line "SIMD=1"
7. './build.sh pd gcc' (yes, again)
8. './build.sh pd gcc install'
9. 'cd ../..'
10. 'extern/flext/build.sh pd gcc'
11. 'sudo extern/flext/build.sh pd gcc install'



### Max/MSP on OS X

1. Download Max/MSP SDK from Cycling74.com and put it somewhere easy to find
2. Place the MaxAPI.framework folder found in source/c74support/max-includes
   into /Library/Frameworks
3. in Terminal make sure you have xcode command line tools installed by typing 'xcode-select --install'
4. in Terminal, navigate to MantaFlext/extern/flext in the libmanta directory
5. './build.sh max gcc'   (it will say Error 1, but will create a config file called "config-mac-max-gcc.txt")
6. Edit buildsys/config-mac-max-gcc.txt and change:
    1. MAXSDKPATH to point to the subdirectory of the Max SDK you
       extracted in step 1 (should be the directory right above the
       max-includes directory, like MaxSDK-5.1.6/c74support/)
    2. INSTPATH to point to where your Max externals live
    3. change the ARCH= line to read only ARCH=i386 x86_64
    4. change the "LDFLAGS" line to read: LDFLAGS += -mmacosx-version-min=10.6 $(shell cat $(MAXSDKPATH)/max-includes/c74_linker_flags.txt)
7. './build.sh max gcc' (it will once again say Error 1, but has created a different config file called "config.txt")
8. Edit config.txt and comment out the line "SIMD=1"
9. './build.sh max gcc' (yes, again)
10. 'sudo ./build.sh max gcc install'
11. Open the included xcode project `manta.xcodeproj`
12. In the Build Settings tab set the variable `maxsdk` to be the c74support
    folder inside the Max/MSP SDK
13. Build the project
14. copy the manta.mxo object from the Products folder on the left into your
    Max externals directory


### Troubleshooting FAQ

#### ld: framework not found vecLib

It looks like that name has been deprecated for some time and now doesn't work
any more. Change vecLib to Accelerate in buildsys/mac/gnumake-gcc.inc

#### No rule to make target '../core/extern/hidapi/hidapi/hidapi.h'

hidapi is included as a git submodule. Go to the root of the libmanta
repository and run:

```
git submodule update --init
```

#### error: unable to open output file 'max-darwin/release-multi/../core/extern/hidapi/mac/hid.o_i386'

Due to some weird path handling you need to run:
`mkdir -p max-darwin/core/extern/hidapi/mac` to manually
create the right destination directory


#### Can't connect to Manta

If the Manta is not recognised on your system, try to run the program with `sudo`. If this works, it is a permission problem that can be fixed with a udev rule.

Linux (e.g. Ubuntu)
-------------------

Copy the file `99-manta.rules` from `platform/linux` into your system's `/etc/udev/rules.d` directory.

Unplug the Manta, run `sudo /etc/init.d/udev restart`, then plug in the Manta.

If that doesn't work, try editing the file and changing `ATTRS` to `ATTR` and/or try changing `MODE:="0666"` to `MODE="0666"`.

Arch Linux
----------

On Arch Linux, the udev system works a bit differently. To see the used variables, attach a manta and run the following command

```
udevadm info -a -p /sys/class/hidraw/hidraw0
```

It shows all udev-related items attached to ´´´hidraw0´´´, The manta should show up here. According to the udev system description on the [arch linux wiki](https://wiki.archlinux.org/index.php/Udev), config files have to end on `.rules`. The config file therefore will be something like `/etc/udev/rules.d/50-manta.rules`. According to the output of the `udevadm info` from above, its content should be something like

```
SUBSYSTEMS=="usb", ATTRS{idVendor}=="2424", ATTRS{idProduct}=="2424", MODE="0666"
```

To reload the udev rules, run
```
sudo udevadm control --reload-rules
```

Re-plug the manta and re-run your libmanta-based application.
