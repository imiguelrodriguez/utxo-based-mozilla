cmake_minimum_required(VERSION 3.29)
project(certverifier)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -static")
# Set the vcpkg toolchain file path

add_executable(certverifier
    BitcoinHandler.cpp
        BitcoinHandler.h)
# Link with the Winsock library on Windows
if(WIN32)
    target_link_libraries(certverifier ws2_32)
endif()