# FastQR

[![License](https://img.shields.io/badge/License-LGPL%202.1-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-14-blue.svg)](https://isocpp.org/)
[![C](https://img.shields.io/badge/C-11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![CMake](https://img.shields.io/badge/CMake-3.10+-064F8C.svg)](https://cmake.org/)
[![Shell](https://img.shields.io/badge/Shell-Bash-4EAA25.svg)](https://www.gnu.org/software/bash/)
[![JavaScript](https://img.shields.io/badge/JavaScript-ES6+-F7DF1E.svg)](https://www.ecma-international.org/ecma-262/)
[![Ruby](https://img.shields.io/badge/Ruby-Gem-red.svg)](https://rubygems.org/gems/fastqr)
[![Node.js](https://img.shields.io/badge/Node.js-npm-green.svg)](https://www.npmjs.com/package/fastqr)
[![PHP](https://img.shields.io/badge/PHP-Composer-blue.svg)](https://packagist.org/packages/fastqr/fastqr)

**The fastest QR code generator on the planet.** 🚀

Generate 1,000 QR codes in **0.37 seconds**. Full UTF-8 support. Custom colors. Logo embedding. Precise size control.

---

## 🔥 Performance Benchmarks

### ⌨️ CLI Performance: FastQR vs qrencode

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 2.56s 🚀 | **0.37s** ⚡ | **8x faster** with batch |
| qrencode | 2.97s 🐌 | ❌ Not supported | - |

**FastQR is 16% faster in sequential mode and 8x faster with batch mode!**

---

### 💎 Ruby Performance: FastQR vs rqrcode

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 3.49s 🚀 | **0.38s** ⚡ | **17x faster** with batch |
| rqrcode | 59.45s 🐢 | ❌ Not supported | - |

**FastQR is 17x faster in sequential mode and 157x faster with batch mode!**

---

### 🟢 Node.js Performance: FastQR vs qrcode

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 2.43s 🚀 | **0.46s** ⚡ | **7x faster** with batch |
| qrcode | 17.25s 🐌 | ❌ Not supported | - |

**FastQR is 7x faster in sequential mode and 37x faster with batch mode!**

---

### 🐘 PHP Performance: FastQR vs endroid/qr-code

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 1.75s 🚀 | **0.64s** ⚡ | **8.4x faster** with batch |
| endroid/qr-code | 14.72s 🐌 | ❌ Not supported | - |

**FastQR is 8.4x faster in sequential mode and 23x faster with batch mode!**

---

## 💪 Why FastQR Dominates

### Speed Comparison Summary

| Platform | Library | Sequential | Batch | FastQR Advantage |
|----------|---------|------------|-------|------------------|
| **⌨️ CLI** | qrencode | 2.97s | ❌ | **8x faster** ⚡ |
| **💎 Ruby** | rqrcode | 59.45s | ❌ | **157x faster** 🚀 |
| **🟢 Node.js** | qrcode | 17.25s | ❌ | **37x faster** ⚡ |
| **🐘 PHP** | endroid/qr-code | 14.72s | ❌ | **23x faster** 🚀 |

**Average: FastQR with batch mode is 56x faster than popular alternatives!**

---

## ✨ What Makes FastQR Special?

### Features That Set Us Apart

| Feature | FastQR | Others |
|---------|--------|--------|
| **Batch Processing** | ✅ Up to 157x faster | ❌ Not available |
| **Exact Size Control** | ✅ 2000×2000px exact | ❌ Scale-based (imprecise) |
| **Full UTF-8 Support** | ✅ Vietnamese, Japanese, Chinese, emoji | ⚠️ Limited or none |
| **Custom Colors** | ✅ RGB foreground + background | ⚠️ Limited or none |
| **Logo Embedding** | ✅ Built-in logo support | ⚠️ Limited or none |
| **Language Bindings** | ✅ Ruby, Node.js, PHP, C++ | ✅ Usually supported |
| **Error Correction** | ✅ 4 levels (L, M, Q, H) | ✅ Usually supported |

---

## 🚀 Key Features

- ⚡ **Blazing Fast**: Up to **157x faster** than popular alternatives with batch mode
- 🔥 **Batch Processing**: Generate 1,000 QR codes in **~0.37 seconds**
- 🌐 **Full UTF-8 Support**: Vietnamese, Japanese (Kanji, Hiragana, Katakana), Chinese, emoji, and more
- 🎨 **Custom Colors**: Choose any RGB color for QR code and background
- 📐 **Exact Size Control**: Generate QR codes with precise pixel dimensions (e.g., 2000×2000px)
- 🖼️ **Logo Embedding**: Add company logos to the center of QR codes
- 🛡️ **Error Correction**: Supports 4 levels (L, M, Q, H)
- 💾 **Multiple Formats**: PNG, JPG, WebP
- 🔧 **Multiple Languages**: Native bindings for Ruby, Node.js, PHP, and C++

---

## 📦 Installation

### CLI Installation

**macOS (Homebrew):**
```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

**Linux (x86_64):**
```bash
# Download latest release (replace VERSION with latest version, e.g., 1.0.21)
VERSION="1.0.21"
wget https://github.com/tranhuucanh/fastqr/releases/download/v${VERSION}/fastqr-${VERSION}-linux-x86_64.tar.gz
tar -xzf fastqr-${VERSION}-linux-x86_64.tar.gz
sudo cp linux-x86_64/bin/fastqr /usr/local/bin/
sudo chmod +x /usr/local/bin/fastqr

# Verify installation
fastqr --version
```

**Linux (ARM64/aarch64):**
```bash
VERSION="1.0.21"
wget https://github.com/tranhuucanh/fastqr/releases/download/v${VERSION}/fastqr-${VERSION}-linux-aarch64.tar.gz
tar -xzf fastqr-${VERSION}-linux-aarch64.tar.gz
sudo cp linux-aarch64/bin/fastqr /usr/local/bin/
sudo chmod +x /usr/local/bin/fastqr
```

### Language Packages

**Ruby:**
```bash
gem install fastqr
```

**Node.js:**
```bash
npm install fastqr-pro
```

**PHP:**
```bash
composer require fastqr/fastqr
```

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

See [INSTALL.md](INSTALL.md) for more options.

---

## 🎯 Quick Start

### CLI

**Single QR Code:**
```bash
fastqr "Hello World" output.png
fastqr -s 500 -f 255,0,0 "Red QR" red.png
fastqr -l logo.png -L 25 "QR with Logo" branded.png
```

**Batch Mode (8x faster!):**
```bash
# Create batch.txt with one text per line
echo "https://example.com/user/1" > batch.txt
echo "https://example.com/user/2" >> batch.txt
# ... add 1000 URLs

# Generate 1,000 QR codes in 0.37 seconds ⚡
fastqr -F batch.txt output_dir/
```

---

### Ruby

```ruby
require 'fastqr'

# Single QR code
FastQR.generate("Hello World", "qr.png", size: 500)

# Batch mode - 157x faster than rqrcode! 🚀
urls = (1..1000).map { |i| "https://example.com/user/#{i}" }
FastQR.generate_batch(urls, "output/")
# ⚡ Done in 0.38 seconds! (vs 59.45s with rqrcode)

# Custom colors and logo
FastQR.generate("https://example.com", "branded.png",
  size: 800,
  foreground: [255, 0, 0],      # Red QR code
  background: [255, 255, 200],  # Light yellow background
  logo: "logo.png",
  logoSize: 20,
  errorLevel: "H"               # High error correction for logos
)
```

---

### Node.js

```javascript
const fastqr = require('fastqr');

// Single QR code
fastqr.generate('Hello World', 'qr.png', { size: 500 });

// Batch mode - 37x faster than qrcode! 🚀
const urls = Array.from({length: 1000}, (_, i) =>
  `https://example.com/user/${i+1}`
);
fastqr.generateBatch(urls, 'output/');
// ⚡ Done in 0.46 seconds! (vs 17.25s with qrcode)

// Custom colors and logo
fastqr.generate('https://example.com', 'branded.png', {
  size: 800,
  foreground: [255, 0, 0],      // Red QR code
  background: [255, 255, 200],  // Light yellow background
  logo: 'logo.png',
  logoSize: 20,
  errorLevel: 'H'               // High error correction for logos
});
```

---

### PHP

```php
use FastQR\FastQR;

// Single QR code
FastQR::generate('Hello World', 'qr.png', ['size' => 500]);

// Batch mode - Lightning fast! ⚡
$urls = array_map(fn($i) => "https://example.com/user/$i", range(1, 1000));
FastQR::generateBatch($urls, 'output/');
// Done in ~0.4 seconds!

// Custom colors and logo
FastQR::generate('https://example.com', 'branded.png', [
    'size' => 800,
    'foreground' => [255, 0, 0],      // Red QR code
    'background' => [255, 255, 200],  // Light yellow background
    'logo' => 'logo.png',
    'logoSize' => 20,
    'errorLevel' => 'H'               // High error correction for logos
]);
```

---

### C++

```cpp
#include <fastqr.h>

// Single QR code
fastqr::QROptions options;
options.size = 500;
fastqr::generate("Hello World", "qr.png", options);

// Batch mode - Blazing fast! 🚀
std::vector<std::string> urls;
for (int i = 1; i <= 1000; i++) {
    urls.push_back("https://example.com/user/" + std::to_string(i));
}
fastqr::generateBatch(urls, "output/", options);
// Done in ~0.4 seconds!
```

---

## 📖 API Reference

### QROptions

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `size` | int | 300 | Output size in pixels (QR codes are square) |
| `optimizeSize` | bool | false | Auto round-up to nearest integer multiple |
| `foreground` | RGB array | [0,0,0] | QR code color (RGB) |
| `background` | RGB array | [255,255,255] | Background color (RGB) |
| `errorLevel` | string | 'M' | Error correction: L, M, Q, H |
| `logo` | string | "" | Path to logo image |
| `logoSize` | int | 20 | Logo size as percentage (1-50) |
| `quality` | int | 95 | Image quality for lossy formats (1-100) |
| `format` | string | 'png' | Output format: png, jpg, webp |

### Error Correction Levels

| Level | Recovery | Use Case |
|-------|----------|----------|
| **L (Low)** | ~7% | Maximum data capacity, clean environment |
| **M (Medium)** | ~15% | Balanced (default) |
| **Q (Quartile)** | ~25% | Good for QR codes with logos |
| **H (High)** | ~30% | Best for damaged/dirty environments |

Higher levels allow QR codes to remain readable when damaged or have logos embedded.

---

## 📚 Documentation

Complete usage guides for each platform:

- **[CLI Usage Guide](docs/CLI_USAGE.md)** - Complete command-line reference
- **[Ruby/Rails Usage Guide](docs/RUBY_USAGE.md)** - Ruby and Rails integration
- **[Node.js Usage Guide](docs/NODEJS_USAGE.md)** - Node.js, Express, and TypeScript
- **[PHP Usage Guide](docs/PHP_USAGE.md)** - PHP, Laravel, and WordPress integration
- **[Documentation Index](docs/README.md)** - Full documentation portal

---

## 🏗️ Architecture

FastQR is built on battle-tested, industry-standard libraries:

- **[libqrencode](https://fukuchi.org/works/qrencode/)** (LGPL v2.1) - QR code bit matrix generation
- **[libpng](http://www.libpng.org/pub/png/libpng.html)** - Lightning-fast PNG encoding
- **[stb_image](https://github.com/nothings/stb)** (Public Domain) - Efficient image loading

**Why so fast?**
- ⚡ Zero process forking overhead
- 🚀 Optimized native C++ core
- 💪 Batch mode processes multiple QR codes in a single call
- 🔥 Efficient memory management and image encoding

---

## 📄 License

FastQR is licensed under the **GNU Lesser General Public License v2.1 (LGPL-2.1)**.

### LGPL Requirements

When using FastQR in your projects:

1. **Open Source Projects**: Use freely ✅
2. **Closed Source/Commercial Projects**: Use as a library, but:
   - Include a copy of the LGPL license
   - State that your software uses FastQR
   - Users must be able to replace FastQR with a modified version

See [LICENSE](LICENSE) for full details.

---

## 🤝 Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

1. Fork the repository
2. Create your feature branch (`git checkout -b feature/AmazingFeature`)
3. Commit your changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

See [CONTRIBUTING.md](CONTRIBUTING.md) for details.

---

## 🐛 Bug Reports

Found a bug? Please open an issue with:
- Your OS and version
- FastQR version
- Steps to reproduce
- Expected vs actual behavior

---

## 🗺️ Roadmap

- [ ] Windows support
- [ ] SVG output format
- [ ] Python bindings
- [ ] QR code scanning/decoding
- [ ] Advanced batch processing options

---

## 📮 Contact

- **GitHub**: [@tranhuucanh](https://github.com/tranhuucanh)
- **Issues**: [GitHub Issues](https://github.com/tranhuucanh/fastqr/issues)

---

## 🙏 Acknowledgments

Special thanks to:
- [libqrencode](https://fukuchi.org/works/qrencode/) by Kentaro Fukuchi
- [libpng](http://www.libpng.org/pub/png/libpng.html) by PNG Development Group
- [stb libraries](https://github.com/nothings/stb) by Sean Barrett

---

<div align="center">

**Made with ❤️ by FastQR Project**

*If FastQR saves you time, give us a star!* ⭐

[⬆ Back to top](#fastqr-)

</div>