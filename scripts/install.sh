#!/bin/bash

# FastQR - Install pre-built binaries script
# This script downloads and installs pre-built binaries for your platform

set -e

echo "📦 FastQR Binary Installer"
echo "=========================="
echo ""

# Detect platform
OS="unknown"
ARCH=$(uname -m)
if [[ "$OSTYPE" == "darwin"* ]]; then
    OS="macos"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    OS="linux"
else
    echo "❌ Unsupported OS: $OSTYPE"
    exit 1
fi

# Normalize arch
case "$ARCH" in
    x86_64|amd64)
        ARCH="x86_64"
        ;;
    aarch64|arm64)
        ARCH="arm64"
        ;;
esac

PLATFORM="$OS-$ARCH"
echo "Platform: $PLATFORM"

# Get version
VERSION="${1:-1.0.0}"
echo "Version: $VERSION"
echo ""

# Download URL
TARBALL="fastqr-$VERSION-$PLATFORM.tar.gz"
URL="https://github.com/tranhuucanh/fastqr/releases/download/v$VERSION/$TARBALL"

echo "📥 Downloading $TARBALL..."
if command -v curl &> /dev/null; then
    curl -L -o "/tmp/$TARBALL" "$URL"
elif command -v wget &> /dev/null; then
    wget -O "/tmp/$TARBALL" "$URL"
else
    echo "❌ Neither curl nor wget found. Please install one of them."
    exit 1
fi

echo "📦 Extracting..."
cd /tmp
tar xzf "$TARBALL"

echo "🔧 Installing..."
INSTALL_DIR="${INSTALL_DIR:-/usr/local}"

if [[ -w "$INSTALL_DIR" ]]; then
    cp -r "$PLATFORM/lib/"* "$INSTALL_DIR/lib/"
    cp -r "$PLATFORM/bin/"* "$INSTALL_DIR/bin/"
    cp -r "$PLATFORM/include/"* "$INSTALL_DIR/include/"
else
    sudo cp -r "$PLATFORM/lib/"* "$INSTALL_DIR/lib/"
    sudo cp -r "$PLATFORM/bin/"* "$INSTALL_DIR/bin/"
    sudo cp -r "$PLATFORM/include/"* "$INSTALL_DIR/include/"
fi

# Update library cache on Linux
if [[ "$OS" == "linux" ]]; then
    sudo ldconfig
fi

# Cleanup
rm -rf "/tmp/$TARBALL" "/tmp/$PLATFORM"

echo ""
echo "✅ FastQR installed successfully!"
echo ""
echo "Test it:"
echo "  fastqr --version"
echo "  fastqr \"Hello World\" test.png"

