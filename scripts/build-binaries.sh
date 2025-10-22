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
    # Linux: Use dynamic linking, let AppImage bundle all dependencies
    cmake .. \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_SHARED_LIBS=OFF \
        -DCMAKE_INSTALL_PREFIX="$PWD/install" \
        -DFASTQR_BUILD_EXAMPLES=OFF
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

    # Install AppImage tools - try stable version instead of continuous
    echo "📥 Downloading linuxdeploy stable version..."
    wget -q https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage
    chmod +x linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage

    # Also try appimagetool as fallback
    echo "📥 Downloading appimagetool as fallback..."
    wget -q https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-${LINUXDEPLOY_ARCH}.AppImage
    chmod +x appimagetool-${LINUXDEPLOY_ARCH}.AppImage

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

    # Check binary dependencies
    echo "📋 Binary dependencies:"
    ldd build/fastqr || echo "ldd failed"

    # Check binary headers
    echo "📋 Binary headers:"
    readelf -h build/fastqr || echo "readelf failed"

    # Check first few bytes
    echo "📋 First 32 bytes (hex):"
    hexdump -C build/fastqr | head -2

    # Test if binary runs
    echo "🧪 Testing binary execution:"
    ./build/fastqr -v || echo "Binary test failed"

    # Check linuxdeploy version and capabilities
    echo "🔍 linuxdeploy info:"
    ./linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage --help | head -10

    # Create AppImage with desktop file and additional flags for better compatibility
    # Bundle all necessary libraries to avoid GLIBC conflicts
    echo "🔧 Trying linuxdeploy method..."
    if ./linuxdeploy-${LINUXDEPLOY_ARCH}.AppImage \
        --executable build/fastqr \
        --desktop-file fastqr.desktop \
        --appdir AppDir \
        --output appimage \
        --library /usr/local/lib \
        --library /usr/lib/x86_64-linux-gnu \
        --library /lib/x86_64-linux-gnu; then
        echo "✅ linuxdeploy succeeded"
    else
        echo "❌ linuxdeploy failed, trying manual AppImage creation..."

        # Manual AppImage creation as fallback
        mkdir -p AppDir/usr/bin
        cp build/fastqr AppDir/usr/bin/
        chmod +x AppDir/usr/bin/fastqr

        # Copy desktop file
        cp fastqr.desktop AppDir/usr/share/applications/

        # Copy dependencies manually
        mkdir -p AppDir/usr/lib
        echo "📋 Copying dependencies..."
        ldd build/fastqr | grep "=>" | awk '{print $3}' | while read libpath; do
            if [ -f "$libpath" ]; then
                echo "  Copying: $libpath"
                cp "$libpath" AppDir/usr/lib/
            fi
        done
        
        # Also copy specific libraries we know we need
        echo "📋 Copying specific libraries..."
        for lib in libqrencode.so libpng.so libz.so; do
            if [ -f "/usr/local/lib/$lib" ]; then
                echo "  Copying: /usr/local/lib/$lib"
                cp "/usr/local/lib/$lib" AppDir/usr/lib/
            elif [ -f "/usr/lib/x86_64-linux-gnu/$lib" ]; then
                echo "  Copying: /usr/lib/x86_64-linux-gnu/$lib"
                cp "/usr/lib/x86_64-linux-gnu/$lib" AppDir/usr/lib/
            fi
        done

        # Create AppRun
        cat > AppDir/AppRun << 'EOF'
#!/bin/bash
HERE="$(dirname "$(readlink -f "${0}")")"
export LD_LIBRARY_PATH="${HERE}/usr/lib:${LD_LIBRARY_PATH}"
exec "${HERE}/usr/bin/fastqr" "$@"
EOF
        chmod +x AppDir/AppRun

        # Extract appimagetool and use it directly (no FUSE needed)
        echo "🔧 Extracting appimagetool..."
        ./appimagetool-${LINUXDEPLOY_ARCH}.AppImage --appimage-extract
        chmod +x squashfs-root/AppRun
        
        # Use extracted appimagetool
        echo "🔧 Creating AppImage with extracted appimagetool..."
        ./squashfs-root/AppRun AppDir fastqr-${LINUXDEPLOY_ARCH}.AppImage
    fi

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

