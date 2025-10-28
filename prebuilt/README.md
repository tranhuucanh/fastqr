# Pre-built Binaries

This directory contains pre-built binaries for FastQR on different platforms.

## Supported Platforms

- **macOS**
  - arm64 (Apple Silicon M1/M2/M3)
  - x86_64 (Intel)
- **Linux**
  - x86_64 (64-bit Intel/AMD)
  - arm64 (64-bit ARM)

## Directory Structure

```
prebuilt/
├── macos-arm64/
│   ├── lib/
│   │   └── libfastqr.dylib
│   ├── bin/
│   │   └── fastqr
│   └── include/
│       └── fastqr.h
├── macos-x86_64/
│   └── ...
├── linux-x86_64/
│   └── ...
└── linux-arm64/
    └── ...
```

## Building Binaries

To build binaries for your platform:

```bash
./scripts/build-binaries.sh
```

This will create a tarball in `prebuilt/fastqr-VERSION-PLATFORM.tar.gz`.

## Using Pre-built Binaries

### For Ruby Gem

Pre-built binaries are automatically included in the gem and loaded via FFI.

```bash
gem install fastqr
# No need to install qrencode!
```

### For Node.js

Pre-built binaries are automatically included in the npm package.

```bash
npm install fastqr-pro
# No need to install qrencode!
```

### For PHP

Pre-built binaries are included in the Composer package.

```bash
composer require fastqr/fastqr
# No need to install qrencode!
```

### Manual Installation

Download and install pre-built binaries manually:

```bash
# Download from GitHub Releases
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

## Platform Detection

The bindings automatically detect your platform and load the appropriate binary:

- **Ruby**: `FastQR::Platform.platform`
- **Node.js**: `require('./lib/platform').detectPlatform()`
- **PHP**: Platform detected in `FastQR::init()`

## Notes

- Binaries are built with static linking of libqrencode
- No external dependencies required at runtime
- Binaries are built on GitHub Actions for consistency
- All binaries are tested before release

## Building for All Platforms

To build binaries for all platforms, use GitHub Actions:

```bash
git tag -a v1.0.0 -m "Release 1.0.0"
git push origin v1.0.0
```

This will trigger the release workflow which builds for all platforms.

## License

The pre-built binaries include statically linked libraries:
- libqrencode (LGPL 2.1)

As required by LGPL, users can rebuild with different versions by building from source.

