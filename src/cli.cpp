/*
 * FastQR - Fast QR Code Generator Library
 * Copyright (C) 2025 FastQR Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 */

#include "fastqr.h"
#include <iostream>
#include <cstring>
#include <cstdlib>

void print_usage(const char* program_name) {
    std::cout << "FastQR v" << fastqr::version() << " - Fast QR Code Generator\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS] <data> <output_file>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -s, --size WxH          Output size in pixels (default: 300x300)\n";
    std::cout << "  -f, --foreground R,G,B  QR code color (default: 0,0,0)\n";
    std::cout << "  -b, --background R,G,B  Background color (default: 255,255,255)\n";
    std::cout << "  -e, --error-level L|M|Q|H  Error correction level (default: M)\n";
    std::cout << "  -l, --logo PATH         Path to logo image\n";
    std::cout << "  -p, --logo-size N       Logo size percentage (default: 20)\n";
    std::cout << "  -q, --quality N         Image quality 1-100 (default: 95)\n";
    std::cout << "  -h, --help              Show this help\n";
    std::cout << "  -v, --version           Show version\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " \"Hello World\" output.png\n";
    std::cout << "  " << program_name << " -s 500x500 -f 255,0,0 \"Red QR\" red_qr.png\n";
    std::cout << "  " << program_name << " -l logo.png \"Company\" qr_with_logo.png\n";
}

bool parse_color(const char* str, fastqr::QROptions::Color& color) {
    int r, g, b;
    if (sscanf(str, "%d,%d,%d", &r, &g, &b) != 3) {
        return false;
    }
    if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255) {
        return false;
    }
    color.r = static_cast<uint8_t>(r);
    color.g = static_cast<uint8_t>(g);
    color.b = static_cast<uint8_t>(b);
    return true;
}

bool parse_size(const char* str, int& width, int& height) {
    if (sscanf(str, "%dx%d", &width, &height) != 2) {
        return false;
    }
    if (width <= 0 || height <= 0 || width > 10000 || height > 10000) {
        return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    fastqr::QROptions options;
    std::string data;
    std::string output_path;

    // Parse arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "-h" || arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "FastQR v" << fastqr::version() << std::endl;
            return 0;
        } else if (arg == "-s" || arg == "--size") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            if (!parse_size(argv[i], options.width, options.height)) {
                std::cerr << "Error: Invalid size format. Use WxH (e.g., 300x300)\n";
                return 1;
            }
        } else if (arg == "-f" || arg == "--foreground") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            if (!parse_color(argv[i], options.foreground)) {
                std::cerr << "Error: Invalid color format. Use R,G,B (e.g., 255,0,0)\n";
                return 1;
            }
        } else if (arg == "-b" || arg == "--background") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            if (!parse_color(argv[i], options.background)) {
                std::cerr << "Error: Invalid color format. Use R,G,B (e.g., 255,255,255)\n";
                return 1;
            }
        } else if (arg == "-e" || arg == "--error-level") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            std::string level = argv[i];
            if (level == "L") options.ec_level = fastqr::ErrorCorrectionLevel::LOW;
            else if (level == "M") options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM;
            else if (level == "Q") options.ec_level = fastqr::ErrorCorrectionLevel::QUARTILE;
            else if (level == "H") options.ec_level = fastqr::ErrorCorrectionLevel::HIGH;
            else {
                std::cerr << "Error: Invalid error level. Use L, M, Q, or H\n";
                return 1;
            }
        } else if (arg == "-l" || arg == "--logo") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            options.logo_path = argv[i];
        } else if (arg == "-p" || arg == "--logo-size") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            options.logo_size_percent = atoi(argv[i]);
            if (options.logo_size_percent < 1 || options.logo_size_percent > 50) {
                std::cerr << "Error: Logo size must be between 1 and 50\n";
                return 1;
            }
        } else if (arg == "-q" || arg == "--quality") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            options.quality = atoi(argv[i]);
            if (options.quality < 1 || options.quality > 100) {
                std::cerr << "Error: Quality must be between 1 and 100\n";
                return 1;
            }
        } else if (arg[0] == '-') {
            std::cerr << "Error: Unknown option: " << arg << std::endl;
            return 1;
        } else {
            // Non-option arguments
            if (data.empty()) {
                data = arg;
            } else if (output_path.empty()) {
                output_path = arg;
            } else {
                std::cerr << "Error: Too many arguments\n";
                return 1;
            }
        }
    }

    // Validate required arguments
    if (data.empty() || output_path.empty()) {
        std::cerr << "Error: Missing required arguments\n";
        print_usage(argv[0]);
        return 1;
    }

    // Generate QR code
    if (!fastqr::generate(data, output_path, options)) {
        std::cerr << "Error: Failed to generate QR code\n";
        return 1;
    }

    // Success - no output for performance
    return 0;
}

