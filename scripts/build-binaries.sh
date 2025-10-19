#!/bin/bash

# FastQR - Build Pre-compiled Binaries
# This script builds binaries for multiple platforms

set -e

echo "🔨 FastQR Binary Builder"
echo "========================"
echo ""

# Get version from CMakeLists.txt
VERSION=$(grep "project(fastqr VERSION" CMakeLists.txt | sed 's/.*VERSION \([0-9.]*\).*/\1/')
echo "Version: $VERSION"

# Detect platform
OS="unknown"
ARCH=$(uname -m)
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
fi

echo "Platform: $OS-$ARCH"
echo ""

# Output directory
OUTPUT_DIR="prebuilt/$OS-$ARCH"
mkdir -p "$OUTPUT_DIR/lib"
mkdir -p "$OUTPUT_DIR/bin"

echo "🔧 Building for $OS-$ARCH..."

# Clean and build
rm -rf build
mkdir build
cd build

echo "🔧 Building standalone CLI with static linking..."

# Set PKG_CONFIG_PATH to find custom-built libraries
if [[ "$OS" == "linux" ]]; then
    export PKG_CONFIG_PATH="/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH"
    echo "PKG_CONFIG_PATH: $PKG_CONFIG_PATH"
fi

# Configure for standalone CLI (all dependencies static)
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF \
    -DCMAKE_INSTALL_PREFIX="$PWD/install" \
    -DFASTQR_BUILD_EXAMPLES=OFF

# Build
if [[ "$OS" == "macos" ]]; then
    make -j$(sysctl -n hw.ncpu)
else
    make -j$(nproc)
fi

cd ..

# Copy standalone CLI binary (no dylib needed!)
cp build/fastqr "$OUTPUT_DIR/bin/fastqr"
echo "✅ Built standalone CLI (all static - no dependencies!)"

# No shared library needed - CLI is standalone!
# (Ruby and Node.js use CLI binary directly)

# Copy headers
cp -r include "$OUTPUT_DIR/"

# Create tarball
cd prebuilt
tar czf "fastqr-$VERSION-$OS-$ARCH.tar.gz" "$OS-$ARCH"
cd ..

echo ""
echo "✅ Binary built successfully!"
echo "📦 Output: prebuilt/fastqr-$VERSION-$OS-$ARCH.tar.gz"
echo ""
echo "Contents:"
ls -lh "$OUTPUT_DIR/lib/"
ls -lh "$OUTPUT_DIR/bin/"

