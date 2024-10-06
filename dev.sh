#!/bin/bash

# Exit script if any command fails
set -e

# Set default build directory if not specified
BUILD_DIR="build"

# Detect the operating system
OS=$(uname)

# Create the build directory if it doesn't exist
if [ ! -d "$BUILD_DIR" ]; then
  echo "Build directory $BUILD_DIR does not exist. Creating it now..."
  mkdir "$BUILD_DIR"
fi

# Change to the build directory
cd "$BUILD_DIR"

# Run CMake to configure the build system if necessary
if [ ! -f "../CMakeLists.txt" ]; then
    echo "CMakeLists.txt not found. Please ensure you're in the correct project directory."
    exit 1
fi

# Platform-specific behavior (if needed)
if [[ "$OS" == "Darwin" ]]; then
    echo "Running on macOS"
elif [[ "$OS" == "Linux" ]]; then
    echo "Running on Linux"
else
    echo "Unsupported operating system: $OS"
    exit 1
fi

# Run CMake to configure and build the project using Ninja
cmake -G Ninja ..
ninja -v  # Verbose build process

# Run the compiled binary
if [[ "$OS" == "Darwin" || "$OS" == "Linux" ]]; then
    ./main
else
    echo "Unsupported platform for running the binary."
    exit 1
fi
