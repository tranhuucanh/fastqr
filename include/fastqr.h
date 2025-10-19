/*
 * FastQR - Fast QR Code Generator Library
 * Copyright (C) 2025 FastQR Project
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef FASTQR_H
#define FASTQR_H

#include <string>
#include <cstdint>

namespace fastqr {

/**
 * QR Code error correction level
 */
enum class ErrorCorrectionLevel {
    LOW,        // Level L - ~7% correction
    MEDIUM,     // Level M - ~15% correction
    QUARTILE,   // Level Q - ~25% correction
    HIGH        // Level H - ~30% correction
};

/**
 * Options for QR code generation
 */
struct QROptions {
    // Output image size (QR codes are square)
    int size = 300;

    // Optimize size - round up to nearest integer multiple for best performance
    bool optimize_size = false;

    // QR code colors (RGB)
    struct Color {
        uint8_t r = 0;
        uint8_t g = 0;
        uint8_t b = 0;
    };

    Color foreground = {0, 0, 0};      // QR code color (default: black)
    Color background = {255, 255, 255}; // Background color (default: white)

    // Error correction level
    ErrorCorrectionLevel ec_level = ErrorCorrectionLevel::MEDIUM;

    // Logo options
    std::string logo_path = "";         // Path to logo image
    int logo_size_percent = 20;         // Logo size as percentage of QR code (default: 20%)

    // Output format
    std::string format = "png";         // png, jpg, webp, etc.
    int quality = 95;                   // For lossy formats (1-100)
};

/**
 * Generate QR code and save to file
 *
 * @param data The data to encode (supports UTF-8)
 * @param output_path Path to save the generated QR code image
 * @param options QR code generation options
 * @return true if successful, false otherwise
 */
bool generate(const std::string& data, const std::string& output_path, const QROptions& options = QROptions());

/**
 * Generate QR code and return image data as buffer
 *
 * @param data The data to encode (supports UTF-8)
 * @param buffer Output buffer for image data
 * @param buffer_size Size of the output buffer
 * @param options QR code generation options
 * @return Size of image data written to buffer, or -1 on error
 */
int generate_to_buffer(const std::string& data, void* buffer, size_t buffer_size, const QROptions& options = QROptions());

/**
 * Get library version
 *
 * @return Version string (e.g., "1.0.0")
 */
const char* version();

} // namespace fastqr

// C API for FFI bindings (Ruby, Node.js, Python, etc.)
#ifdef __cplusplus
extern "C" {
#endif

/**
 * C struct for QR options (FFI-friendly)
 */
typedef struct {
    int size;
    int optimize_size;  // boolean: 0 or 1
    unsigned char foreground_r;
    unsigned char foreground_g;
    unsigned char foreground_b;
    unsigned char background_r;
    unsigned char background_g;
    unsigned char background_b;
    int ec_level;  // 0=LOW, 1=MEDIUM, 2=QUARTILE, 3=HIGH
    const char* logo_path;
    int logo_size_percent;
    const char* format;
    int quality;
} QROptions;

/**
 * Generate QR code (C API)
 *
 * @param data Data to encode (UTF-8 string)
 * @param output_path Path to save the QR code image
 * @param options Pointer to QROptions struct (can be NULL for defaults)
 * @return 1 if successful, 0 on error
 */
int fastqr_generate(const char* data, const char* output_path, const QROptions* options);

/**
 * Get library version (C API)
 *
 * @return Version string (e.g., "1.0.7")
 */
const char* fastqr_version(void);

#ifdef __cplusplus
}
#endif

#endif // FASTQR_H

