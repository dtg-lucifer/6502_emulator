cmake_minimum_required(VERSION 3.10)
project(6502_cpu_emulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable debug symbols and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Include header files
include_directories(include)

include(cmake/UpdateSubmodules.cmake)

include(CTest)
add_subdirectory(external)
add_subdirectory(test)

# Add the main executable
add_executable(6502_cpu_emulator
    src/main.cpp
    src/cpu.cpp
    src/memory.cpp
)

# Generate compile_commands.json for tooling
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
