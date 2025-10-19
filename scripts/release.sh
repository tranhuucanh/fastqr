#!/bin/bash
# Create and push a new release

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <version> [--force]"
    echo "Example: $0 1.0.1"
    echo ""
    echo "Options:"
    echo "  --force    Force re-tag if tag already exists (use with caution!)"
    exit 1
fi

VERSION="$1"
FORCE_FLAG=""

if [ "$2" = "--force" ]; then
    FORCE_FLAG="--force"
    echo "⚠️  Force mode enabled - will overwrite existing tag if present"
fi

echo "🚀 Releasing FastQR v$VERSION..."
echo ""

# Check if we're on a clean working tree
if [ -n "$(git status --porcelain)" ]; then
    echo "❌ Working directory not clean. Please commit or stash changes first."
    git status --short
    exit 1
fi

# Update version in all files
echo "📝 Updating version strings..."
./scripts/update-version.sh "$VERSION"

# Show what changed
echo ""
echo "📋 Changed files:"
git diff --name-only
echo ""

# Commit version changes
echo "💾 Committing version update..."
git add -A
git commit -m "chore: bump version to $VERSION" || echo "Nothing to commit"

# Create tag
echo "🏷️  Creating tag v$VERSION..."
if git rev-parse "v$VERSION" >/dev/null 2>&1; then
    if [ -z "$FORCE_FLAG" ]; then
        echo "❌ Tag v$VERSION already exists!"
        echo "   Use --force to overwrite (not recommended for published versions)"
        exit 1
    else
        echo "⚠️  Deleting existing tag v$VERSION..."
        git tag -d "v$VERSION"
    fi
fi

git tag -a "v$VERSION" -m "Release v$VERSION"

# Push
echo "📤 Pushing to GitHub..."
git push origin main $FORCE_FLAG
git push origin "v$VERSION" $FORCE_FLAG

echo ""
echo "✅ Release v$VERSION created successfully!"
echo ""
echo "📊 Next steps:"
echo "1. Monitor GitHub Actions: https://github.com/tranhuucanh/fastqr/actions"
echo "2. Check npm: https://www.npmjs.com/package/fastqr"
echo "3. Check RubyGems: https://rubygems.org/gems/fastqr"
echo "4. Check Packagist: https://packagist.org/packages/fastqr/fastqr"
echo "5. Check Homebrew: brew upgrade fastqr"
