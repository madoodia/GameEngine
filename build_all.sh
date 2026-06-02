#!/bin/bash

# (C) 2026 madoodia.com
# ---------------------
# Linux Build System
# Build system for all projects

set -e

ROOT_DIR="${1:-.}"
INPUT="${2}"

echo "BUILDING All projects"

# Build Engine
echo "Building Engine..."

pushd Engine
source build.sh "$ROOT_DIR" "$INPUT"
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
	echo "Failed to build Engine"
	exit 1
fi

# Build Sandbox
echo "Building Sandbox..."

pushd Sandbox
source build.sh "$ROOT_DIR" "$INPUT"
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]; then
	echo "Failed to build Sandbox"
	exit 1
fi

echo "All projects built successfully!"
