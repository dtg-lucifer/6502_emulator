cmake_minimum_required(VERSION 3.10)
project(6502_cpu_emulator)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Enable debug symbols and warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

# Set global include directories (will be inherited by all targets)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

# Generate compile_commands.json for tooling (helps IDEs find symbols)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Create a library for the core emulator code
add_library(emulator_core
    src/cpu.cpp
    src/memory.cpp
    src/types.cpp
    src/reader.cpp
    src/op_codes.cpp
    programs/demo_program.cpp
    src/instructions/lda.cpp
    src/instructions/ldx.cpp
    src/instructions/ldy.cpp
    src/instructions/jsr.cpp
    src/instructions/rts.cpp
    src/instructions/sta.cpp
    src/instructions/stx.cpp
    src/instructions/sty.cpp
)

# Make includes available to any target linking against emulator_core
target_include_directories(emulator_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Add option for enabling test code
option(ENABLE_TESTING "Enable inline tests" OFF)

if(ENABLE_TESTING)
    # Define the testing macro for conditional compilation
    add_definitions(-D__TESTING)

    # Add the test executable
    add_executable(emulator_test
        tests/main.cpp
        tests/index_test.cpp
        tests/invalid_opcode_test.cpp
        tests/lda_test.cpp
        tests/ldx_test.cpp
        tests/ldy_test.cpp
        tests/jsr_test.cpp
        tests/sta_test.cpp
        tests/stx_test.cpp
        tests/sty_test.cpp
    )

    # Link the test executable with the core library
    target_link_libraries(emulator_test PRIVATE emulator_core)
else()
    # Add the main executable when not in testing mode
    add_executable(6502_cpu_emulator
        src/main.cpp
    )

    # Link the main executable with the core library
    target_link_libraries(6502_cpu_emulator PRIVATE emulator_core)
endif()

# Create a symbolic link to compile_commands.json in the source directory
# This helps many IDEs find the compilation database
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        ${CMAKE_BINARY_DIR}/compile_commands.json
        ${CMAKE_SOURCE_DIR}/compile_commands.json
    )
endif()
