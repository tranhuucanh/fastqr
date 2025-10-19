#!/bin/bash
# Build FastQR locally for testing

set -e

echo "🔨 Building FastQR locally..."

# Clean previous build
rm -rf build/
mkdir -p build

# Build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 4)

echo "✅ Build complete! Binary: build/fastqr"
echo "Test it: ./build/fastqr 'Hello World' test.png"
