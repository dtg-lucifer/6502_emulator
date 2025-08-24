#!/bin/bash
# Script to build an external project using the 6502 CPU emulator library

set -e

# Default values
SOURCE_FILE=""
OUTPUT_NAME=""
EMULATOR_DIR=$(dirname $(dirname $(readlink -f $0))) # Path to emulator directory
INCLUDE_DIR="${EMULATOR_DIR}/include"
LIB_DIR="${EMULATOR_DIR}/build"
NON_INTERACTIVE=0  # Interactive mode by default

# Print usage information
usage() {
    echo "Usage: $0 -s source_file [-o output_name] [-d emulator_dir] [-y]"
    echo ""
    echo "Options:"
    echo "  -s SOURCE_FILE   Source file to compile (required)"
    echo "  -o OUTPUT_NAME   Output binary name (defaults to basename of source_file)"
    echo "  -d EMULATOR_DIR  Path to emulator directory (defaults to script's parent dir)"
    echo "  -y               Non-interactive mode (answer yes to all prompts)"
    echo "  -h               Show this help message"
    echo ""
    echo "Example:"
    echo "  $0 -s my_project.cpp -o my_program"
    exit 1
}

# Process command line arguments
while getopts "s:o:d:yh" opt; do
    case ${opt} in
        s )
            SOURCE_FILE=$OPTARG
            ;;
        o )
            OUTPUT_NAME=$OPTARG
            ;;
        d )
            EMULATOR_DIR=$OPTARG
            INCLUDE_DIR="${EMULATOR_DIR}/include"
            LIB_DIR="${EMULATOR_DIR}/build"
            ;;
        y )
            NON_INTERACTIVE=1
            ;;
        h )
            usage
            ;;
        \? )
            usage
            ;;
    esac
done

# Check if source file is provided
if [ -z "$SOURCE_FILE" ]; then
    echo "Error: Source file is required"
    usage
fi

# If output name is not provided, use the basename of the source file
if [ -z "$OUTPUT_NAME" ]; then
    OUTPUT_NAME=$(basename "$SOURCE_FILE" .cpp)
fi

# Check if emulator library exists
if [ ! -f "${LIB_DIR}/libemulator_core.a" ] && [ ! -f "${LIB_DIR}/libemulator_core.so" ]; then
    echo "Error: Emulator library not found in ${LIB_DIR}"
    echo "Build the library first with: cd ${EMULATOR_DIR} && make library"
    exit 1
fi

# Inform user about build
echo "Building ${OUTPUT_NAME} from ${SOURCE_FILE}..."
echo "Using emulator library from ${EMULATOR_DIR}"

# Compile the program
g++ -o "${OUTPUT_NAME}" "${SOURCE_FILE}" \
    -I"${INCLUDE_DIR}" \
    -L"${LIB_DIR}" \
    -lemulator_core \
    -std=c++17

# Check if compilation succeeded
if [ $? -eq 0 ]; then
    echo "Build successful: ${OUTPUT_NAME}"
    echo ""
    echo "Run the program with:"
    echo "LD_LIBRARY_PATH=${LIB_DIR} ./${OUTPUT_NAME}"

    # Make the output executable
    chmod +x "${OUTPUT_NAME}"

    echo ""
    if [ $NON_INTERACTIVE -eq 1 ]; then
        echo "Running program in non-interactive mode..."
        LD_LIBRARY_PATH="${LIB_DIR}" "./${OUTPUT_NAME}"
    else
        echo "Would you like to run the program now? (y/n)"
        read -r response
        if [[ "$response" =~ ^[Yy] ]]; then
            LD_LIBRARY_PATH="${LIB_DIR}" "./${OUTPUT_NAME}"
        fi
    fi
else
    echo "Build failed."
fi
