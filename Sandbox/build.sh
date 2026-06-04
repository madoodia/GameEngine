#!/bin/bash

# (C) 2026 madoodia.com
# ---------------------

# Linux Build System
# Build system for the Sandbox

set -e

ROOT_DIR="${1:-.}"
INPUT="${2}"

# Set the build configuration (Debug/Release)
BUILD_CONFIG="Debug"
BUILD_FLAG="_DEBUG"

COMPILER_FLAGS="-g -fdeclspec -fPIC"

if [ "$INPUT" = "1" ]; then
	BUILD_CONFIG="Release"
	BUILD_FLAG="_RELEASE"
	COMPILER_FLAGS="-fdeclspec -fPIC"
fi

# Get the list of .cpp files in the Source directory
SOURCE_FILES=$(find . -type f -name "*.cpp")

if [ ! -d "$ROOT_DIR/Bin/$BUILD_CONFIG" ]; then
	mkdir -p "$ROOT_DIR/Bin/$BUILD_CONFIG"
fi

PROJECT="Sandbox"
INCLUDE_FLAGS="-ISource -I$ROOT_DIR/Engine/Source"
LIBRARY_FLAGS="-L$ROOT_DIR/Bin/$BUILD_CONFIG -lEngine -Wl,-rpath,$ROOT_DIR/Bin/$BUILD_CONFIG"
DEFINES="-D${BUILD_FLAG} -DGEIMPORT"

echo "BUILDING $PROJECT with configuration: $BUILD_CONFIG"
clang++ $SOURCE_FILES $COMPILER_FLAGS $INCLUDE_FLAGS $LIBRARY_FLAGS $DEFINES -o "$ROOT_DIR/Bin/$BUILD_CONFIG/$PROJECT"

echo "Build completed successfully!"
