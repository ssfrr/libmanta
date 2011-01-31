cmake_minimum_required (VERSION 2.6)
project (Manta)
add_library(Manta SHARED Manta.cpp MantaUSB.cpp)
target_link_libraries(Manta usb-1.0)
