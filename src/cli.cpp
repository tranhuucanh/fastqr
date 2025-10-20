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
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <errno.h>

#ifdef _OPENMP
#include <omp.h>
#endif

void print_usage(const char* program_name) {
    std::cout << "FastQR v" << fastqr::version() << " - Fast QR Code Generator\n\n";
    std::cout << "Usage: " << program_name << " [OPTIONS] <data> <output_file>\n";
    std::cout << "       " << program_name << " [OPTIONS] -F <input.txt> <output_dir>\n\n";
    std::cout << "Options:\n";
    std::cout << "  -s, --size SIZE         Output size in pixels (default: 300)\n";
    std::cout << "  -o, --optimize          Auto round-up size for best performance\n";
    std::cout << "  -f, --foreground R,G,B  QR code color (default: 0,0,0)\n";
    std::cout << "  -b, --background R,G,B  Background color (default: 255,255,255)\n";
    std::cout << "  -e, --error-level L|M|Q|H  Error correction level (default: M)\n";
    std::cout << "  -l, --logo PATH         Path to logo image\n";
    std::cout << "  -p, --logo-size N       Logo size percentage (default: 20)\n";
    std::cout << "  -q, --quality N         Image quality 1-100 (default: 95)\n";
    std::cout << "  -F, --file PATH         Batch mode: process text file (one QR per line)\n";
    std::cout << "  -h, --help              Show this help\n";
    std::cout << "  -v, --version           Show version\n\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " \"Hello World\" output.png\n";
    std::cout << "  " << program_name << " -s 500 \"Large QR\" large.png\n";
    std::cout << "  " << program_name << " -s 500 -o \"Optimized\" fast.png\n";
    std::cout << "  " << program_name << " -s 500 -f 255,0,0 \"Red QR\" red_qr.png\n";
    std::cout << "  " << program_name << " -l logo.png \"Company\" qr_with_logo.png\n";
    std::cout << "  " << program_name << " -F batch.txt output_dir/ -s 500 -o\n";
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

bool parse_size(const char* str, int& size) {
    if (sscanf(str, "%d", &size) != 1) {
        return false;
    }
    if (size <= 0 || size > 10000) {
        return false;
    }
    return true;
}

// Create directory recursively
bool mkdir_p(const std::string& path) {
    struct stat st;
    if (stat(path.c_str(), &st) == 0) {
        return S_ISDIR(st.st_mode);
    }

    // Try to create directory
    if (mkdir(path.c_str(), 0755) == 0) {
        return true;
    }

    if (errno != ENOENT) {
        return false;
    }

    // Parent doesn't exist, create it
    size_t pos = path.find_last_of('/');
    if (pos == std::string::npos) {
        return false;
    }

    if (!mkdir_p(path.substr(0, pos))) {
        return false;
    }

    return mkdir(path.c_str(), 0755) == 0;
}

// Read batch file (one QR text per line)
bool read_batch_file(const std::string& filename, std::vector<std::string>& lines) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file: " << filename << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    if (lines.empty()) {
        std::cerr << "Error: File is empty: " << filename << std::endl;
        return false;
    }

    return true;
}

// Process batch with parallel processing
bool process_batch(const std::string& input_file, const std::string& output_dir,
                   const fastqr::QROptions& options) {
    // Read input file
    std::vector<std::string> lines;
    if (!read_batch_file(input_file, lines)) {
        return false;
    }

    // Create output directory
    if (!mkdir_p(output_dir)) {
        std::cerr << "Error: Cannot create directory: " << output_dir << std::endl;
        return false;
    }

    std::cout << "Processing " << lines.size() << " QR codes..." << std::endl;

    int success_count = 0;
    int fail_count = 0;

    // Parallel processing with OpenMP
    #pragma omp parallel for schedule(dynamic, 10) reduction(+:success_count,fail_count)
    for (size_t i = 0; i < lines.size(); i++) {
        // Generate output filename: 1.png, 2.png, ...
        std::string output_path = output_dir;
        if (output_path.back() != '/') {
            output_path += '/';
        }
        output_path += std::to_string(i + 1) + ".png";

        // Generate QR code (reusing single-QR generation - no overhead!)
        if (fastqr::generate(lines[i], output_path, options)) {
            success_count++;
        } else {
            fail_count++;
            #pragma omp critical
            {
                std::cerr << "Error: Failed to generate QR " << (i + 1) << std::endl;
            }
        }
    }

    std::cout << "Done: " << success_count << " success, " << fail_count << " failed" << std::endl;

    return fail_count == 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    fastqr::QROptions options;
    std::string data;
    std::string output_path;
    std::string batch_file;  // For batch mode

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
            if (!parse_size(argv[i], options.size)) {
                std::cerr << "Error: Invalid size format. Use SIZE (e.g., 500)\n";
                return 1;
            }
        } else if (arg == "-o" || arg == "--optimize") {
            options.optimize_size = true;
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
        } else if (arg == "-F" || arg == "--file") {
            if (++i >= argc) {
                std::cerr << "Error: " << arg << " requires an argument\n";
                return 1;
            }
            batch_file = argv[i];
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

    // Batch mode vs single mode
    if (!batch_file.empty()) {
        // Batch mode: --file <input.txt> <output_dir>
        if (data.empty()) {
            std::cerr << "Error: Output directory required for batch mode\n";
            print_usage(argv[0]);
            return 1;
        }

        // In batch mode, first non-option arg is output_dir
        std::string output_dir = data;

        if (!process_batch(batch_file, output_dir, options)) {
            return 1;
        }
    } else {
        // Single mode: <data> <output_file>
        if (data.empty() || output_path.empty()) {
            std::cerr << "Error: Missing required arguments\n";
            print_usage(argv[0]);
            return 1;
        }

        // Generate single QR code (no overhead - same performance as before!)
        if (!fastqr::generate(data, output_path, options)) {
            std::cerr << "Error: Failed to generate QR code\n";
            return 1;
        }
    }

    return 0;
}

