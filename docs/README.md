# FastQR Documentation

Complete documentation for FastQR - Fast QR Code Generator.

## Overview

FastQR is a fast, powerful QR code generator with full UTF-8 support, custom colors, logo embedding, and precise size control. Pre-built binaries are included in all language packages - no system dependencies required!

## Quick Links

### Installation & Setup
- [Installation Guide](../INSTALL.md) - Installation for all platforms
- [Build Instructions](../BUILD.md) - Build from source
- [Distribution Guide](../DISTRIBUTION.md) - Publishing to package managers
- [Pre-built Binaries](../PREBUILT.md) - About pre-compiled binaries

### Usage Guides
- **[CLI Usage](CLI_USAGE.md)** - Command-line tool complete guide
- **[Ruby Usage](RUBY_USAGE.md)** - Ruby and Rails integration
- **[Node.js Usage](NODEJS_USAGE.md)** - Node.js and Express integration
- **[PHP Usage](PHP_USAGE.md)** - PHP, Laravel, and WordPress integration

### Project Information
- [README](../README.md) - Project overview and features
- [Contributing](../CONTRIBUTING.md) - How to contribute
- [Changelog](../CHANGELOG.md) - Version history
- [License](../LICENSE) - LGPL 2.1 license

## Features at a Glance

### ✨ Core Features
- 🚀 **High Performance** - No process forking, 2.6x faster than qrencode
- 🌐 **Full UTF-8 Support** - Vietnamese, Japanese, Chinese, emoji, etc.
- 📐 **Exact Size Control** - Generate QR codes with precise pixel dimensions
- 🎨 **Custom Colors** - RGB colors for QR code and background
- 🖼️ **Logo Embedding** - Add company logos to QR codes
- 🛡️ **Error Correction** - 4 levels (L, M, Q, H) up to 30% recovery
- 💾 **Multiple Formats** - PNG, JPG, WebP

### 📦 No Dependencies!
Starting from v1.0.0, all language packages include pre-built binaries:
- Ruby: `gem install fastqr` ✅
- Node.js: `npm install fastqr` ✅
- PHP: `composer require fastqr/fastqr` ✅

No need to install libqrencode or libvips separately!

## Quick Start

### CLI
```bash
# Install
brew install fastqr  # macOS

# Use
fastqr "Hello World" qr.png
fastqr -s 1000x1000 -f 255,0,0 "Large Red QR" large.png
```
[→ Full CLI Guide](CLI_USAGE.md)

### Ruby
```ruby
gem install fastqr

require 'fastqr'
FastQR.generate("Hello World", "qr.png", width: 500, height: 500)
```
[→ Full Ruby Guide](RUBY_USAGE.md)

### Node.js
```javascript
npm install fastqr

const fastqr = require('fastqr');
fastqr.generate('Hello World', 'qr.png', { width: 500, height: 500 });
```
[→ Full Node.js Guide](NODEJS_USAGE.md)

### PHP
```php
composer require fastqr/fastqr

use FastQR\FastQR;
FastQR::generate('Hello World', 'qr.png', ['width' => 500, 'height' => 500]);
```
[→ Full PHP Guide](PHP_USAGE.md)

## Common Use Cases

### 1. Website QR Codes
Generate QR codes for URLs with custom branding:
```bash
fastqr -s 600x600 -l logo.png -e H "https://example.com" website_qr.png
```

### 2. Business Cards (vCard)
```bash
fastqr "BEGIN:VCARD
VERSION:3.0
FN:John Doe
EMAIL:john@example.com
TEL:+1234567890
END:VCARD" vcard.png
```

### 3. WiFi Credentials
```bash
fastqr "WIFI:T:WPA;S:NetworkName;P:password123;;" wifi.png
```

### 4. Event Information
With full UTF-8 support:
```bash
fastqr "Sự kiện: Hội thảo công nghệ
Thời gian: 20/10/2025
Địa điểm: TP.HCM
URL: https://event.com" event.png
```

### 5. Product Labels
High-resolution QR for printing:
```bash
fastqr -s 2000x2000 -q 100 -e H "SKU-12345" product_qr.png
```

## Platform Comparison

| Feature | CLI | Ruby | Node.js | PHP |
|---------|-----|------|---------|-----|
| **Installation** | `brew install` | `gem install` | `npm install` | `composer require` |
| **Dependencies** | ✅ None (binary) | ✅ None (bundled) | ✅ None (bundled) | ✅ None (bundled) |
| **UTF-8** | ✅ Full | ✅ Full | ✅ Full | ✅ Full |
| **Colors** | ✅ RGB | ✅ RGB | ✅ RGB | ✅ RGB |
| **Logo** | ✅ Yes | ✅ Yes | ✅ Yes | ✅ Yes |
| **Async** | - | ✅ Yes | ✅ Yes | - |
| **TypeScript** | - | - | ✅ Yes | - |

## API Quick Reference

### Options (All Languages)

| Option | CLI | Ruby | Node.js | PHP | Default |
|--------|-----|------|---------|-----|---------|
| **Size** | `-s 500x500` | `width: 500, height: 500` | `{width: 500, height: 500}` | `['width' => 500, 'height' => 500]` | 300x300 |
| **Foreground** | `-f 255,0,0` | `foreground: [255,0,0]` | `{foreground: [255,0,0]}` | `['foreground' => [255,0,0]]` | [0,0,0] |
| **Background** | `-b 255,255,200` | `background: [255,255,200]` | `{background: [255,255,200]}` | `['background' => [255,255,200]]` | [255,255,255] |
| **Error Level** | `-e H` | `error_level: 'H'` | `{errorLevel: 'H'}` | `['errorLevel' => 'H']` | 'M' |
| **Logo** | `-l logo.png` | `logo: 'logo.png'` | `{logo: 'logo.png'}` | `['logo' => 'logo.png']` | - |
| **Logo Size** | `-p 25` | `logo_size: 25` | `{logoSize: 25}` | `['logoSize' => 25]` | 20 |
| **Quality** | `-q 95` | `quality: 95` | `{quality: 95}` | `['quality' => 95]` | 95 |

## Getting Help

- **Documentation**: You're here! Browse the usage guides above
- **Issues**: [GitHub Issues](https://github.com/tranhuucanh/fastqr/issues)
- **Contributing**: See [CONTRIBUTING.md](../CONTRIBUTING.md)
- **License**: See [LICENSE](../LICENSE) (LGPL 2.1)

## Architecture

FastQR is built on:
- **[libqrencode](https://fukuchi.org/works/qrencode/)** (LGPL v2.1) - QR code generation
- **[libvips](https://libvips.github.io/libvips/)** (LGPL v2.1+) - Image processing

Pre-built binaries are automatically generated for:
- macOS (Intel & Apple Silicon)
- Linux (x86_64 & ARM64)

See [PREBUILT.md](../PREBUILT.md) for details on the build process.

## Support

FastQR is open source software. For support:

1. Check the relevant usage guide above
2. Search [existing issues](https://github.com/tranhuucanh/fastqr/issues)
3. Open a new issue if needed

## License

FastQR is licensed under LGPL 2.1. See [LICENSE](../LICENSE) for details.

---

**Made with ❤️ by the FastQR Project**

[GitHub](https://github.com/tranhuucanh/fastqr) •
[npm](https://www.npmjs.com/package/fastqr) •
[RubyGems](https://rubygems.org/gems/fastqr) •
[Packagist](https://packagist.org/packages/fastqr/fastqr)

