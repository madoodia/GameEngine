#!/bin/bash

# (C) 2026 madoodia.com
# ---------------------
# Linux Build System
# Build system for the Engine

set -e

ROOT_DIR="${1:-.}"
INPUT="${2}"

# Set the build configuration (Debug/Release)
BUILD_CONFIG="Debug"
BUILD_FLAG="_DEBUG"

COMPILER_FLAGS="-g -shared -fdeclspec -fPIC"

if [ "$INPUT" = "1" ]; then
	BUILD_CONFIG="Release"
	BUILD_FLAG="_RELEASE"
	COMPILER_FLAGS="-shared -fdeclspec -fPIC"
fi

# Get the list of .cpp files in the Source directory
SOURCE_FILES=$(find . -type f -name "*.cpp")

if [ ! -d "$ROOT_DIR/Bin/$BUILD_CONFIG" ]; then
	mkdir -p "$ROOT_DIR/Bin/$BUILD_CONFIG"
fi

PROJECT="Engine"
INCLUDE_FLAGS="-ISource -I${VULKAN_SDK}/include"
LIBRARY_FLAGS="-lvulkan -lxcb -lX11 -lxcb -lxkbcommon-x11 -L${VULKAN_SDK}/lib"
DEFINES="-D${BUILD_FLAG} -DGEEXPORT"

echo "BUILDING $PROJECT with configuration: $BUILD_CONFIG"
clang++ $SOURCE_FILES $COMPILER_FLAGS $INCLUDE_FLAGS $LIBRARY_FLAGS $DEFINES -o "$ROOT_DIR/Bin/$BUILD_CONFIG/lib$PROJECT.so"

echo "Build completed successfully!"

