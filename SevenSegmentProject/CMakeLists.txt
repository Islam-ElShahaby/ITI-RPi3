cmake_minimum_required(VERSION 3.10)

# -----------------------------------------------------------------------------
# 1. Cross-Compilation Setup (MUST BE BEFORE 'project')
# -----------------------------------------------------------------------------
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# Define the path to your cross-compiler
set(TOOLCHAIN_ROOT "$ENV{HOME}/x-tools/aarch64-rpi3-linux-gnu")

# Force CMake to use your specific compilers
set(CMAKE_C_COMPILER   "${TOOLCHAIN_ROOT}/bin/aarch64-rpi3-linux-gnu-gcc")
set(CMAKE_CXX_COMPILER "${TOOLCHAIN_ROOT}/bin/aarch64-rpi3-linux-gnu-g++")

# Adjust search behaviors to look in the toolchain location, not your PC
set(CMAKE_FIND_ROOT_PATH "${TOOLCHAIN_ROOT}")
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# -----------------------------------------------------------------------------
# 2. Project Initialization
# -----------------------------------------------------------------------------
project(SevenSegmentProject)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# -----------------------------------------------------------------------------
# 3. Create the Shared Library
# -----------------------------------------------------------------------------
# This library contains all your logic classes.
# NOTE: Ensure 'src/pi_gpio.cpp' matches your actual filename (case-sensitive!)
add_library(SevenSegmentLib SHARED
    src/Stream.cpp
    src/IStream.cpp
    src/OStream.cpp
    src/SevenSegment.cpp
    src/pi_gpio.cpp      
)

# Tell the library where to find its header files (the "include" folder)
target_include_directories(SevenSegmentLib PUBLIC 
    ${CMAKE_SOURCE_DIR}/include
)

# -----------------------------------------------------------------------------
# 4. Create the Executable
# -----------------------------------------------------------------------------
add_executable(SevenSegmentApp
    app/main.cpp
)

# Link the executable to your shared library
target_link_libraries(SevenSegmentApp 
    PRIVATE 
    SevenSegmentLib
)