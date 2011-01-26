cmake_minimum_required (VERSION 2.6)
project (MantaOSC)
add_executable(MantaOSC MantaOSC.cpp ../core/Manta.cpp ../core/MantaUSB.cpp)
target_link_libraries(MantaOSC lo usb-1.0)
