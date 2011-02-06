cmake_minimum_required (VERSION 2.6)

add_executable(MantaTest MantaTest.cpp ../core/Manta.cpp ../core/MantaUSB.cpp)
target_link_libraries(MantaTest usb-1.0)

add_executable(MantaUSBTest MantaUSBTest.cpp ../core/Manta.cpp ../core/MantaUSB.cpp)
target_link_libraries(MantaUSBTest usb-1.0)

add_executable(MantaUSBTimer MantaUSBTimer.cpp ../core/Manta.cpp ../core/MantaUSB.cpp)
target_link_libraries(MantaUSBTimer usb-1.0 rt)
