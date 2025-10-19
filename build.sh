#!/bin/bash

# FastQR Quick Build Script
# This script automates the build process for FastQR

set -e

echo "🚀 FastQR Build Script"
echo "======================"
echo ""

# Detect OS
OS="unknown"
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
    echo "📱 Detected: macOS"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
    echo "🐧 Detected: Linux"
else
    echo "❌ Unsupported OS: $OSTYPE"
    exit 1
fi

# Check dependencies
echo ""
echo "🔍 Checking dependencies..."

if ! command -v cmake &> /dev/null; then
    echo "❌ cmake not found. Please install CMake 3.15+"
    exit 1
fi
echo "✅ cmake found: $(cmake --version | head -n1)"

if ! command -v pkg-config &> /dev/null; then
    echo "❌ pkg-config not found"
    exit 1
fi
echo "✅ pkg-config found"

if ! pkg-config --exists libqrencode; then
    echo "❌ libqrencode not found. Install with:"
    if [[ "$OS" == "macos" ]]; then
        echo "   brew install qrencode"
    else
        echo "   sudo apt-get install libqrencode-dev"
    fi
    exit 1
fi
echo "✅ libqrencode found: $(pkg-config --modversion libqrencode)"

if ! pkg-config --exists libpng; then
    echo "❌ libpng not found. Install with:"
    if [[ "$OS" == "macos" ]]; then
        echo "   brew install libpng"
    else
        echo "   sudo apt-get install libpng-dev"
    fi
    exit 1
fi
echo "✅ libpng found: $(pkg-config --modversion libpng)"

# Build
echo ""
echo "🔨 Building FastQR..."

# Create build directory
BUILD_DIR="build"
if [ -d "$BUILD_DIR" ]; then
    echo "📁 Cleaning old build directory..."
    rm -rf "$BUILD_DIR"
fi

mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Configure
echo "⚙️  Configuring..."
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr/local \
    -DFASTQR_BUILD_EXAMPLES=ON

# Build
echo "🔧 Compiling..."
if [[ "$OS" == "macos" ]]; then
    NCPU=$(sysctl -n hw.ncpu)
else
    NCPU=$(nproc)
fi

make -j$NCPU

# Test
echo ""
echo "🧪 Running tests..."
make test

echo ""
echo "✅ Build completed successfully!"
echo ""
echo "📦 To install system-wide, run:"
echo "   cd $BUILD_DIR && sudo make install"
echo ""
echo "🎯 To test the CLI:"
echo "   ./fastqr \"Hello World\" test.png"
echo ""
echo "💡 See README.md for usage examples"

