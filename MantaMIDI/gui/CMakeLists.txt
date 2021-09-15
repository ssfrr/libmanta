project(MantaGUI)

cmake_minimum_required(VERSION 2.6.0)

#add /opt/local to cmake search prefix to find MacPorts-installed libs on OSX
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/local)

find_path(LO_INCLUDE_DIR lo/lo.h)
find_library(LO_LIBRARY lo)
find_path(USB-1.0_INCLUDE_DIR libusb-1.0/libusb.h)
find_library(USB-1.0_LIBRARY usb-1.0)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

link_directories(${QT_LIBRARY_DIR})
add_definitions(${QT_DEFINITIONS} -Wall)

#####################
# Handle includes
#####################

include(${QT_USE_FILE})
include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR} # QT4_WRAP_UI writes ui_whatever.h here
    ${QT_INCLUDE_DIR}
    ${USB-1.0_INCLUDE_DIR}
    ${LO_INCLUDE_DIR}
)

#SET(CMAKE_BUILD_TYPE "Debug")
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   FIND_LIBRARY(IOKIT_LIB IOKit)
   FIND_LIBRARY(COREFOUND_LIB CoreFoundation)
   SET(SYSTEM_LIBS ${IOKIT_LIB} ${COREFOUND_LIB})
   SET(HIDAPI_SRC "../../core/extern/hidapi/mac/hid.c")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET(HIDAPI_SRC "../../core/extern/hidapi/linux/hid-libusb.c")
   SET(SYSTEM_LIBS)
ENDIF()
INCLUDE_DIRECTORIES("../../core/extern/hidapi/hidapi")

##########################
# List sources
##########################
SET(Manta_Core_Srcs		# Core sources
        ../../core/Manta.cpp
        ../../core/MantaUSB.cpp
	${HIDAPI_SRC}
)

SET(Manta_RtMidi_Srcs		# RtMIDI library
        ../rtmidi/RtMidi.cpp
)

SET(MantaMidi_Srcs
        ../midimanager/MIDImanager.cpp
        ../midimanager/RtMidiManager.cpp
        ../MantaMidiSettings.cpp
        ../LayoutMgr.cpp

        ${Manta_Core_Srcs}
        ${Manta_RtMidi_Srcs}
)

# MantaGUI_SOURCES contains all the .cpp files for this project
set(MantaGUI_SOURCES
    ${MantaMidi_Srcs}
    main.cpp
    mainwindow.cpp
    mantathread.cpp
    qmantasettings.cpp
    qmantalogging.cpp
)

set(MantaGUI_MOC_HEADERS
    mainwindow.h
    mantathread.h
    qmantasettings.h
    qmantalogging.h
)

set(MantaGUI_UIS
    mainwindow.ui
)

#####################
# Qt stuff
#####################
# build the ui_xxx files from the XML-style .ui files
QT4_WRAP_UI(MantaGUI_SOURCES ${MantaGUI_UIS})

# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MantaGUI_SOURCES ${MantaGUI_MOC_HEADERS})


####################
# Compiler define flags
####################
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

# create an executable file named "qtproject" from the source files in the variable "qtproject_SRCS".
IF(UNITX)
	ADD_EXECUTABLE(MantaGUI ${MantaGUI_SOURCES})
ELSEIF(APPLE)
	ADD_EXECUTABLE(MantaGUI MACOSX_BUNDLE ${MantaGUI_SOURCES})
ELSEIF(WIN32)
	ADD_EXECUTABLE(MantaGUI WIN32 ${MantaGUI_SOURCES})
ENDIF()

############################
# RtMIDI Library additions #
############################
IF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        TARGET_LINK_LIBRARIES(MantaGUI "asound")
	TARGET_LINK_LIBRARIES(MantaGUI ${SYSTEM_LIBS})
ENDIF("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
IF(APPLE)
        TARGET_LINK_LIBRARIES(MantaGUI "-framework CoreMidi")
        TARGET_LINK_LIBRARIES(MantaGUI "-framework CoreAudio")
        TARGET_LINK_LIBRARIES(MantaGUI "-framework CoreFoundation")
	TARGET_LINK_LIBRARIES(MantaGUI ${SYSTEM_LIBS})
ENDIF(APPLE)
IF(MINGW)
        TARGET_LINK_LIBRARIES(MantaGUI "winmm.lib")
        TARGET_LINK_LIBRARIES(MantaGUI "-mwindows") # Disable console output on Windows
  	TARGET_LINK_LIBRARIES(MantaGUI ${SYSTEM_LIBS}) 
ENDIF(MINGW)

# link the "qtprojet" target against the Qt libraries. which libraries exactly is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable
target_link_libraries(MantaGUI ${QT_LIBRARIES} ${LO_LIBRARY} ${USB-1.0_LIBRARY} m)


