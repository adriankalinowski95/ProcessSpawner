#!/usr/bin/env bash

set -e

CURRENT_DIR="$PWD"

if [ -z "build" ]; then
  echo "=== Step 0. Build folder doesn't exist - create new ==="
  
  mkdir -p "$CURRENT_DIR/build"
else 
  rm -rf "$CURRENT_DIR/build"
  mkdir -p "$CURRENT_DIR/build"
fi

echo "=== Step 1: Install conan ==="

conan install "$CURRENT_DIR" --output="$CURRENT_DIR" --build=missing

echo "=== Step 2: Cmake build ==="

cmake -B "$CURRENT_DIR/build" -DCMAKE_EXPORT_COMPILE=ON -DCMAKE_TOOLCHAIN_FILE="$CURRENT_DIR/build/Debug/generators/conan_toolchain.cmake"

echo "=== Step 3: Compile ==="

make -C "$CURRENT_DIR/build"