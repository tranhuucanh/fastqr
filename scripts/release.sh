#!/bin/bash
# Release new version to GitHub
# Usage:
#   ./scripts/release.sh <version>           # Normal release
#   ./scripts/release.sh <version> --force   # Force overwrite existing tag

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <version> [--force]"
    echo ""
    echo "Examples:"
    echo "  $0 1.0.1           # Release new version"
    echo "  $0 1.0.0 --force   # Force overwrite existing tag (after manual deletion on GitHub)"
    exit 1
fi

NEW_VERSION="$1"
FORCE_FLAG="$2"

echo "🚀 FastQR Release Script"
echo "========================"
echo ""
echo "📦 Target version: v$NEW_VERSION"

# Check if version tag already exists
if git rev-parse "v$NEW_VERSION" >/dev/null 2>&1; then
    if [ "$FORCE_FLAG" != "--force" ]; then
        echo ""
        echo "❌ Error: Tag v$NEW_VERSION already exists!"
        echo ""
        echo "If you want to overwrite:"
        echo "  1. Manually delete the tag and release on GitHub"
        echo "  2. Run: $0 $NEW_VERSION --force"
        exit 1
    else
        echo "⚠️  Force mode: Deleting existing local tag..."
        git tag -d "v$NEW_VERSION" 2>/dev/null || true
        echo "✅ Local tag deleted"
    fi
fi

# Check for uncommitted changes
if ! git diff-index --quiet HEAD --; then
    echo ""
    echo "❌ Error: You have uncommitted changes!"
    echo ""
    echo "Please commit your changes first:"
    echo "  git add ."
    echo "  git commit -m 'Your commit message'"
    exit 1
fi

# Update version in all files
echo ""
echo "📝 Updating version in all files..."
./scripts/update-version.sh "$NEW_VERSION"

# Commit version changes
echo ""
echo "💾 Committing version changes..."
git add VERSION CMakeLists.txt fastqr.gemspec bindings/ composer.json README.md INSTALL.md docs/
git commit -m "chore: bump version to v$NEW_VERSION" || echo "No changes to commit"

# Create and push tag
echo ""
echo "🏷️  Creating tag v$NEW_VERSION..."
git tag -a "v$NEW_VERSION" -m "Release v$NEW_VERSION"

echo ""
echo "📤 Pushing to GitHub..."
if [ "$FORCE_FLAG" == "--force" ]; then
    git push origin master --force-with-lease
    git push origin "v$NEW_VERSION" --force
else
    git push origin master
    git push origin "v$NEW_VERSION"
fi

echo ""
echo "✅ Release v$NEW_VERSION completed!"
echo ""
echo "GitHub Actions will now:"
echo "  1. Build binaries for macOS (arm64 & x86_64) and Linux (x86_64 & arm64)"
echo "  2. Create GitHub Release with binaries"
echo "  3. Update Homebrew formula (version + sha256)"
echo ""
echo "Monitor progress:"
echo "  https://github.com/tranhuucanh/fastqr/actions"
echo ""
echo "After Actions complete, users can install:"
echo "  brew upgrade fastqr  # macOS"
echo "  gem update fastqr    # Ruby"
echo "  npm update fastqr    # Node.js"
echo "  composer update fastqr/fastqr  # PHP"

