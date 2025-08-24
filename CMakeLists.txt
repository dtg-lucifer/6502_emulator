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
    src/emulator_api.cpp
    programs/demo_program.cpp
    src/instructions/lda.cpp
    src/instructions/ldx.cpp
    src/instructions/ldy.cpp
    src/instructions/jsr.cpp
    src/instructions/rts.cpp
    src/instructions/sta.cpp
    src/instructions/stx.cpp
    src/instructions/sty.cpp
    src/instructions/jmp.cpp
    src/instructions/pla.cpp
    src/instructions/plp.cpp
    src/instructions/php.cpp
    src/instructions/pha.cpp
    src/instructions/txs.cpp
    src/instructions/tsx.cpp
)

# Make includes available to any target linking against emulator_core
target_include_directories(emulator_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Set property for building shared libraries
set_property(TARGET emulator_core PROPERTY POSITION_INDEPENDENT_CODE ON)

# Create shared and static library versions
add_library(emulator_core_shared SHARED $<TARGET_OBJECTS:emulator_core>)
set_target_properties(emulator_core_shared PROPERTIES
    OUTPUT_NAME "emulator_core"
    VERSION 1.0.0
    SOVERSION 1
)

# Make includes available to any target linking against emulator_core_shared
target_include_directories(emulator_core_shared PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Install the libraries and headers
install(TARGETS emulator_core emulator_core_shared
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION include/6502_cpu_emulator
    FILES_MATCHING PATTERN "*.h"
)

# Add a custom target for generating pkg-config file
set(PKG_CONFIG_FILE "${CMAKE_BINARY_DIR}/emulator_core.pc")
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/emulator_core.pc.in"
    "${PKG_CONFIG_FILE}"
    @ONLY
)
install(FILES "${PKG_CONFIG_FILE}"
    DESTINATION lib/pkgconfig
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
        tests/jmp_test.cpp
        tests/sta_test.cpp
        tests/stx_test.cpp
        tests/sty_test.cpp
        tests/stack_operations_test.cpp
        tests/and_test.cpp
        tests/eor_test.cpp
        tests/ora_test.cpp
        tests/status_flag_test.cpp
        tests/bit_manipulation_test.cpp
        tests/shifts_rotates_test.cpp
        tests/branch_test.cpp
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

    # Add an example API usage executable
    add_executable(emulator_api_example
        examples/api_example.cpp
    )

    # Link with the shared library
    target_link_libraries(emulator_api_example PRIVATE emulator_core_shared)

    # Add binary file creator utility
    add_executable(create_test_binary
        examples/create_test_binary.cpp
    )

    # Link with the core library
    target_link_libraries(create_test_binary PRIVATE emulator_core)
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

# Add custom target for doc generation
add_custom_target(docs
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/docs
    COMMAND cp ${CMAKE_SOURCE_DIR}/API_USAGE.md ${CMAKE_BINARY_DIR}/docs/
    COMMENT "Generating documentation in ${CMAKE_BINARY_DIR}/docs"
)
