project(MantaMIDI)

cmake_minimum_required (VERSION 2.6)

# add /opt/local to cmake search prefix to find MacPorts-installed libs on OSX
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/local)

find_path(LO_INCLUDE_DIR lo/lo.h)
find_library(LO_LIBRARY lo)
find_path(USB-1.0_INCLUDE_DIR libusb-1.0/libusb.h)
find_library(USB-1.0_LIBRARY usb-1.0)


#################
# List sources  #
#################
SET(Manta_Core_Srcs		# Core sources
	../core/Manta.cpp
	../core/MantaUSB.cpp
)

SET(Manta_RtMidi_Srcs		# RtMIDI library
	rtmidi/RtMidi.cpp
)

SET(MantaMidi_Srcs
	MantaMIDI.cpp
        ./midimanager/MIDImanager.cpp
        ./midimanager/RtMidiManager.cpp

	${Manta_Core_Srcs}
	${Manta_RtMidi_Srcs}
)

#########################
# Compiler Define Flags #
#########################
# RtMidi cross-platform definitions
IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux") #IF(LINUX) isn't supported by CMake yet
        ADD_DEFINITIONS(-D__LINUX_ALSASEQ__)
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
IF(APPLE)
        ADD_DEFINITIONS(-D__MACOSX_CORE__)
ENDIF(APPLE)
IF(MINGW)
        ADD_DEFINITIONS(-D__WINDOWS_MM__)
ENDIF(MINGW)

include_directories(${USB-1.0_INCLUDE_DIR} ${LO_INCLUDE_DIR} )

###############################################
ADD_EXECUTABLE(MantaMIDI ${MantaMidi_Srcs})
##############################################

############################
# RtMIDI Library additions #
############################
IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        TARGET_LINK_LIBRARIES(MantaMIDI "asound")
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
IF(APPLE)
        TARGET_LINK_LIBRARIES(MantaMIDI "-framework CoreMidi")
        TARGET_LINK_LIBRARIES(MantaMIDI "-framework CoreAudio")
        TARGET_LINK_LIBRARIES(MantaMIDI "-framework CoreFoundation")
ENDIF(APPLE)
IF(MINGW)
        TARGET_LINK_LIBRARIES(MantaMIDI "winmm.lib")
        TARGET_LINK_LIBRARIES(MantaMIDI "-mwindows") # Disable console output on Windows
ENDIF(MINGW)

target_link_libraries(MantaMIDI ${LO_LIBRARY} ${USB-1.0_LIBRARY} m)
