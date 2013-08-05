cmake_minimum_required (VERSION 2.6)
# add /opt/local to cmake search prefix to find MacPorts-installed libs on OSX
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} /opt/local)
find_path(LO_INCLUDE_DIR lo/lo.h)
find_library(LO_LIBRARY lo)
include_directories(${LO_INCLUDE_DIR})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
   FIND_LIBRARY(IOKIT_LIB IOKit)
   FIND_LIBRARY(COREFOUND_LIB CoreFoundation)
   SET(SYSTEM_LIBS ${IOKIT_LIB} ${COREFOUND_LIB})
   SET(HIDAPI_SRC "../core/extern/hidapi/mac/hid.c")
ELSEIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
   SET(HIDAPI_SRC "../core/extern/hidapi/linux/hid.c")
   SET(SYSTEM_LIBS udev)
ENDIF()
INCLUDE_DIRECTORIES("../core/extern/hidapi/hidapi")

INSTALL(PROGRAMS build/MantaOSC DESTINATION bin)
add_executable(MantaOSC MantaOSC.cpp 
                        ../core/Manta.cpp
                        ../core/MantaUSB.cpp
                        ${HIDAPI_SRC})

target_link_libraries(MantaOSC ${LO_LIBRARY} ${SYSTEM_LIBS} m)
