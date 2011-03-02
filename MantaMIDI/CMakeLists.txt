cmake_minimum_required (VERSION 2.6)
# add /opt/local to cmake search prefix to find MacPorts-installed libs on OSX
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/local)
find_path(LO_INCLUDE_DIR lo/lo.h)
find_library(LO_LIBRARY lo)
find_path(USB-1.0_INCLUDE_DIR libusb-1.0/libusb.h)
find_library(USB-1.0_LIBRARY usb-1.0)
find_library(COREAUDIO CoreAudio "/usr")
find_library(COREFOUNDATION CoreFoundation "/")
find_library(COREMIDI CoreMidi "/")

include_directories(${USB-1.0_INCLUDE_DIR} ${LO_INCLUDE_DIR})
add_executable(MantaMIDI MantaMIDI.cpp ../core/Manta.cpp ../core/MantaUSB.cpp)
target_link_libraries(MantaMIDI ${LO_LIBRARY} ${USB-1.0_LIBRARY} ${COREAUDIO} ${COREFOUNDATION} ${COREMIDI} m)
