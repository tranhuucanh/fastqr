/*
 * FastQR - Fast QR Code Generator Library
 * Copyright (C) 2025 Tran Huu Canh and FastQR Contributors
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, see <https://www.gnu.org/licenses/>.
 *
 * Homepage: https://github.com/tranhuucanh/fastqr
 */

const fastqr = require('./index');

console.log('FastQR Node.js Test');
console.log('Version:', fastqr.version());
console.log('');

try {
    // Basic test
    console.log('Test 1: Basic QR code...');
    fastqr.generate('Hello, FastQR from Node.js!', 'test_basic.png');
    console.log('✓ Generated test_basic.png');

    // Colored QR
    console.log('\nTest 2: Colored QR code...');
    fastqr.generate('Colored QR', 'test_colored.png', {
        width: 500,
        height: 500,
        foreground: [255, 0, 0],
        background: [255, 255, 200]
    });
    console.log('✓ Generated test_colored.png');

    // UTF-8 Vietnamese
    console.log('\nTest 3: Vietnamese text...');
    fastqr.generate('Xin chào Việt Nam! 🇻🇳', 'test_vietnamese.png', {
        width: 400,
        height: 400
    });
    console.log('✓ Generated test_vietnamese.png');

    // UTF-8 Japanese
    console.log('\nTest 4: Japanese text...');
    fastqr.generate('こんにちは日本', 'test_japanese.png', {
        width: 400,
        height: 400,
        foreground: [0, 0, 255]
    });
    console.log('✓ Generated test_japanese.png');

    console.log('\n✓ All tests passed!');
} catch (error) {
    console.error('✗ Test failed:', error.message);
    process.exit(1);
}

