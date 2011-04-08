project(MantaGUI)

cmake_minimum_required(VERSION 2.6.0)

find_package(Qt4 REQUIRED) # find and setup Qt4 for this project

link_directories(${QT_LIBRARY_DIR})
add_definitions(${QT_DEFINITIONS} -Wall)

# MantaGUI_SOURCES contains all the .cpp files for this project
set(MantaGUI_SOURCES
	main.cpp
	mainwindow.cpp
)

set(MantaGUI_MOC_HEADERS
    mainwindow.h
)

set(MantaGUI_UIS
	mainwindow.ui
)

include(${QT_USE_FILE})
include_directories(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_CURRENT_SOURCE_DIR} 
	${CMAKE_CURRENT_BINARY_DIR} # QT4_WRAP_UI writes ui_whatever.h here
	${QT_INCLUDE_DIR} 
)

# build the ui_xxx files from the XML-style .ui files
QT4_WRAP_UI(MantaGUI_SOURCES ${MantaGUI_UIS})

# this moc's the above variable and appends to the cxx sources
QT4_WRAP_CPP(MantaGUI_SOURCES ${MantaGUI_MOC_HEADERS})

# create an executable file named "qtproject" from the source files in the variable "qtproject_SRCS".
IF(UNITX)
	ADD_EXECUTABLE(MantaGUI ${MantaGUI_SOURCES})
ELSEIF(APPLE)
	ADD_EXECUTABLE(MantaGUI MACOSX_BUNDLE ${MantaGUI_SOURCES})
ELSEIF(WIN32)
	ADD_EXECUTABLE(MantaGUI WIN32 ${MantaGUI_SOURCES})
ENDIF()


# link the "qtprojet" target against the Qt libraries. which libraries exactly is defined by the "include(${QT_USE_FILE})" line above, which sets up this variable
target_link_libraries(MantaGUI ${QT_LIBRARIES})


