#!/bin/bash
set -e

echo "🧪 Testing Ruby Gem Locally..."
echo ""

# Get version
VERSION=$(cat VERSION)
echo "📦 Version: $VERSION"
echo ""

# Step 1: Build project (need shared library for Ruby FFI)
echo "🔨 Step 1: Building C++ library (shared)..."
rm -rf build
mkdir -p build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cd ..
echo "✅ Build complete!"
echo ""

# Step 2: Copy binaries to Ruby prebuilt directory
echo "📋 Step 2: Copying binaries to Ruby prebuilt..."
PLATFORM="macos-arm64"  # Adjust for your platform
PREBUILT_DIR="bindings/ruby/prebuilt/$PLATFORM"

mkdir -p "$PREBUILT_DIR/lib"
mkdir -p "$PREBUILT_DIR/bin"

# Copy library and CLI
cp build/libfastqr.dylib "$PREBUILT_DIR/lib/"
cp build/fastqr "$PREBUILT_DIR/bin/"

echo "✅ Copied to $PREBUILT_DIR/"
echo ""

# Step 3: Build gem
echo "🔨 Step 3: Building gem..."
gem build fastqr.gemspec
echo ""

# Step 4: Install gem locally
echo "📦 Step 4: Installing gem locally..."
gem install fastqr-${VERSION}.gem --local --force
echo ""

# Step 5: Test gem
echo "🧪 Step 5: Testing gem..."
gem install ffi --silent
ruby -e "
require 'fastqr'
puts 'FastQR version: ' + FastQR.version
puts 'Generating test QR code...'
FastQR.generate('Test Local Build', 'test_local.png', size: 300)
puts '✅ Success! Check test_local.png'
"

echo ""
echo "✅ LOCAL GEM TEST PASSED!"
echo ""
echo "Now you can safely release:"
echo "  ./scripts/release.sh <version>"

