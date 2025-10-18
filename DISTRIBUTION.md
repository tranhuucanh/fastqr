# FastQR Distribution Guide

Detailed guide for distributing FastQR to various package managers.

## 📋 Table of Contents

- [Homebrew (macOS)](#homebrew-macos)
- [APT/DEB (Ubuntu/Debian)](#aptdeb-ubuntudebian)
- [RubyGems (Ruby)](#rubygems-ruby)
- [npm (Node.js)](#npm-nodejs)
- [Packagist (PHP/Composer)](#packagist-phpcomposer)
- [GitHub Releases](#github-releases)

---

## 🍺 Homebrew (macOS)

### Step 1: Create Homebrew Tap

```bash
# Create tap repository on GitHub
# Repository name must follow format: homebrew-<tapname>
# Example: homebrew-fastqr

# Clone repository
git clone https://github.com/tranhuucanh/homebrew-fastqr.git
cd homebrew-fastqr
```

### Step 2: Create Formula

Create file `fastqr.rb`:

```ruby
class Fastqr < Formula
  desc "Fast QR code generator with UTF-8 support"
  homepage "https://github.com/tranhuucanh/fastqr"
  url "https://github.com/tranhuucanh/fastqr/archive/v1.0.0.tar.gz"
  sha256 "SHA256_OF_TARBALL"
  license "LGPL-2.1"

  depends_on "cmake" => :build
  depends_on "qrencode"
  depends_on "vips"

  def install
    system "cmake", "-S", ".", "-B", "build", *std_cmake_args
    system "cmake", "--build", "build"
    system "cmake", "--install", "build"
  end

  test do
    system "#{bin}/fastqr", "--version"
    system "#{bin}/fastqr", "Test", "test.png"
    assert_predicate testpath/"test.png", :exist?
  end
end
```

### Step 3: Calculate SHA256

```bash
# Download source tarball
wget https://github.com/tranhuucanh/fastqr/archive/v1.0.0.tar.gz

# Calculate SHA256
shasum -a 256 v1.0.0.tar.gz

# Update SHA256 in formula
```

### Step 4: Test Formula

```bash
# Test installation
brew install --build-from-source fastqr.rb

# Test uninstall
brew uninstall fastqr

# Audit formula
brew audit --strict fastqr.rb
```

### Step 5: Publish

```bash
git add fastqr.rb
git commit -m "Add fastqr formula v1.0.0"
git push origin main
```

### Step 6: User Installation Instructions

```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

**Note**: Homebrew automatically installs dependencies (qrencode and vips), so users don't need to install them manually.

---

## 📦 APT/DEB (Ubuntu/Debian)

### Step 1: Prepare Build Environment

```bash
sudo apt-get install build-essential devscripts debhelper
```

### Step 2: Create DEB Package Structure

```bash
cd fastqr
mkdir -p debian

# Create required files
```

**debian/control:**

```
Source: fastqr
Section: utils
Priority: optional
Maintainer: Your Name <your.email@example.com>
Build-Depends: debhelper (>= 10), cmake, libqrencode-dev, libvips-dev
Standards-Version: 4.1.3
Homepage: https://github.com/tranhuucanh/fastqr

Package: fastqr
Architecture: any
Depends: ${shlibs:Depends}, ${misc:Depends}, libqrencode4, libvips42
Description: Fast QR code generator with UTF-8 support
 FastQR is a fast QR code generator with full UTF-8 support,
 custom colors, logo embedding, and precise size control.
```

**debian/changelog:**

```
fastqr (1.0.0-1) unstable; urgency=medium

  * Initial release

 -- Your Name <your.email@example.com>  Sat, 18 Oct 2025 10:00:00 +0000
```

**debian/rules:**

```makefile
#!/usr/bin/make -f
%:
	dh $@

override_dh_auto_configure:
	dh_auto_configure -- -DCMAKE_BUILD_TYPE=Release
```

**debian/compat:**

```
10
```

### Step 3: Build Package

```bash
# Build binary package
dpkg-buildpackage -us -uc -b

# Package will be created in parent directory
cd ..
ls fastqr_1.0.0-1_amd64.deb
```

### Step 4: Test Package

```bash
# Install
sudo dpkg -i fastqr_1.0.0-1_amd64.deb

# Test
fastqr --version

# Uninstall
sudo apt-get remove fastqr
```

### Step 5: Upload to GitHub Releases

```bash
# Upload .deb file to GitHub Releases
# Users can download and install:
wget https://github.com/tranhuucanh/fastqr/releases/download/v1.0.0/fastqr_1.0.0-1_amd64.deb
sudo dpkg -i fastqr_1.0.0-1_amd64.deb
```

---

## 💎 RubyGems (Ruby)

### Step 1: Prepare Gemspec

File `fastqr.gemspec` is already prepared. Review it:

```ruby
# fastqr.gemspec
Gem::Specification.new do |spec|
  spec.name          = "fastqr"
  spec.version       = "1.0.0"
  # ... other fields
end
```

### Step 2: Build Gem with Pre-built Binaries

```bash
# First, build binaries for all platforms using GitHub Actions
# or build locally using scripts/build-binaries.sh

# Build gem (includes pre-built binaries)
gem build fastqr.gemspec

# Test install locally
gem install fastqr-1.0.0.gem

# Test
ruby -r fastqr -e "puts FastQR.version"

# Uninstall
gem uninstall fastqr
```

**Important**: The gem now includes pre-built binaries, so users don't need to install libqrencode or libvips separately!

### Step 3: Create RubyGems Account

```bash
# Register account at: https://rubygems.org/sign_up

# Setup credentials
gem push  # First time will ask for login
```

### Step 4: Publish to RubyGems

```bash
# Push gem
gem push fastqr-1.0.0.gem

# Gem will be available at: https://rubygems.org/gems/fastqr
```

### Step 5: Verify

```bash
# Users can now install with just:
gem install fastqr

# No need to install qrencode or vips!

# Or via Gemfile:
# gem 'fastqr', '~> 1.0'
```

### Update Version

```bash
# 1. Update version in files:
#    - fastqr.gemspec
#    - bindings/ruby/lib/fastqr/version.rb

# 2. Rebuild binaries using GitHub Actions

# 3. Rebuild and push gem
gem build fastqr.gemspec
gem push fastqr-1.0.1.gem
```

---

## 📦 npm (Node.js)

### Step 1: Prepare Package

File `bindings/nodejs/package.json` is already prepared. Review it:

```json
{
  "name": "fastqr",
  "version": "1.0.0",
  "description": "Fast QR code generator...",
  "main": "index.js"
}
```

### Step 2: Test Package Locally

```bash
cd bindings/nodejs

# Install dependencies
npm install

# Test (will use pre-built binaries if available)
node test/test.js

# Test installation from tarball
npm pack
npm install fastqr-1.0.0.tgz
```

**Important**: The npm package now includes pre-built binaries, so users don't need to install libqrencode or libvips separately!

### Step 3: Create npm Account

```bash
# Register at: https://www.npmjs.com/signup

# Login
npm login
```

### Step 4: Publish to npm

```bash
cd bindings/nodejs

# Ensure pre-built binaries are included
# (GitHub Actions should have built them)

# Publish
npm publish

# Package will be available at: https://www.npmjs.com/package/fastqr
```

### Step 5: Verify

```bash
# Users can now install with just:
npm install fastqr

# No need to install qrencode or vips!

# Or via package.json:
# "dependencies": {
#   "fastqr": "^1.0.0"
# }
```

### Update Version

```bash
# 1. Update version
cd bindings/nodejs
npm version patch  # or minor, or major

# 2. Rebuild binaries using GitHub Actions

# 3. Publish
npm publish

# 4. Push git tag
git push --tags
```

### npm Scripts Useful

```bash
# Unpublish (within 72 hours)
npm unpublish fastqr@1.0.0

# Deprecate old version
npm deprecate fastqr@1.0.0 "Please upgrade to 1.0.1"

# View package info
npm info fastqr
```

---

## 📦 Packagist (PHP/Composer)

### Step 1: Prepare composer.json

File `composer.json` is already prepared at root. Review it:

```json
{
  "name": "fastqr/fastqr",
  "type": "library",
  "license": "LGPL-2.1"
}
```

### Step 2: Create Packagist Account

1. Visit: https://packagist.org/
2. Register account
3. Link with GitHub account

### Step 3: Submit Package

1. Visit: https://packagist.org/packages/submit
2. Enter GitHub repository URL: `https://github.com/tranhuucanh/fastqr`
3. Click "Check"
4. If valid, click "Submit"

**Important**: The composer package now includes pre-built binaries in the repository (via GitHub Actions), so users don't need to install libqrencode or libvips separately!

### Step 4: Setup Auto-Update (Recommended)

**Option 1: GitHub Service Hook (Recommended)**

1. Go to Packagist package page
2. Copy API Token
3. Go to GitHub repo → Settings → Webhooks → Add webhook
4. Payload URL: `https://packagist.org/api/github?username=PACKAGIST_USERNAME`
5. Content type: `application/json`
6. Secret: Paste API token
7. Events: "Just the push event"

**Option 2: Manual Update**

```bash
# Update package manually on Packagist
# Visit: https://packagist.org/packages/fastqr/fastqr
# Click "Update"
```

### Step 5: Tag Release

```bash
# Create and push tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0

# Packagist will auto-detect new tag (if webhook is configured)
```

### Step 6: Verify

```bash
# Users can now install with just:
composer require fastqr/fastqr

# No need to install qrencode or vips!

# Or via composer.json:
# "require": {
#   "fastqr/fastqr": "^1.0"
# }
```

### Update Version

```bash
# 1. Update version in composer.json (optional, Packagist uses git tags)
# 2. Commit changes
git commit -am "Update to v1.0.1"

# 3. Rebuild binaries using GitHub Actions

# 4. Create new tag
git tag -a v1.0.1 -m "Release version 1.0.1"
git push origin v1.0.1

# Packagist will auto-update if webhook is configured
```

---

## 🚀 GitHub Releases

### Step 1: Create Tag

```bash
# Create tag
git tag -a v1.0.0 -m "Release version 1.0.0"
git push origin v1.0.0
```

### Step 2: Create Release on GitHub

1. Go to: `https://github.com/tranhuucanh/fastqr/releases/new`
2. Choose tag: `v1.0.0`
3. Release title: `FastQR v1.0.0`
4. Description: Release notes
5. Upload assets:
   - Source code (auto)
   - `fastqr-1.0.0-linux-x64.deb`
   - `fastqr-1.0.0-macos-arm64.tar.gz`
   - `fastqr-1.0.0.gem`
   - `fastqr-1.0.0.tgz` (npm)
   - `fastqr-binaries-all.tar.gz` (all pre-built binaries)
6. Click "Publish release"

### Step 3: Build and Upload Assets

GitHub Actions workflow will automatically build and upload assets when you push a tag.

Alternatively, build manually:

```bash
# Build DEB package
dpkg-buildpackage -us -uc -b
mv ../fastqr_1.0.0-1_amd64.deb ./fastqr-1.0.0-linux-x64.deb

# Build binaries for all platforms
./scripts/build-binaries.sh

# Build Ruby gem (with binaries)
gem build fastqr.gemspec

# Build npm package (with binaries)
cd bindings/nodejs
npm pack
cd ../..

# Upload via GitHub UI or use GitHub CLI:
gh release upload v1.0.0 \
  fastqr-1.0.0-linux-x64.deb \
  fastqr-1.0.0-macos-arm64.tar.gz \
  fastqr-1.0.0.gem \
  bindings/nodejs/fastqr-1.0.0.tgz
```

---

## 📝 Release Checklist

Before releasing a new version:

- [ ] Update version in all files:
  - [ ] `CMakeLists.txt` (project VERSION)
  - [ ] `include/fastqr.h` (FASTQR_VERSION)
  - [ ] `src/fastqr.cpp` (FASTQR_VERSION)
  - [ ] `fastqr.gemspec` (spec.version)
  - [ ] `bindings/ruby/lib/fastqr/version.rb`
  - [ ] `bindings/nodejs/package.json` (version)
  - [ ] `composer.json` (version in description)

- [ ] Update CHANGELOG.md with new changes

- [ ] Run tests:
  ```bash
  cd build
  make test
  cd ../bindings/ruby && rake test
  cd ../nodejs && npm test
  cd ../php && vendor/bin/phpunit
  ```

- [ ] Build pre-compiled binaries for all platforms:
  ```bash
  # Push tag to trigger GitHub Actions
  git tag -a v1.0.1 -m "Release version 1.0.1"
  git push origin v1.0.1
  # GitHub Actions will build binaries automatically
  ```

- [ ] Test on all platforms:
  - [ ] macOS (Intel + ARM)
  - [ ] Ubuntu 20.04, 22.04
  - [ ] Debian 11, 12

- [ ] Commit changes:
  ```bash
  git commit -am "Bump version to 1.0.1"
  ```

- [ ] Publish to package managers:
  - [ ] Homebrew (update formula)
  - [ ] RubyGems: `gem push fastqr-1.0.1.gem`
  - [ ] npm: `npm publish`
  - [ ] Packagist (auto via webhook)

- [ ] Create GitHub Release with assets

- [ ] Announce on:
  - [ ] GitHub Discussions
  - [ ] Twitter/X
  - [ ] Reddit (r/ruby, r/node, r/PHP)

---

## 🆘 Troubleshooting

### Homebrew

**Problem**: Formula fails audit

```bash
# Fix common issues:
brew audit --strict --online fastqr.rb

# Check style:
brew style fastqr.rb
```

**Problem**: Wrong SHA256

```bash
# Recalculate:
shasum -a 256 v1.0.0.tar.gz
```

### RubyGems

**Problem**: Gem build fails

```bash
# Check gemspec:
gem spec fastqr.gemspec

# Validate:
gem build --verbose fastqr.gemspec
```

**Problem**: Pre-built binary not loading

```bash
# Check if binaries are included:
gem unpack fastqr-1.0.0.gem
ls fastqr-1.0.0/bindings/ruby/prebuilt/

# Check platform detection:
ruby -r fastqr/platform -e "puts FastQR::Platform.platform"
```

### npm

**Problem**: Native addon fails to build

```bash
# Should use pre-built binary instead
# Check if binaries are included:
npm pack --dry-run | grep prebuilt

# Check platform detection:
node -e "const p = require('./lib/platform'); console.log(p.detectPlatform())"
```

**Problem**: Module not found

```bash
# Check package structure:
npm pack --dry-run
```

### PHP/Composer

**Problem**: Packagist not updating

```bash
# Check webhook:
# GitHub → Settings → Webhooks → Recent Deliveries

# Manual update:
# Visit: https://packagist.org/packages/fastqr/fastqr
# Click "Update"
```

**Problem**: FFI library not found

```bash
# Check if binaries are in composer package:
composer show -i fastqr/fastqr
ls vendor/fastqr/fastqr/bindings/php/prebuilt/

# Check platform detection:
php -r "echo PHP_OS_FAMILY;"
```

**Problem**: FFI extension not available

```bash
# Check PHP FFI extension:
php -m | grep ffi

# Install if missing:
# Ubuntu: sudo apt-get install php-ffi
# macOS: brew install php (includes FFI)
```

---

## 📚 Resources

- [Homebrew Formula Cookbook](https://docs.brew.sh/Formula-Cookbook)
- [Debian Packaging Tutorial](https://www.debian.org/doc/manuals/maint-guide/)
- [RubyGems Guides](https://guides.rubygems.org/)
- [npm Publishing Guide](https://docs.npmjs.com/packages-and-modules/contributing-packages-to-the-registry)
- [Packagist Documentation](https://packagist.org/about)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)

---

## 🎯 Pre-built Binaries

Starting from v1.0.0, FastQR includes pre-built binaries for:

- **macOS**: arm64 (Apple Silicon), x86_64 (Intel)
- **Linux**: x86_64, arm64

These binaries are automatically bundled in:
- Ruby gems
- npm packages
- Composer packages (committed to repository)

**This means users can install with just:**
- `gem install fastqr` - no system dependencies needed!
- `npm install fastqr` - no system dependencies needed!
- `composer require fastqr/fastqr` - no system dependencies needed!

The binaries are built automatically by GitHub Actions on every tagged release.

---

**Important Note**: For LGPL license compliance, when distributing binary packages:

1. Include LICENSE file
2. Include link to source code
3. Include BUILD.md with build instructions
4. State that the package uses libqrencode and libvips

This ensures LGPL requirements are met for static linking.
