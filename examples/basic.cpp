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

#include "fastqr.h"
#include <iostream>

int main() {
    std::cout << "FastQR Basic Example\n\n";

    // Basic usage
    fastqr::QROptions options;
    options.size = 400;

    if (fastqr::generate("Hello, FastQR!", "basic_qr.png", options)) {
        std::cout << "✓ Generated basic_qr.png\n";
    }

    // Colored QR code
    options.foreground = {255, 0, 0};  // Red
    options.background = {255, 255, 200};  // Light yellow

    if (fastqr::generate("Colored QR Code", "colored_qr.png", options)) {
        std::cout << "✓ Generated colored_qr.png\n";
    }

    // UTF-8 support - Vietnamese
    if (fastqr::generate("Xin chào Việt Nam! 🇻🇳", "vietnamese_qr.png", options)) {
        std::cout << "✓ Generated vietnamese_qr.png (Vietnamese text)\n";
    }

    // UTF-8 support - Japanese
    options.foreground = {0, 0, 255};  // Blue
    if (fastqr::generate("こんにちは日本", "japanese_qr.png", options)) {
        std::cout << "✓ Generated japanese_qr.png (Japanese text)\n";
    }

    // High error correction
    options.foreground = {0, 0, 0};
    options.ec_level = fastqr::ErrorCorrectionLevel::HIGH;

    if (fastqr::generate("High Error Correction", "high_ec_qr.png", options)) {
        std::cout << "✓ Generated high_ec_qr.png (with HIGH error correction)\n";
    }

    // Large QR code
    options.size = 2000;
    options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM;

    if (fastqr::generate("Large QR Code - 2000x2000px", "large_qr.png", options)) {
        std::cout << "✓ Generated large_qr.png (2000x2000)\n";
    }

    std::cout << "\nAll examples completed successfully!\n";
    return 0;
}

