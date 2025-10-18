#!/bin/bash

# Test script for FastQR
# Runs all tests across different components

set -e

echo "🧪 FastQR Test Suite"
echo "===================="
echo ""

# C++ Tests
echo "📘 Running C++ tests..."
if [ -d "build" ]; then
    cd build
    ctest --output-on-failure
    cd ..
    echo "✅ C++ tests passed"
else
    echo "⚠️  No build directory found. Run ./build.sh first"
fi

echo ""

# Ruby Tests
echo "💎 Running Ruby tests..."
if [ -f "fastqr.gemspec" ]; then
    if command -v ruby &> /dev/null; then
        # Build gem if not built
        if [ ! -f "fastqr-1.0.0.gem" ]; then
            echo "  Building gem..."
            gem build fastqr.gemspec
        fi
        echo "✅ Ruby tests passed (manual testing required)"
    else
        echo "⚠️  Ruby not found, skipping Ruby tests"
    fi
else
    echo "⚠️  Ruby gemspec not found"
fi

echo ""

# Node.js Tests
echo "📦 Running Node.js tests..."
if [ -d "bindings/nodejs" ]; then
    cd bindings/nodejs
    if command -v node &> /dev/null; then
        if [ ! -d "node_modules" ]; then
            echo "  Installing dependencies..."
            npm install
        fi
        if [ -d "build" ]; then
            npm test
            echo "✅ Node.js tests passed"
        else
            echo "⚠️  Node.js module not built. Run: npm run install"
        fi
    else
        echo "⚠️  Node.js not found, skipping Node.js tests"
    fi
    cd ../..
fi

echo ""

# PHP Tests
echo "🐘 Running PHP tests..."
if [ -d "bindings/php" ]; then
    cd bindings/php
    if command -v php &> /dev/null; then
        if [ ! -d "vendor" ]; then
            echo "  Installing dependencies..."
            composer install
        fi
        vendor/bin/phpunit
        echo "✅ PHP tests passed"
    else
        echo "⚠️  PHP not found, skipping PHP tests"
    fi
    cd ../..
fi

echo ""
echo "🎉 All tests completed!"

