# Installation Guide

FastQR can be installed in multiple ways depending on your needs and platform.

## 📦 Quick Install

### macOS (Homebrew)

```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

This installs the CLI tool and libraries system-wide.

### Language-Specific Packages

**Ruby (Gem):**
```bash
gem install fastqr
```

**Node.js (npm):**
```bash
npm install fastqr-pro
```

**PHP (Composer):**
```bash
composer require fastqr/fastqr
```

**All language packages include pre-built binaries - no system dependencies required!**

## 🔨 Build from Source

### Prerequisites

- C++14 compatible compiler
- CMake 3.15+
- libqrencode 4.0+
- libvips 8.10+

### macOS

```bash
# Install dependencies
brew install cmake qrencode vips

# Clone and build
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr
./build.sh

# Install
cd build
sudo make install
```

### Ubuntu/Debian

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y cmake libqrencode-dev libvips-dev build-essential

# Clone and build
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr
./build.sh

# Install
cd build
sudo make install
```

## 📥 Pre-built Binaries

Download pre-built binaries from [GitHub Releases](https://github.com/tranhuucanh/fastqr/releases):

```bash
# Download and install
curl -fsSL https://raw.githubusercontent.com/tranhuucanh/fastqr/main/scripts/install.sh | bash
```

Or manually:

```bash
# Download for your platform
wget https://github.com/tranhuucanh/fastqr/releases/download/v1.0.0/fastqr-1.0.0-macos-arm64.tar.gz

# Extract
tar xzf fastqr-1.0.0-macos-arm64.tar.gz

# Install
sudo cp -r macos-arm64/lib/* /usr/local/lib/
sudo cp -r macos-arm64/bin/* /usr/local/bin/
sudo cp -r macos-arm64/include/* /usr/local/include/

# On Linux, update library cache
sudo ldconfig
```

## 🧪 Verify Installation

### CLI Tool

```bash
fastqr --version
fastqr "Hello World" test.png
```

### Ruby

```ruby
require 'fastqr'
puts FastQR.version
FastQR.generate("Hello", "test.png")
```

### Node.js

```javascript
const fastqr = require('fastqr');
console.log(fastqr.version());
fastqr.generate('Hello', 'test.png');
```

### PHP

```php
<?php
use FastQR\FastQR;
echo FastQR::version();
FastQR::generate('Hello', 'test.png');
```

## 📚 Next Steps

- Read the [README](README.md) for usage examples
- See [BUILD.md](BUILD.md) for detailed build instructions
- Check [DISTRIBUTION.md](DISTRIBUTION.md) for publishing guides

## ❓ Troubleshooting

**"Command not found" after installation:**
```bash
# Make sure /usr/local/bin is in your PATH
echo $PATH
export PATH="/usr/local/bin:$PATH"
```

**"Library not found" error:**
```bash
# macOS
export DYLD_LIBRARY_PATH=/usr/local/lib:$DYLD_LIBRARY_PATH

# Linux
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
sudo ldconfig
```

**Gem/npm/composer packages not finding binaries:**

This should not happen as binaries are pre-built and bundled. If it does:
1. Check your platform is supported
2. Try building from source
3. Open an issue on GitHub

For more help, see [BUILD.md](BUILD.md) or open an [issue](https://github.com/tranhuucanh/fastqr/issues).

