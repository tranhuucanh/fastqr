# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-10-18

### Added
- Initial release of FastQR
- Core C++ library with libqrencode and libvips integration
- Full UTF-8 support (Vietnamese, Japanese, Chinese, emoji, etc.)
- Custom color support for QR code and background
- Precise size control (exact pixel dimensions)
- Logo embedding capability
- Error correction levels (L, M, Q, H)
- Multiple output formats (PNG, JPG, WebP)
- CLI tool with comprehensive options
- Ruby binding (Gem)
- Node.js binding (npm) with TypeScript definitions
- PHP binding (Composer) using FFI
- CMake build system
- Comprehensive documentation (README, BUILD, DISTRIBUTION)
- Example code
- Unit tests for all bindings
- LGPL 2.1 license compliance
- macOS and Linux support

### Features
- Generate QR codes with exact dimensions (e.g., 2000x2000px)
- Support for all UTF-8 characters
- RGB color customization for foreground and background
- Logo overlay with size percentage control
- 4 error correction levels
- Quality control for lossy formats
- Fast performance (no process forking)

### Bindings
- **Ruby**: Full-featured gem with native extension
- **Node.js**: Native addon with N-API
- **PHP**: FFI-based binding (no compilation needed)

### Documentation
- Comprehensive README with examples
- BUILD.md with detailed compilation instructions
- DISTRIBUTION.md with publishing guides for all package managers
- API documentation for all languages

[1.0.0]: https://github.com/tranhuucanh/fastqr/releases/tag/v1.0.0

