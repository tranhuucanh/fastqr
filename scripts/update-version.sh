#!/bin/bash
# Update version across all project files

set -e

if [ -z "$1" ]; then
    echo "Usage: $0 <version>"
    echo "Example: $0 1.0.1"
    exit 1
fi

NEW_VERSION="$1"

echo "🔄 Updating version to $NEW_VERSION..."

# Update VERSION file
echo "$NEW_VERSION" > VERSION

# Update CMakeLists.txt
sed -i.bak "s/project(fastqr VERSION [0-9.]\+ /project(fastqr VERSION $NEW_VERSION /" CMakeLists.txt
rm -f CMakeLists.txt.bak

# Update src/fastqr.cpp
sed -i.bak "s/#define FASTQR_VERSION \"[^\"]*\"/#define FASTQR_VERSION \"$NEW_VERSION\"/" src/fastqr.cpp
rm -f src/fastqr.cpp.bak

# Update Ruby version
sed -i.bak "s/VERSION = \"[^\"]*\"/VERSION = \"$NEW_VERSION\"/" bindings/ruby/lib/fastqr/version.rb
rm -f bindings/ruby/lib/fastqr/version.rb.bak

# Update gemspec
sed -i.bak "s/spec.version[[:space:]]*=[[:space:]]*\"[^\"]*\"/spec.version       = \"$NEW_VERSION\"/" fastqr.gemspec
rm -f fastqr.gemspec.bak

# Update Node.js package.json
sed -i.bak "s/\"version\": \"[^\"]*\"/\"version\": \"$NEW_VERSION\"/" bindings/nodejs/package.json
rm -f bindings/nodejs/package.json.bak

# Update composer.json (PHP)
if [ -f composer.json ]; then
    sed -i.bak "s/\"version\": \"[^\"]*\"/\"version\": \"$NEW_VERSION\"/" composer.json
    rm -f composer.json.bak
fi

# Update documentation
for doc in README.md docs/*.md; do
    if [ -f "$doc" ]; then
        # Update version in installation examples
        sed -i.bak "s/v[0-9]\+\.[0-9]\+\.[0-9]\+/v$NEW_VERSION/g" "$doc"
        sed -i.bak "s/fastqr-[0-9]\+\.[0-9]\+\.[0-9]\+/fastqr-$NEW_VERSION/g" "$doc"
        rm -f "$doc.bak"
    fi
done

echo "✅ Version updated to $NEW_VERSION in all files"
echo ""
echo "Changed files:"
git diff --name-only
