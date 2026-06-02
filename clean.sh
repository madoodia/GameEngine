#!/bin/bash

# (C) 2026 madoodia.com
# ---------------------
# Linux Build System
# Clean build artifacts

set -e

ROOT_DIR="${1:-.}"

echo "Cleaning the Project..."

if [ -d "$ROOT_DIR/Bin" ]; then
	rm -rf "$ROOT_DIR/Bin"
fi

echo "Clean completed successfully!"
