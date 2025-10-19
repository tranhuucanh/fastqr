# Building FastQR from Source

This guide explains how to build FastQR from source, including building with custom versions of libqrencode (as required by LGPL).

## 📋 Requirements

### All Platforms

- C++14 compatible compiler (GCC 5+, Clang 3.4+, MSVC 2017+)
- CMake 3.15 or higher
- Git

### Dependencies

- **libqrencode** (version 4.0+)
- **libpng**

## 🍎 macOS

### Install Dependencies

```bash
# Using Homebrew
brew install cmake qrencode libpng pkg-config

# Or build from source (see below)
```

### Build FastQR

```bash
# Clone repository
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr

# Create build directory
mkdir build
cd build

# Configure
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local

# Build
make -j$(sysctl -n hw.ncpu)

# Test
make test

# Install
sudo make install
```

### Build with Custom libqrencode

```bash
# Build libqrencode from source
cd /tmp
git clone https://github.com/fukuchi/libqrencode.git
cd libqrencode
./autogen.sh
./configure --prefix=/usr/local
make
sudo make install

# Build FastQR
cd /path/to/fastqr
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/local
make
sudo make install
```

## 🐧 Ubuntu/Debian

### Install Dependencies

```bash
# Ubuntu 20.04+, Debian 11+
sudo apt-get update
sudo apt-get install -y \
  build-essential \
  cmake \
  pkg-config \
  libqrencode-dev \
  libpng-dev \
  git
```

### Build FastQR

```bash
# Clone repository
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr

# Create build directory
mkdir build
cd build

# Configure
cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=/usr/local

# Build
make -j$(nproc)

# Test
make test

# Install
sudo make install
```

### Build with Custom libqrencode

```bash
# Install build dependencies
sudo apt-get install -y autoconf automake libtool libpng-dev

# Build libqrencode
cd /tmp
git clone https://github.com/fukuchi/libqrencode.git
cd libqrencode
./autogen.sh
./configure --prefix=/usr/local
make
sudo make install
sudo ldconfig

# Build FastQR
cd /path/to/fastqr
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/local
make
sudo make install
```

## 🪟 Windows (MSVC)

### Install Dependencies

1. Install Visual Studio 2019 or later with C++ support
2. Install CMake from https://cmake.org/download/
3. Install vcpkg:

```powershell
cd C:\
git clone https://github.com/Microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
```

4. Install dependencies:

```powershell
.\vcpkg install qrencode:x64-windows
.\vcpkg install libpng:x64-windows
```

### Build FastQR

```powershell
# Clone repository
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr

# Create build directory
mkdir build
cd build

# Configure
cmake .. `
  -DCMAKE_TOOLCHAIN_FILE=C:\vcpkg\scripts\buildsystems\vcpkg.cmake `
  -DCMAKE_BUILD_TYPE=Release

# Build
cmake --build . --config Release

# Install
cmake --install . --prefix C:\fastqr
```

## 🔧 CMake Build Options

```bash
# Build shared library (default: static)
cmake .. -DBUILD_SHARED_LIBS=ON

# Disable examples
cmake .. -DFASTQR_BUILD_EXAMPLES=OFF

# Disable bindings
cmake .. -DFASTQR_BUILD_BINDINGS=OFF

# Custom install prefix
cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/local

# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Specify C++ compiler
cmake .. -DCMAKE_CXX_COMPILER=clang++
```

## 🧪 Running Tests

```bash
cd build

# Run all tests
make test

# Or use ctest
ctest --output-on-failure

# Run specific test
ctest -R basic_test
```

## 💎 Building Ruby Gem

```bash
cd /path/to/fastqr

# Install dependencies
gem install rake rake-compiler

# Build gem
gem build fastqr.gemspec

# Install locally
gem install fastqr-1.0.0.gem

# Test
ruby -rfastqr -e "puts FastQR.version"
```

### Building Ruby Gem with Custom Libraries

```bash
# Set library paths
export LIBRARY_PATH=/usr/local/lib:$LIBRARY_PATH
export C_INCLUDE_PATH=/usr/local/include:$C_INCLUDE_PATH
export CPLUS_INCLUDE_PATH=/usr/local/include:$CPLUS_INCLUDE_PATH

# Build gem
gem build fastqr.gemspec
```

## 📦 Building Node.js Module

```bash
cd /path/to/fastqr/bindings/nodejs

# Install dependencies
npm install

# Build native addon
npm run install

# Test
npm test

# Create tarball
npm pack
```

### Building Node.js Module with Custom Libraries

```bash
cd bindings/nodejs

# Set PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH

# Build
npm run install
```

## 🐘 Building PHP Extension

PHP uses FFI, so no compilation needed. Just ensure libfastqr is installed:

```bash
# Build and install libfastqr
cd /path/to/fastqr
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
make
sudo make install

# Test PHP binding
cd ../bindings/php
composer install
vendor/bin/phpunit
```

## 🔍 Troubleshooting

### "qrencode.h not found"

```bash
# Make sure qrencode is installed
pkg-config --modversion libqrencode

# If not found, install or add to PKG_CONFIG_PATH
export PKG_CONFIG_PATH=/usr/local/lib/pkgconfig:$PKG_CONFIG_PATH
```


### "Library not found" at runtime

```bash
# macOS
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH

# Linux
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig

# Or add to system library path
# macOS: Add to /etc/ld.so.conf
# Linux: sudo ldconfig /usr/local/lib
```

### CMake can't find libraries

```bash
# Specify library locations explicitly
cmake .. \
  -DQRENCODE_INCLUDE_DIR=/usr/local/include \
  -DQRENCODE_LIBRARY=/usr/local/lib/libqrencode.a \
  -DPNG_INCLUDE_DIR=/usr/local/include \
  -DPNG_LIBRARY=/usr/local/lib/libpng.a
```

### Ruby gem fails to build

```bash
# Install development headers
# Ubuntu/Debian:
sudo apt-get install ruby-dev

# macOS: Already included with Homebrew Ruby

# Check Ruby configuration
ruby -rrbconfig -e 'puts RbConfig::CONFIG["CC"]'
```

### Node.js module fails to build

```bash
# Install node-gyp globally
npm install -g node-gyp

# Rebuild
node-gyp rebuild

# Clean and rebuild
node-gyp clean
node-gyp configure
node-gyp build
```

## 📝 LGPL Compliance Notes

FastQR uses **LGPL 2.1** license and statically links with:
- libqrencode (LGPL 2.1)

As required by LGPL, you can:

1. **Replace the library**: Build FastQR with different versions of libqrencode using the instructions above
2. **Modify the library**: Fork and modify libqrencode, then build FastQR against your modified versions
3. **Link dynamically**: Use `-DBUILD_SHARED_LIBS=ON` to build shared library instead of static

### Example: Using Modified libqrencode

```bash
# 1. Fork and modify libqrencode
git clone https://github.com/yourfork/libqrencode.git
cd libqrencode
# Make your changes
./autogen.sh
./configure --prefix=$HOME/custom
make install

# 2. Build FastQR with your modified libqrencode
cd /path/to/fastqr
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=$HOME/custom
make
```

This satisfies LGPL requirement that users must be able to replace/modify the LGPL libraries.

## 🆘 Getting Help

If you encounter issues:

1. Check this BUILD.md
2. Check GitHub Issues: https://github.com/tranhuucanh/fastqr/issues
3. Open a new issue with:
   - Your OS and version
   - Compiler version
   - CMake output
   - Error messages

## 📚 Resources

- [CMake Documentation](https://cmake.org/documentation/)
- [libqrencode](https://github.com/fukuchi/libqrencode)
- [libpng](http://www.libpng.org/pub/png/libpng.html)
- [LGPL v2.1 License](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)

