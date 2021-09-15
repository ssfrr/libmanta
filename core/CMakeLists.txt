# Note - this build file isn't kept up to date. If you want to build
# libmanta as a shared library please file a github issue

cmake_minimum_required (VERSION 2.6)
if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g")
endif()
project (Manta)
add_library(Manta SHARED Manta.cpp MantaUSB.cpp extern/hidapi/linux/hid.c)
target_link_libraries(Manta m)
