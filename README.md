# FastQR

<div align="center">

<!-- Logo placeholder - add your logo here -->
<!-- <img src="docs/logo.png" alt="FastQR Logo" width="200"/> -->

**The fastest QR code generator on the planet.** 🚀

Generate 1,000 QR codes in **0.37 seconds**. Full UTF-8 support. Custom colors. Logo embedding. Precise size control.

[![GitHub Stars](https://img.shields.io/github/stars/tranhuucanh/fastqr?style=social)](https://github.com/tranhuucanh/fastqr/stargazers)
[![GitHub Forks](https://img.shields.io/github/forks/tranhuucanh/fastqr?style=social)](https://github.com/tranhuucanh/fastqr/network/members)

[Installation](#-installation) • [Quick Start](#-quick-start) • [Documentation](#-documentation) • [Benchmarks](#-performance-benchmarks)

---

### 🛠️ Tech Stack & 📊 Stats

[![License](https://img.shields.io/badge/License-LGPL%202.1-blue.svg)](LICENSE)
[![C++](https://img.shields.io/badge/C++-14-blue.svg)](https://isocpp.org/)
[![C](https://img.shields.io/badge/C-11-blue.svg)](https://en.wikipedia.org/wiki/C11_(C_standard_revision))
[![CMake](https://img.shields.io/badge/CMake-3.10+-064F8C.svg)](https://cmake.org/)
[![Shell](https://img.shields.io/badge/Shell-Bash-4EAA25.svg)](https://www.gnu.org/software/bash/)
[![JavaScript](https://img.shields.io/badge/JavaScript-ES6+-F7DF1E.svg)](https://www.ecma-international.org/ecma-262/)

💎 **Ruby:** [![Gem](https://img.shields.io/badge/Gem-fastqr-red.svg)](https://rubygems.org/gems/fastqr) [![Gem Downloads](https://badgen.net/rubygems/dt/fastqr)](https://rubygems.org/gems/fastqr)

🟢 **Node.js:** [![npm](https://img.shields.io/badge/npm-fastqr--pro-green.svg)](https://www.npmjs.com/package/fastqr-pro) [![npm downloads](https://img.shields.io/npm/dt/fastqr-pro)](https://www.npmjs.com/package/fastqr-pro)

🐘 **PHP:** [![Composer](https://img.shields.io/badge/Composer-fastqr-blue.svg)](https://packagist.org/packages/fastqr/fastqr) [![Packagist Downloads](https://img.shields.io/packagist/dt/fastqr/fastqr)](https://packagist.org/packages/fastqr/fastqr)

📦 **CLI:** [![GitHub Downloads](https://img.shields.io/github/downloads/tranhuucanh/fastqr/total)](https://github.com/tranhuucanh/fastqr/releases)

</div>

---

## 📑 Table of Contents

- [Performance Benchmarks](#-performance-benchmarks)
- [Key Features](#-key-features)
- [Installation](#-installation)
- [Quick Start](#-quick-start)
- [API Reference](#-api-reference)
- [Documentation](#-documentation)
- [Architecture](#️-architecture)
- [License](#-license)
- [Contributing](#-contributing)
- [Bug Reports](#-bug-reports)
- [Support This Project](#-support-this-project)
- [Roadmap](#️-roadmap)
- [Contact & Support](#-contact--support)
- [Acknowledgments](#-acknowledgments)

---

## 🔥 Performance Benchmarks

<div align="center">

**Generate 1,000 QR codes (500×500px)**

| Platform | FastQR Batch | Competitor | Competitor Time | 🚀 Speedup |
|----------|-------------:|------------|----------------:|-----------:|
| ⌨️ **CLI** | **0.37s** ⚡ | qrencode | 2.97s | **8x** |
| 💎 **Ruby** | **0.38s** ⚡ | rqrcode | 59.45s | **157x** 🏆 |
| 🟢 **Node.js** | **0.46s** ⚡ | qrcode | 17.25s | **37x** |
| 🐘 **PHP** | **0.64s** ⚡ | endroid/qr-code | 14.72s | **23x** |

**Average: 56x faster than popular alternatives!** 🎯

</div>

<details closed>
<summary>📊 <b>CLI Performance: FastQR vs qrencode</b></summary>

<br/>

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 2.56s 🚀 | **0.37s** ⚡ | **8x faster** with batch |
| qrencode | 2.97s 🐌 | ❌ Not supported | - |

**FastQR is 16% faster in sequential mode and 8x faster with batch mode!**

</details>

<details closed>
<summary>💎 <b>Ruby Performance: FastQR vs rqrcode</b></summary>

<br/>

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 3.49s 🚀 | **0.38s** ⚡ | **17x faster** with batch |
| rqrcode | 59.45s 🐢 | ❌ Not supported | - |

**FastQR is 17x faster in sequential mode and 157x faster with batch mode!**

</details>

<details closed>
<summary>🟢 <b>Node.js Performance: FastQR vs qrcode</b></summary>

<br/>

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 2.43s 🚀 | **0.46s** ⚡ | **7x faster** with batch |
| qrcode | 17.25s 🐌 | ❌ Not supported | - |

**FastQR is 7x faster in sequential mode and 37x faster with batch mode!**

</details>

<details closed>
<summary>🐘 <b>PHP Performance: FastQR vs endroid/qr-code</b></summary>

<br/>

*Task: Generate 1,000 QR codes (500×500px)*

| Library | Sequential Mode | Batch Mode | Speedup |
|---------|----------------|------------|---------|
| **FastQR** | 1.75s 🚀 | **0.64s** ⚡ | **8.4x faster** with batch |
| endroid/qr-code | 14.72s 🐌 | ❌ Not supported | - |

**FastQR is 8.4x faster in sequential mode and 23x faster with batch mode!**

</details>

---

## 💪 Key Features

<table width="100%">
<tr>
<td width="50%">

### ⚡ Performance
- **Up to 157x faster** than alternatives
- **Batch mode**: 1,000 QR codes in 0.37s
- Zero process forking overhead
- Optimized C++ core

</td>
<td width="50%">

### 🎨 Customization
- **Custom colors** (RGB for QR & background)
- **Logo embedding** with auto-scaling
- **Exact size control** (e.g., 2000×2000px)
- **Multiple formats**: PNG, JPG, WebP

</td>
</tr>
<tr>
<td width="50%">

### 🌐 UTF-8 Support
- Vietnamese, Japanese, Chinese
- Emoji and special characters
- All Unicode characters supported

</td>
<td width="50%">

### 🔧 Multi-Language
- Ruby, Node.js, PHP, C++
- Native bindings for each language
- Consistent API across platforms

</td>
</tr>
</table>

---

## 📦 Installation

<table>
<tr>
<td width="50%">

### 🍎 macOS
```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

### 🐧 Linux
```bash
# x86_64
# Download latest release (replace VERSION with latest version, e.g., 1.0.26)
VERSION="1.0.26"
wget https://github.com/tranhuucanh/fastqr/releases/download/v${VERSION}/fastqr-${VERSION}-linux-x86_64.tar.gz
tar -xzf fastqr-${VERSION}-linux-x86_64.tar.gz
sudo cp linux-x86_64/bin/fastqr /usr/local/bin/
sudo chmod +x /usr/local/bin/fastqr

# Verify installation
fastqr --version

# ARM64
VERSION="1.0.26"
wget https://github.com/tranhuucanh/fastqr/releases/download/v${VERSION}/fastqr-${VERSION}-linux-aarch64.tar.gz
tar -xzf fastqr-${VERSION}-linux-aarch64.tar.gz
sudo cp linux-aarch64/bin/fastqr /usr/local/bin/
sudo chmod +x /usr/local/bin/fastqr
```

</td>
<td width="50%">

### 💎 Ruby
```bash
gem install fastqr
```

### 🟢 Node.js
```bash
npm install fastqr-pro
```

### 🐘 PHP
```bash
composer require fastqr/fastqr
```

### 🔨 Build from Source
```bash
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr && mkdir build && cd build
cmake .. && make && sudo make install
```

</td>
</tr>
</table>

---

## 🎯 Quick Start

### CLI
```bash
# Single QR code
fastqr "Hello World" output.png

# With custom colors
fastqr -s 500 -f 255,0,0 -b 255,255,200 "Red QR" red.png

# With logo
fastqr -l logo.png "QR with Logo" branded.png

# Batch mode (8x faster!)
fastqr -F urls.txt output_dir/
```

<details>
<summary>💎 <b>Ruby</b></summary>

```ruby
require 'fastqr'

# Single QR
FastQR.generate("Hello World", "qr.png", size: 500)

# Batch mode (157x faster!)
urls = (1..1000).map { |i| "https://example.com/user/#{i}" }
FastQR.generate_batch(urls, "output/")

# With colors and logo
FastQR.generate("https://example.com", "branded.png",
  size: 800,
  foreground: [255, 0, 0],
  background: [255, 255, 200],
  logo: "logo.png",
  logoSize: 20,
  errorLevel: "H"
)
```

[Full Ruby Guide →](docs/RUBY_USAGE.md)
</details>

<details>
<summary>🟢 <b>Node.js</b></summary>

```javascript
const fastqr = require('fastqr-pro');

// Single QR
fastqr.generate('Hello World', 'qr.png', { size: 500 });

// Batch mode (37x faster!)
const urls = Array.from({length: 1000}, (_, i) => `https://example.com/user/${i+1}`);
fastqr.generateBatch(urls, 'output/');

// With colors and logo
fastqr.generate('https://example.com', 'branded.png', {
  size: 800,
  foreground: [255, 0, 0],
  background: [255, 255, 200],
  logo: 'logo.png',
  logoSize: 20,
  errorLevel: 'H'
});
```

[Full Node.js Guide →](docs/NODEJS_USAGE.md)
</details>

<details>
<summary>🐘 <b>PHP</b></summary>

```php
use FastQR\FastQR;

// Single QR
FastQR::generate('Hello World', 'qr.png', ['size' => 500]);

// Batch mode (23x faster!)
$urls = array_map(fn($i) => "https://example.com/user/$i", range(1, 1000));
FastQR::generateBatch($urls, 'output/');

// With colors and logo
FastQR::generate('https://example.com', 'branded.png', [
    'size' => 800,
    'foreground' => [255, 0, 0],
    'background' => [255, 255, 200],
    'logo' => 'logo.png',
    'logoSize' => 20,
    'errorLevel' => 'H'
]);
```

[Full PHP Guide →](docs/PHP_USAGE.md)
</details>

<details>
<summary>⚙️ <b>C++</b></summary>

```cpp
#include <fastqr.h>

// Single QR
fastqr::QROptions options;
options.size = 500;
fastqr::generate("Hello World", "qr.png", options);

// Batch mode
std::vector<std::string> urls;
for (int i = 1; i <= 1000; i++) {
    urls.push_back("https://example.com/user/" + std::to_string(i));
}
fastqr::generateBatch(urls, "output/", options);
```
</details>

---

## 📖 API Reference

### Common Options

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

## 💖 Support This Project

If FastQR helps you save time and money, consider supporting its development:
- ⭐ Star this repository
- 🐛 Report bugs and suggest features
- 📖 Improve documentation
- 💬 Share FastQR with others
- ✍️ Write a blog post or tutorial

---

## 🗺️ Roadmap

**Coming Soon:** Windows support • SVG output • Python bindings • Advanced batch processing options

---

## 📮 Contact & Support

**GitHub:** [@tranhuucanh](https://github.com/tranhuucanh) • [Issues](https://github.com/tranhuucanh/fastqr/issues) • [Discussions](https://github.com/tranhuucanh/fastqr/discussions)

---

## 🙏 Acknowledgments

Built with: **[libqrencode](https://fukuchi.org/works/qrencode/)** by Kentaro Fukuchi • **[libpng](http://www.libpng.org/pub/png/libpng.html)** by PNG Development Group • **[stb](https://github.com/nothings/stb)** by Sean Barrett

Thanks to all [contributors](https://github.com/tranhuucanh/fastqr/graphs/contributors)! 🎉

---

<div align="center">

**Made with ❤️ by FastQR Project**

*If FastQR saves you time, give us a star!* ⭐

[![Star History Chart](https://api.star-history.com/svg?repos=tranhuucanh/fastqr&type=Date)](https://star-history.com/#tranhuucanh/fastqr&Date)

[⬆ Back to top](#fastqr)

</div>