# FastQR

[![License](https://img.shields.io/badge/License-LGPL%202.1-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-14-blue.svg)](https://isocpp.org/)
[![Ruby](https://img.shields.io/badge/Ruby-Gem-red.svg)](https://rubygems.org/gems/fastqr)
[![Node.js](https://img.shields.io/badge/Node.js-npm-green.svg)](https://www.npmjs.com/package/fastqr)
[![PHP](https://img.shields.io/badge/PHP-Composer-blue.svg)](https://packagist.org/packages/fastqr/fastqr)

FastQR is a fast, powerful QR code generator with full UTF-8 support, custom colors, logo embedding, and precise size control. **Pre-built binaries included**!

## ✨ Features

- 🚀 **High Performance**: No process forking
- ⚡ **Batch Mode**: Generate 1000 QR codes in ~0.4s (7x faster than single mode!)
- 🌐 **Full UTF-8 Support**: Vietnamese, Japanese (Kanji, Hiragana, Katakana), Chinese, emoji, etc.
- 🎨 **Custom Colors**: Choose colors for QR code and background
- 📐 **Exact Size**: Generate QR codes with precise pixel dimensions (e.g., 2000x2000px)
- 🖼️ **Logo Embedding**: Add company logos to the center of QR codes
- 📦 **Multiple Languages**: Bindings for Ruby, Node.js, PHP
- 🛡️ **Error Correction**: Supports 4 levels (L, M, Q, H)
- 💾 **Multiple Formats**: PNG, JPG, WebP
- ✅ **Pre-built Binaries**: No dependencies needed for gem/npm/composer packages!

## 🎯 Comparison with qrencode

| Feature | FastQR | qrencode |
|---------|--------|----------|
| **Speed** | ✅ The same | ✅ The same |
| **Exact Size** | ✅ 2000x2000px exact | ❌ Scale-based (hard to be exact) |
| **UTF-8 Support** | ✅ Full | ⚠️ Limited |
| **Colors** | ✅ RGB customizable | ❌ Black only |
| **Logo** | ✅ Yes | ❌ No |
| **Bindings** | ✅ Ruby, Node.js, PHP | ❌ CLI only |
| **Installation** | ✅ Pre-built binaries | ❌ Requires system deps |

## 🏗️ Architecture

FastQR is built on:

- **[libqrencode](https://fukuchi.org/works/qrencode/)** (LGPL v2.1) - QR code generation
- **[libpng](http://www.libpng.org/pub/png/libpng.html)** - Fast PNG encoding
- **[stb_image](https://github.com/nothings/stb)** (Public Domain) - Logo image loading

## 📦 Installation

### macOS (Homebrew)

```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

### Ubuntu/Debian

```bash
# Download from GitHub Releases
wget https://github.com/tranhuucanh/fastqr/releases/download/v1.0.0/fastqr-1.0.0-linux-x64.deb
sudo dpkg -i fastqr-1.0.0-linux-x64.deb
```

### Language Packages (Pre-built Binaries Included!)

**Ruby:**
```bash
gem install fastqr
# No system dependencies needed! 🎉
```

**Node.js:**
```bash
npm install fastqr
# No system dependencies needed! 🎉
```

**PHP:**
```bash
composer require fastqr/fastqr
# No system dependencies needed! 🎉
```

**Important**: Starting from v1.0.0, all language packages include pre-built binaries for:
- macOS (Intel & Apple Silicon)
- Linux (x86_64 & arm64)

You don't need to install `libqrencode` or `libpng` separately! The binaries are automatically bundled and loaded.

### Build from Source

```bash
# Install dependencies
# macOS:
brew install qrencode libpng cmake

# Ubuntu/Debian:
sudo apt-get install libqrencode-dev libpng-dev cmake build-essential

# Build
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr
mkdir build && cd build
cmake ..
make
sudo make install
```

See [INSTALL.md](INSTALL.md) for more installation options.

## 🚀 Quick Start

### CLI
```bash
fastqr "Hello World" output.png
fastqr -s 500 -f 255,0,0 "Red QR" red.png
fastqr -F batch.txt output_dir/  # Batch mode - 7x faster!
```

### Ruby
```bash
gem install fastqr  # Pre-built binaries included!
```
```ruby
require 'fastqr'
FastQR.generate("Hello", "qr.png", size: 500)
FastQR.generate_batch(["QR 1", "QR 2"], "output/")  # Batch mode
```

### Node.js
```bash
npm install fastqr  # Pre-built binaries included!
```
```javascript
const fastqr = require('fastqr');
fastqr.generate('Hello', 'qr.png', { size: 500 });
fastqr.generateBatch(['QR 1', 'QR 2'], 'output/');  // Batch mode
```

### PHP
```bash
composer require fastqr/fastqr  # Pre-built binaries included!
```
```php
use FastQR\FastQR;
FastQR::generate('Hello', 'qr.png', ['size' => 500]);
FastQR::generateBatch(['QR 1', 'QR 2'], 'output/');  // Batch mode
```

### C++
```cpp
#include <fastqr.h>
fastqr::QROptions options;
options.size = 500;
fastqr::generate("Hello", "qr.png", options);
```

## 📚 Documentation

Complete usage guides for each platform:

- **[CLI Usage Guide](docs/CLI_USAGE.md)** - Complete command-line reference with all options and examples
- **[Ruby/Rails Usage Guide](docs/RUBY_USAGE.md)** - Ruby and Rails integration with examples
- **[Node.js Usage Guide](docs/NODEJS_USAGE.md)** - Node.js, Express, and TypeScript guide
- **[PHP Usage Guide](docs/PHP_USAGE.md)** - PHP, Laravel, and WordPress integration
- **[Documentation Index](docs/README.md)** - Full documentation portal

## 📖 API Reference

### QROptions

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `size` | int | 300 | Output size in pixels (QR codes are square) |
| `optimizeSize` | bool | false | Auto round-up to nearest integer multiple for best performance |
| `foreground` | RGB array | [0,0,0] | QR code color |
| `background` | RGB array | [255,255,255] | Background color |
| `errorLevel` | string | 'M' | Error correction: L (~7%), M (~15%), Q (~25%), H (~30%) |
| `logo` | string | "" | Path to logo image |
| `logoSize` | int | 20 | Logo size as percentage (1-50) |
| `quality` | int | 95 | Image quality for lossy formats (1-100) |
| `format` | string | 'png' | Output format: png, jpg, webp |

### Error Correction Levels

- **L (Low)**: ~7% of codewords can be restored
- **M (Medium)**: ~15% of codewords can be restored
- **Q (Quartile)**: ~25% of codewords can be restored
- **H (High)**: ~30% of codewords can be restored

Higher levels allow QR codes to remain readable when damaged or have logos embedded.

## 🔧 Development

### Build from Source

```bash
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr

# Install dependencies
brew install qrencode libpng cmake

# Build
mkdir build && cd build
cmake ..
make

# Run examples
./example_basic

# Run tests
make test
```

### Project Structure

```
fastqr/
├── include/           # C++ headers
│   ├── fastqr.h
│   ├── stb_image.h
│   └── stb_image_write.h
├── src/              # C++ source
│   ├── fastqr.cpp
│   └── cli.cpp
├── bindings/         # Language bindings
│   ├── ruby/
│   ├── nodejs/
│   └── php/
├── prebuilt/         # Pre-compiled binaries
│   ├── macos-arm64/
│   ├── macos-x86_64/
│   ├── linux-x86_64/
│   └── linux-arm64/
├── scripts/          # Build and install scripts
├── examples/         # Examples
├── cmake/           # CMake configs
└── LICENSE          # LGPL 2.1
```

## 📄 License

FastQR is licensed under the **GNU Lesser General Public License v2.1 (LGPL-2.1)**.

This project uses:
- **libqrencode** (LGPL v2.1) - Copyright (C) 2006-2017 Kentaro Fukuchi
- **libpng** - PNG image encoding
- **stb_image** (Public Domain) - Sean Barrett's single-header image library

As required by the LGPL, you can obtain the source code and rebuild this library with modified versions of libqrencode. See [BUILD.md](BUILD.md) for instructions.

### LGPL Requirements

When using FastQR in your projects:

1. **Open Source Projects**: You can freely use FastQR
2. **Closed Source/Commercial Projects**: You can use FastQR as a library, but:
   - You must include a copy of the LGPL license
   - You must state that your software uses FastQR
   - Users must be able to replace the FastQR library with a modified version

See [LICENSE](LICENSE) for full details.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for more details.

## 🐛 Bug Reports

If you find a bug, please open an issue with:
- Your OS and version
- FastQR version
- Steps to reproduce
- Expected vs actual behavior

## 📮 Contact

- GitHub: [@tranhuucanh](https://github.com/tranhuucanh)
- Issues: [GitHub Issues](https://github.com/tranhuucanh/fastqr/issues)

## 🙏 Acknowledgments

- [libqrencode](https://fukuchi.org/works/qrencode/) by Kentaro Fukuchi
- [libpng](http://www.libpng.org/pub/png/libpng.html) by PNG Development Group
- [stb libraries](https://github.com/nothings/stb) by Sean Barrett

## 📊 Benchmarks

```
Generating 100 QR codes (500x500px):  ~0.3 seconds
Generating 1000 QR codes (500x500px): ~3 seconds

Performance tested on modern hardware 🚀
```

## 🗺️ Roadmap

- [ ] Windows support
- [ ] SVG output
- [ ] Python bindings
- [ ] Batch processing API
- [ ] QR code scanning/decoding

---

Made with ❤️ by FastQR Project
