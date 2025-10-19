#!/bin/bash
set -e

echo "🧪 Testing Node.js Package with REAL CODE..."
echo ""

# Get version
VERSION=$(cat VERSION)
echo "📦 Version: $VERSION"
echo ""

# Step 1: Build project (need shared library + CLI)
echo "🔨 Step 1: Building C++ library..."
rm -rf build
mkdir -p build
cd build
cmake -DBUILD_SHARED_LIBS=ON -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
cd ..
echo "✅ Build complete!"
echo ""

# Step 2: Copy binaries to Node.js prebuilt directory
echo "📋 Step 2: Copying binaries to Node.js prebuilt..."
PLATFORM="macos-arm64"  # Adjust for your platform
PREBUILT_DIR="bindings/nodejs/prebuilt/$PLATFORM"

mkdir -p "$PREBUILT_DIR/lib"
mkdir -p "$PREBUILT_DIR/bin"

# Copy library and CLI
cp build/libfastqr.dylib "$PREBUILT_DIR/lib/"
cp build/fastqr "$PREBUILT_DIR/bin/"

echo "✅ Copied to $PREBUILT_DIR/"
echo ""

# Step 3: Test with real Node.js code
echo "🧪 Step 3: Testing with real Node.js code..."
node -e "
const fastqr = require('./bindings/nodejs/index.js');

console.log('FastQR version:', fastqr.version());
console.log('');

// Test 1: Basic generation
console.log('Test 1: Basic QR code...');
const result1 = fastqr.generate('Hello World', 'test_node_basic.png');
console.log('✅ Result:', result1);

// Test 2: With size option
console.log('');
console.log('Test 2: QR with size 500...');
const result2 = fastqr.generate('Test Size', 'test_node_500.png', { size: 500 });
console.log('✅ Result:', result2);

// Test 3: With options
console.log('');
console.log('Test 3: QR with optimize...');
const result3 = fastqr.generate('Test Optimize', 'test_node_opt.png', { 
  size: 500, 
  optimizeSize: true 
});
console.log('✅ Result:', result3);

// Test 4: Batch mode
console.log('');
console.log('Test 4: Batch generation...');
const data = ['QR 1', 'QR 2', 'QR 3'];
const result4 = fastqr.generateBatch(data, 'test_node_batch/', { size: 300 });
console.log('✅ Result:', JSON.stringify(result4));

console.log('');
console.log('✅ All tests passed!');
"

echo ""
echo "✅ REAL NODE.JS TEST PASSED!"
echo ""

