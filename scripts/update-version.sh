#!/bin/bash
# Update version in all files
# Usage: ./scripts/update-version.sh NEW_VERSION

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 1.0.1"
    exit 1
fi

NEW_VERSION="$1"
echo "🔄 Updating version to: $NEW_VERSION"

# Update VERSION file
echo "$NEW_VERSION" > VERSION
echo "✅ Updated VERSION file"

# Update CMakeLists.txt
sed -i.bak "s/project(fastqr VERSION [0-9]\+\.[0-9]\+\.[0-9]\+/project(fastqr VERSION $NEW_VERSION/" CMakeLists.txt
rm -f CMakeLists.txt.bak
echo "✅ Updated CMakeLists.txt"

# Update fastqr.gemspec
if [ -f "fastqr.gemspec" ]; then
    sed -i.bak "s/spec.version[[:space:]]*=[[:space:]]*['\"][0-9]\+\.[0-9]\+\.[0-9]\+['\"]/spec.version = '$NEW_VERSION'/" fastqr.gemspec
    rm -f fastqr.gemspec.bak
    echo "✅ Updated fastqr.gemspec"
fi

# Update bindings/ruby/lib/fastqr/version.rb
if [ -f "bindings/ruby/lib/fastqr/version.rb" ]; then
    sed -i.bak "s/VERSION = ['\"][0-9]\+\.[0-9]\+\.[0-9]\+['\"]/VERSION = '$NEW_VERSION'/" bindings/ruby/lib/fastqr/version.rb
    rm -f bindings/ruby/lib/fastqr/version.rb.bak
    echo "✅ Updated bindings/ruby/lib/fastqr/version.rb"
fi

# Update bindings/nodejs/package.json
if [ -f "bindings/nodejs/package.json" ]; then
    sed -i.bak "s/\"version\": \"[0-9]\+\.[0-9]\+\.[0-9]\+\"/\"version\": \"$NEW_VERSION\"/" bindings/nodejs/package.json
    rm -f bindings/nodejs/package.json.bak
    echo "✅ Updated bindings/nodejs/package.json"
fi

# Update composer.json
if [ -f "composer.json" ]; then
    sed -i.bak "s/\"version\": \"[0-9]\+\.[0-9]\+\.[0-9]\+\"/\"version\": \"$NEW_VERSION\"/" composer.json
    rm -f composer.json.bak
    echo "✅ Updated composer.json"
fi

# Update all markdown files (v1.0.0 → vX.Y.Z)
echo "🔄 Updating markdown files..."
for md_file in README.md INSTALL.md docs/*.md; do
    if [ -f "$md_file" ]; then
        # Update download URLs (v1.0.0 → vX.Y.Z)
        sed -i.bak "s|/v[0-9]\+\.[0-9]\+\.[0-9]\+/|/v$NEW_VERSION/|g" "$md_file"
        # Update version references (e.g., "1.0.0" in text)
        sed -i.bak "s/fastqr-[0-9]\+\.[0-9]\+\.[0-9]\+/fastqr-$NEW_VERSION/g" "$md_file"
        rm -f "$md_file.bak"
    fi
done
echo "✅ Updated markdown files"

echo ""
echo "✅ All files updated to version $NEW_VERSION"
echo ""
echo "Changed files:"
git status --short | grep -E "VERSION|CMakeLists.txt|gemspec|package.json|composer.json|\.md$" || echo "  (none)"

