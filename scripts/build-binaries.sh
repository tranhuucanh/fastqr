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

# Configure with static linking
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=ON \
    -DCMAKE_INSTALL_PREFIX="$PWD/install" \
    -DFASTQR_BUILD_EXAMPLES=OFF

# Build
if [[ "$OS" == "macos" ]]; then
    make -j$(sysctl -n hw.ncpu)
else
    make -j$(nproc)
fi

# Install to temporary location
make install DESTDIR="$PWD/staging"

cd ..

# Copy binaries
if [[ "$OS" == "macos" ]]; then
    cp build/staging/usr/local/lib/libfastqr.*.dylib "$OUTPUT_DIR/lib/libfastqr.dylib" 2>/dev/null || \
    cp build/libfastqr.*.dylib "$OUTPUT_DIR/lib/libfastqr.dylib" 2>/dev/null || \
    cp build/libfastqr.dylib "$OUTPUT_DIR/lib/libfastqr.dylib"

    cp build/staging/usr/local/bin/fastqr "$OUTPUT_DIR/bin/fastqr" 2>/dev/null || \
    cp build/fastqr "$OUTPUT_DIR/bin/fastqr"
else
    # Copy all .so files to lib directory
    if [ -f build/staging/usr/local/lib/libfastqr.so ]; then
        cp build/staging/usr/local/lib/libfastqr.so* "$OUTPUT_DIR/lib/"
    else
        cp build/libfastqr.so* "$OUTPUT_DIR/lib/"
    fi

    # Copy binary
    if [ -f build/staging/usr/local/bin/fastqr ]; then
        cp build/staging/usr/local/bin/fastqr "$OUTPUT_DIR/bin/fastqr"
    else
        cp build/fastqr "$OUTPUT_DIR/bin/fastqr"
    fi
fi

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

