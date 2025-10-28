# FastQR Release Scripts

This directory contains scripts for building and releasing FastQR.

## Scripts

### 1. `build-binaries.sh`
Build pre-compiled binaries for distribution.

**Usage:**
```bash
./scripts/build-binaries.sh
```

**What it does:**
- Builds static binaries for the current platform
- Creates tarball in `prebuilt/` directory
- Used by GitHub Actions for releases

---

### 2. `update-version.sh`
Update version number across all project files.

**Usage:**
```bash
./scripts/update-version.sh <version>
```

**What it updates:**
- `VERSION` file
- `CMakeLists.txt`
- `fastqr.gemspec`
- `bindings/ruby/lib/fastqr/version.rb`
- `bindings/nodejs/package.json`
- All `*.md` files (URLs and version references)

**Example:**
```bash
./scripts/update-version.sh 1.0.1
```

**Note:** This is automatically called by `release.sh`, you rarely need to run it manually.

---

### 3. `release.sh`
Release a new version to GitHub.

**Usage:**
```bash
./scripts/release.sh <version>           # Normal release
./scripts/release.sh <version> --force   # Force overwrite existing tag
```

**What it does:**
1. Checks if tag already exists
2. Checks for uncommitted changes
3. Updates version in all files
4. Commits changes
5. Creates git tag
6. Pushes to GitHub
7. Triggers GitHub Actions

**Normal Release Example:**
```bash
# Make your changes
git add .
git commit -m "feat: add new feature"

# Release new version
./scripts/release.sh 1.0.1
```

**Force Release Example:**
```bash
# If you need to re-release the same version:
# 1. Manually delete tag and release on GitHub
# 2. Run with --force flag
./scripts/release.sh 1.0.1 --force
```

---

## Release Workflow

### For New Version

```bash
# 1. Make your changes and commit
git add .
git commit -m "feat: your changes"

# 2. Test locally (optional)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
./fastqr "Test" test.png
cd ..

# 3. Release new version
./scripts/release.sh 1.0.1
```

GitHub Actions will automatically:
- Build binaries for all platforms
- Create GitHub Release
- Update Homebrew formula
- Publish to package registries

### For Overwriting Version

```bash
# 1. Delete tag and release on GitHub manually
# Go to: https://github.com/tranhuucanh/fastqr/releases
# - Delete the release
# - Delete the tag (Settings → Tags)

# 2. Force release locally
./scripts/release.sh 1.0.0 --force
```

---

## Version Management

FastQR uses a single-source-of-truth approach for versioning:

- **Source:** `VERSION` file
- **Updated by:** `update-version.sh`
- **Used in:** All build scripts and CI/CD

### Version Format

Semantic Versioning: `MAJOR.MINOR.PATCH`

Examples:
- `1.0.0` - Initial release
- `1.0.1` - Bug fixes
- `1.1.0` - New features (backward compatible)
- `2.0.0` - Breaking changes

---

## Troubleshooting

### "Tag already exists" error
```bash
./scripts/release.sh 1.0.1
# Error: Tag v1.0.1 already exists!
```

**Solution:**
1. If you want a new version, use a different number: `1.0.2`
2. If you want to overwrite, delete on GitHub first, then use `--force`:
   ```bash
   ./scripts/release.sh 1.0.1 --force
   ```

### "Uncommitted changes" error
```bash
./scripts/release.sh 1.0.1
# Error: You have uncommitted changes!
```

**Solution:**
```bash
git status  # Check what's changed
git add .
git commit -m "your message"
./scripts/release.sh 1.0.1
```

### GitHub Actions not triggering
- Check: https://github.com/tranhuucanh/fastqr/actions
- Ensure tag was pushed: `git push origin v1.0.1`
- Check workflow permissions in repo settings

### Homebrew formula not updating
- Check Actions log: https://github.com/tranhuucanh/fastqr/actions
- Ensure `HOMEBREW_TAP_TOKEN` secret is set in repo settings
- Check formula repo: https://github.com/tranhuucanh/homebrew-fastqr

---

## CI/CD Pipeline

```
Push tag v1.0.1
    ↓
GitHub Actions triggered
    ↓
┌─────────────────────────────┐
│ 1. Build binaries           │
│    - macOS (arm64, x86_64)  │
│    - Linux (x86_64, arm64)  │
└─────────────┬───────────────┘
              ↓
┌─────────────────────────────┐
│ 2. Create GitHub Release    │
│    - Upload binaries        │
│    - Generate changelog     │
└─────────────┬───────────────┘
              ↓
┌─────────────────────────────┐
│ 3. Update Homebrew formula  │
│    - Download source tarball│
│    - Calculate SHA256       │
│    - Update Formula/fastqr.rb│
│    - Push to homebrew-fastqr│
└─────────────────────────────┘
```

---

## Files Modified by Release

When you run `./scripts/release.sh 1.0.1`:

```
VERSION                                 → 1.0.1
CMakeLists.txt                          → project(fastqr VERSION 1.0.1 ...)
fastqr.gemspec                          → spec.version = '1.0.1'
bindings/ruby/lib/fastqr/version.rb     → VERSION = '1.0.1'
bindings/nodejs/package.json            → "version": "1.0.1"
README.md                               → /v1.0.1/fastqr-1.0.1-...
docs/*.md                               → /v1.0.1/fastqr-1.0.1-...
```

All changes are committed with message: `chore: bump version to v1.0.1`

---

## See Also

- [README.md](../README.md) - Project overview
- [CONTRIBUTING.md](../CONTRIBUTING.md) - Contributing guidelines
- [CHANGELOG.md](../CHANGELOG.md) - Version history

