# Pre-built Binaries Release Process

This document describes how pre-built binaries are generated and distributed for FastQR.

## Overview

Starting from v1.0.0, FastQR includes pre-built binaries in all language packages (Ruby, Node.js, PHP). This means users can install packages without needing to install system dependencies (libqrencode, libvips).

## Supported Platforms

- **macOS**
  - arm64 (Apple Silicon M1/M2/M3)
  - x86_64 (Intel)
- **Linux**
  - x86_64 (64-bit Intel/AMD)
  - arm64 (64-bit ARM)

## How It Works

### 1. Building Binaries

When a new tag is pushed, the GitHub Actions workflow `.github/workflows/build-binaries.yml` automatically:

1. Builds binaries for all 4 platforms
2. Creates tarballs for each platform
3. Extracts and organizes binaries into binding directories
4. Commits binaries to the repository
5. Creates GitHub Release with all assets

### 2. Platform Detection & Loading

Each language binding automatically detects the platform and loads the appropriate binary:

**Ruby:**
```ruby
# bindings/ruby/lib/fastqr/platform.rb
FastQR::Platform.platform  # => "macos-arm64"
FastQR::Platform.lib_path  # => Path to pre-built binary
```

**Node.js:**
```javascript
// bindings/nodejs/lib/platform.js
platform.detectPlatform()  // => "macos-arm64"
platform.getPrebuiltPath() // => Path to pre-built binary
```

**PHP:**
```php
// bindings/php/src/FastQR.php
// Auto-detects platform and loads binary via FFI
```

### 3. Distribution

**Ruby Gem:**
- Pre-built binaries are included in `bindings/ruby/prebuilt/`
- Gem is published to RubyGems.org
- Users: `gem install fastqr` (no dependencies needed!)

**npm Package:**
- Pre-built binaries are included in `bindings/nodejs/prebuilt/`
- Package is published to npm
- Users: `npm install fastqr` (no dependencies needed!)

**Composer Package:**
- Pre-built binaries are committed to repository in `bindings/php/prebuilt/`
- Package is published to Packagist
- Users: `composer require fastqr/fastqr` (no dependencies needed!)

## Release Checklist

When releasing a new version:

1. **Update version** in all files:
   ```bash
   # CMakeLists.txt, package.json, gemspec, etc.
   ```

2. **Tag and push**:
   ```bash
   git tag -a v1.0.1 -m "Release 1.0.1"
   git push origin v1.0.1
   ```

3. **GitHub Actions will**:
   - Build binaries for all platforms
   - Create GitHub Release
   - Commit binaries to repository

4. **Publish packages**:
   ```bash
   # Ruby
   gem build fastqr.gemspec
   gem push fastqr-1.0.1.gem

   # Node.js
   cd bindings/nodejs
   npm publish

   # PHP (auto via webhook)
   ```

## Manual Build

To build binaries manually:

```bash
# Build for current platform
./scripts/build-binaries.sh

# Output: prebuilt/fastqr-VERSION-PLATFORM.tar.gz
```

## Binary Structure

Each platform binary tarball contains:

```
platform/
├── lib/
│   └── libfastqr.{dylib|so}  # Shared library
├── bin/
│   └── fastqr                 # CLI tool
└── include/
    └── fastqr.h               # Header file
```

## Troubleshooting

**Problem**: Binary not found for platform

**Solution**: Check if platform is supported. If not, user must build from source.

**Problem**: Binary fails to load

**Solution**: Check file permissions and paths. On macOS, may need to allow in System Preferences → Security.

**Problem**: GitHub Actions build fails

**Solution**: Check build logs. Common issues:
- Missing dependencies
- Platform-specific build errors
- Network issues downloading dependencies

## Future Improvements

- [ ] Add Windows support
- [ ] Add more Linux distributions (Alpine, CentOS)
- [ ] Optimize binary size
- [ ] Add binary verification/signing
- [ ] Cache dependencies in CI for faster builds

## License Compliance

Pre-built binaries statically link libqrencode and libvips (both LGPL 2.1).

To comply with LGPL:
1. LICENSE file is included in all packages
2. BUILD.md provides instructions for rebuilding with custom libraries
3. Source code is available on GitHub
4. Attribution is included in README

## Resources

- [GitHub Actions Workflow](.github/workflows/build-binaries.yml)
- [Build Script](scripts/build-binaries.sh)
- [Platform Detection (Ruby)](bindings/ruby/lib/fastqr/platform.rb)
- [Platform Detection (Node.js)](bindings/nodejs/lib/platform.js)
- [Pre-built Binaries Directory](prebuilt/)

