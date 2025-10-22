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
if [[ "$OS" == "linux" ]]; then
    # Linux: Use static linking but let AppImage bundle dependencies
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX="$PWD/install" \
        -DFASTQR_BUILD_EXAMPLES=OFF \
        -DCMAKE_EXE_LINKER_FLAGS="-static"
else
    # macOS: Regular static linking
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX="$PWD/install" \
        -DFASTQR_BUILD_EXAMPLES=OFF
fi

# Build
if [[ "$OS" == "macos" ]]; then
    make -j$(sysctl -n hw.ncpu)
else
    make -j$(nproc)
fi

cd ..

if [[ "$OS" == "linux" ]]; then
    echo "🔧 Building AppImage for Linux..."

    # Determine the correct AppImage tool for the architecture
    if [[ "$ARCH" == "x86_64" ]]; then
        LINUXDEPLOY_ARCH="x86_64"
    elif [[ "$ARCH" == "aarch64" ]]; then
        LINUXDEPLOY_ARCH="aarch64"
    else
        echo "❌ Unsupported Linux architecture: $ARCH"
        exit 1
    fi

    # Install AppImage tools
    wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage
    chmod +x linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage

    # Clean any existing AppDir
    rm -rf AppDir

    # Create desktop file for AppImage (simplified version)
    cat > fastqr.desktop << 'EOF'
[Desktop Entry]
Name=FastQR
Comment=Fast QR Code Generator
Exec=fastqr
Type=Application
Categories=Utility;
EOF

    # Debug: Show what we created
    echo "Desktop file contents:"
    cat fastqr.desktop
    echo "Files in current directory:"
    ls -la *.desktop 2>/dev/null || echo "No desktop files found"

    # Debug: Check the binary first
    echo "🔍 Checking binary file:"
    file build/fastqr
    ls -la build/fastqr
    echo "Binary size: $(stat -c%s build/fastqr) bytes"

    # Test if binary runs
    echo "🧪 Testing binary execution:"
    ./build/fastqr -v || echo "Binary test failed"

    # Create AppImage with desktop file and additional flags for better compatibility
    # Try different parameter order
    ./linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage \
        --executable build/fastqr \
        --desktop-file fastqr.desktop \
        --appdir AppDir \
        --output appimage \
        --library /usr/local/lib

    # Copy AppImage to output directory
    cp fastqr-${LINUXDEPLOY_ARCH}.AppImage "$OUTPUT_DIR/bin/fastqr"
    chmod +x "$OUTPUT_DIR/bin/fastqr"

    echo "✅ Built AppImage for Linux (universal compatibility!)"
else
    # macOS: Copy standalone CLI binary
    cp build/fastqr "$OUTPUT_DIR/bin/fastqr"
    echo "✅ Built standalone CLI for macOS (all static - no dependencies!)"
fi

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

