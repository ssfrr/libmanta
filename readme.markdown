libmanta
=========

Introduction
------------

libmanta is a library and set of example programs, and a PD/Max object for
interfacing with the Snyderphonics Manta, a touch-sensitive usb controller with
LED feedback.

libmanta is currently in beta stage, and the examples are intended simply as
starting points to show how the library may be used. The [manta] object for
Pure Data and Max/MSP will be a user-facing release released in both source and
binary forms.

manta the pd/max object is built on an abstraction layer called flext by
Thomas Grill. It allows an external to be compiled for both PD and Max and on
OS X, Linux, and Windows. You will need to build and install flext using the
included instructions before building and installing the external.

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
2. Place the MaxAPI.framework folder found inside the max-includes directory
   into Library/Frameworks 
3. in Terminal, navigate to MantaFlext/extern/flext in the libmanta directory
4. './build.sh max gcc'
5. Edit buildsys/config-mac-max-gcc.txt and change:
    1. MAXSDKPATH to point to the subdirectory of the Max SDK you
       extracted in step 1 (should be the directory right above the
       max-includes directory, like MaxSDK-5.1.6/c74support/)
    2. INSTPATH to point to where your Max externals live
    3. change the ARCH= line to read only ARCH=i386
6. './build.sh max gcc'
7. Edit config.txt and comment out the line "SIMD=1"
8. './build.sh max gcc' (yes, again)
9. 'sudo ./build.sh max gcc install'
10. 'cd ../..'
11. 'extern/flext/build.sh max gcc'
12. 'extern/flext/build.sh max gcc' (again)
13. in MantaFlext/max-darwin create an empty folder called 'core'
14. 'extern/flext/build.sh max gcc' (again)
15. 'sudo extern/flext/build.sh max gcc install'
