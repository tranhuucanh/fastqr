#!/bin/bash
# Build fastqr locally for testing
# Usage: ./scripts/build-local.sh

set -e

echo "🔨 Building fastqr locally..."

# Get version
VERSION=$(cat VERSION | tr -d '\n')
echo "📦 Version: $VERSION"

# Create build directory
rm -rf build
mkdir -p build
cd build

# Configure with CMake
echo "⚙️  Configuring..."
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
echo "🔧 Building..."
cmake --build . -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

echo "✅ Build complete!"
echo "📍 Binary location: $(pwd)/fastqr"
echo ""
echo "Test it:"
echo "  ./build/fastqr --version"
echo "  ./build/fastqr 'Hello' test.png"

