CMAKE_MINIMUM_REQUIRED (VERSION 2.6)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   FIND_LIBRARY(IOKIT_LIB IOKit)
   FIND_LIBRARY(COREFOUND_LIB CoreFoundation)
   SET(SYSTEM_LIBS ${IOKIT_LIB} ${COREFOUND_LIB})
   SET(HIDAPI_SRC "../core/extern/hidapi/mac/hid.c")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET(HIDAPI_SRC "../core/extern/hidapi/linux/hid-libusb.c")
   SET(SYSTEM_LIBS)
ENDIF()
INCLUDE_DIRECTORIES("../core/extern/hidapi/hidapi")

ADD_EXECUTABLE(MantaTest MantaTest.cpp
                        ../core/Manta.cpp
                        ../core/MantaUSB.cpp
                        ${HIDAPI_SRC})
TARGET_LINK_LIBRARIES(MantaTest ${SYSTEM_LIBS})

ADD_EXECUTABLE(MantaUSBTest MantaUSBTest.cpp
                        ../core/MantaUSB.cpp
                        ${HIDAPI_SRC})
TARGET_LINK_LIBRARIES(MantaUSBTest ${SYSTEM_LIBS})

ADD_EXECUTABLE(MantaUSBTimer MantaUSBTimer.cpp
                        ../core/Manta.cpp
                        ../core/MantaUSB.cpp
                        ${HIDAPI_SRC})
TARGET_LINK_LIBRARIES(MantaUSBTimer ${SYSTEM_LIBS})
