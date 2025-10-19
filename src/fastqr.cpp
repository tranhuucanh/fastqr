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
#include <qrencode.h>
#include <png.h>
#include <zlib.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <cstring>
#include <memory>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <chrono>

// Enable benchmarking
// #define FASTQR_BENCHMARK

#define FASTQR_VERSION "1.0.21"

namespace fastqr {

// RAII wrapper for QRcode
struct QRCodeDeleter {
    void operator()(QRcode* qr) const {
        if (qr) QRcode_free(qr);
    }
};
using QRCodePtr = std::unique_ptr<QRcode, QRCodeDeleter>;

// Detect text encoding and return appropriate encoding mode
static QRecLevel to_qr_level(ErrorCorrectionLevel level) {
    switch (level) {
        case ErrorCorrectionLevel::LOW: return QR_ECLEVEL_L;
        case ErrorCorrectionLevel::MEDIUM: return QR_ECLEVEL_M;
        case ErrorCorrectionLevel::QUARTILE: return QR_ECLEVEL_Q;
        case ErrorCorrectionLevel::HIGH: return QR_ECLEVEL_H;
        default: return QR_ECLEVEL_M;
    }
}

// Detect if string contains only ASCII
static bool is_ascii(const std::string& str) {
    for (unsigned char c : str) {
        if (c > 127) return false;
    }
    return true;
}

// Generate QR code using libqrencode with proper UTF-8 support
static QRCodePtr generate_qr_code(const std::string& data, ErrorCorrectionLevel ec_level) {
    QRecLevel level = to_qr_level(ec_level);

    // Use appropriate encoding based on content
    QRcode* qr = nullptr;
    if (is_ascii(data)) {
        // ASCII data - use standard encoding
        qr = QRcode_encodeString(data.c_str(), 0, level, QR_MODE_8, 1);
    } else {
        // UTF-8 data - encode as 8-bit data
        qr = QRcode_encodeData(data.length(),
                               reinterpret_cast<const unsigned char*>(data.c_str()),
                               0, level);
    }

    if (!qr) {
        std::cerr << "Failed to generate QR code" << std::endl;
        return nullptr;
    }

    return QRCodePtr(qr);
}

// Write indexed PNG (1-bit, black and white) - fastest method
static bool write_indexed_png(const char* filename, const std::vector<unsigned char>& data,
                               int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return false;

    // Large buffer for faster I/O
    setvbuf(fp, nullptr, _IOFBF, 65536);

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return false;
    }

    png_init_io(png, fp);

    // Set PNG parameters for 1-bit indexed (palette)
    png_set_IHDR(png, info, width, height, 1, PNG_COLOR_TYPE_PALETTE,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // Create palette: 0=white, 1=black
    png_color palette[2];
    palette[0].red = palette[0].green = palette[0].blue = 255; // white
    palette[1].red = palette[1].green = palette[1].blue = 0;   // black
    png_set_PLTE(png, info, palette, 2);

    // Level 1 compression - good balance between speed and size
    png_set_compression_level(png, 1);
    png_set_filter(png, 0, PNG_FILTER_NONE);

    png_write_info(png, info);

    // Data is already packed (8 pixels per byte), no packing needed

    // Write image data row by row
    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = const_cast<png_bytep>(&data[y * ((width + 7) / 8)]);
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return true;
}

// Write grayscale PNG (8-bit)
static bool write_grayscale_png(const char* filename, const std::vector<unsigned char>& data,
                                int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return false;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return false;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_GRAY,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_compression_level(png, 1);
    png_set_filter(png, 0, PNG_FILTER_NONE);

    png_write_info(png, info);

    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = const_cast<png_bytep>(&data[y * width]);
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return true;
}

// Write RGB PNG
static bool write_rgb_png(const char* filename, const std::vector<unsigned char>& data,
                          int width, int height) {
    FILE* fp = fopen(filename, "wb");
    if (!fp) return false;

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png) {
        fclose(fp);
        return false;
    }

    png_infop info = png_create_info_struct(png);
    if (!info) {
        png_destroy_write_struct(&png, nullptr);
        fclose(fp);
        return false;
    }

    if (setjmp(png_jmpbuf(png))) {
        png_destroy_write_struct(&png, &info);
        fclose(fp);
        return false;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    png_set_compression_level(png, 1);
    png_set_filter(png, 0, PNG_FILTER_NONE);

    png_write_info(png, info);

    std::vector<png_bytep> row_pointers(height);
    for (int y = 0; y < height; y++) {
        row_pointers[y] = const_cast<png_bytep>(&data[y * width * 3]);
    }

    png_write_image(png, row_pointers.data());
    png_write_end(png, nullptr);

    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return true;
}

// Simple nearest-neighbor resize for logo
static void resize_logo(const std::vector<unsigned char>& src, int src_w, int src_h, int channels,
                        std::vector<unsigned char>& dst, int dst_w, int dst_h) {
    dst.resize(dst_w * dst_h * channels);
    double x_ratio = static_cast<double>(src_w) / dst_w;
    double y_ratio = static_cast<double>(src_h) / dst_h;

    for (int y = 0; y < dst_h; y++) {
        int src_y = static_cast<int>(y * y_ratio);
        for (int x = 0; x < dst_w; x++) {
            int src_x = static_cast<int>(x * x_ratio);
            for (int c = 0; c < channels; c++) {
                dst[(y * dst_w + x) * channels + c] = src[(src_y * src_w + src_x) * channels + c];
            }
        }
    }
}

// Add logo to QR code (works with grayscale or RGB)
static void add_logo_to_image(std::vector<unsigned char>& qr_img, int qr_size, int qr_channels,
                               const std::string& logo_path, int logo_size_percent) {
    // Load logo
    int logo_w, logo_h, logo_channels;
    unsigned char* logo_data = stbi_load(logo_path.c_str(), &logo_w, &logo_h, &logo_channels, 0);

    if (!logo_data) {
        std::cerr << "Warning: Failed to load logo: " << logo_path << std::endl;
        return;
    }

    // Calculate logo size
    int logo_target_size = (qr_size * logo_size_percent) / 100;
    int logo_new_w, logo_new_h;

    // Keep aspect ratio
    if (logo_w > logo_h) {
        logo_new_w = logo_target_size;
        logo_new_h = (logo_h * logo_target_size) / logo_w;
    } else {
        logo_new_h = logo_target_size;
        logo_new_w = (logo_w * logo_target_size) / logo_h;
    }

    // Resize logo
    std::vector<unsigned char> logo_src(logo_data, logo_data + logo_w * logo_h * logo_channels);
    stbi_image_free(logo_data);

    std::vector<unsigned char> logo_resized;
    resize_logo(logo_src, logo_w, logo_h, logo_channels, logo_resized, logo_new_w, logo_new_h);

    // Calculate position (center)
    int start_x = (qr_size - logo_new_w) / 2;
    int start_y = (qr_size - logo_new_h) / 2;

    // Composite logo onto QR
    for (int y = 0; y < logo_new_h; y++) {
        for (int x = 0; x < logo_new_w; x++) {
            int qr_x = start_x + x;
            int qr_y = start_y + y;

            if (qr_x >= 0 && qr_x < qr_size && qr_y >= 0 && qr_y < qr_size) {
                int logo_idx = (y * logo_new_w + x) * logo_channels;
                int qr_idx = (qr_y * qr_size + qr_x) * qr_channels;

                if (logo_channels == 4) {
                    // Has alpha channel
                    float alpha = logo_resized[logo_idx + 3] / 255.0f;

                    if (qr_channels == 1) {
                        // Grayscale QR
                        unsigned char logo_gray = (logo_resized[logo_idx] +
                                                   logo_resized[logo_idx + 1] +
                                                   logo_resized[logo_idx + 2]) / 3;
                        qr_img[qr_idx] = static_cast<unsigned char>(
                            logo_gray * alpha + qr_img[qr_idx] * (1 - alpha)
                        );
                    } else {
                        // RGB QR
                        for (int c = 0; c < 3; c++) {
                            qr_img[qr_idx + c] = static_cast<unsigned char>(
                                logo_resized[logo_idx + c] * alpha + qr_img[qr_idx + c] * (1 - alpha)
                            );
                        }
                    }
                } else {
                    // No alpha, direct copy
                    if (qr_channels == 1 && logo_channels >= 3) {
                        // Convert RGB logo to grayscale
                        qr_img[qr_idx] = (logo_resized[logo_idx] +
                                         logo_resized[logo_idx + 1] +
                                         logo_resized[logo_idx + 2]) / 3;
                    } else if (qr_channels == 3 && logo_channels == 1) {
                        // Grayscale logo to RGB
                        qr_img[qr_idx] = qr_img[qr_idx + 1] = qr_img[qr_idx + 2] = logo_resized[logo_idx];
                    } else {
                        // Same channels
                        for (int c = 0; c < std::min(qr_channels, logo_channels); c++) {
                            qr_img[qr_idx + c] = logo_resized[logo_idx + c];
                        }
                    }
                }
            }
        }
    }
}

bool generate(const std::string& data, const std::string& output_path, const QROptions& options) {
#ifdef FASTQR_BENCHMARK
    auto t_start = std::chrono::high_resolution_clock::now();
#endif

    // Generate QR code
    auto qr = generate_qr_code(data, options.ec_level);
    if (!qr) {
        return false;
    }

#ifdef FASTQR_BENCHMARK
    auto t_qr_gen = std::chrono::high_resolution_clock::now();
    auto dur_qr = std::chrono::duration_cast<std::chrono::microseconds>(t_qr_gen - t_start).count();
#endif

    int qr_size = qr->width;
    unsigned char* qr_data = qr->data;

    // Determine final output size
    int final_size = options.size;
    if (options.optimize_size) {
        // Round up to nearest integer multiple for best performance
        int scale = (options.size + qr_size - 1) / qr_size; // Ceiling division
        final_size = scale * qr_size;
    }

    // Check if using default black/white colors
    bool is_bw = (options.foreground.r == 0 && options.foreground.g == 0 && options.foreground.b == 0 &&
                  options.background.r == 255 && options.background.g == 255 && options.background.b == 255);

    // Check if we can use indexed PNG (only if no logo)
    bool can_use_indexed = is_bw && options.logo_path.empty();

    if (can_use_indexed) {
        // FASTEST PATH: 1-bit indexed PNG (like qrencode)
        // Pack 8 pixels into 1 byte
        int scale = final_size / qr_size;

#ifdef FASTQR_BENCHMARK
        FILE* debug_log = fopen("/tmp/fastqr_debug.log", "a");
        if (debug_log) {
            fprintf(debug_log, "DEBUG: is_bw=%d, qr_size=%d, final_size=%d, scale=%d, scale*qr=%d\n",
                   is_bw, qr_size, final_size, scale, scale*qr_size);
            fclose(debug_log);
        }
#endif

        if (scale * qr_size == final_size) {
            // Integer scaling - optimized bit packing
            int bytes_per_row = (final_size + 7) / 8;
            std::vector<unsigned char> packed_data(bytes_per_row * final_size, 0);

            // Build one template row then replicate vertically
            std::vector<unsigned char> template_row(bytes_per_row);

            for (int src_y = 0; src_y < qr_size; src_y++) {
                // Build template row for this QR row
                std::memset(template_row.data(), 0, bytes_per_row);

                for (int src_x = 0; src_x < qr_size; src_x++) {
                    if (qr_data[src_y * qr_size + src_x] & 1) {
                        // Black pixel - set all bits for scaled pixels
                        int dst_x_start = src_x * scale;
                        int dst_x_end = dst_x_start + scale;

                        for (int dst_x = dst_x_start; dst_x < dst_x_end; dst_x++) {
                            int byte_idx = dst_x >> 3;
                            int bit_idx = 7 - (dst_x & 7);
                            template_row[byte_idx] |= (1 << bit_idx);
                        }
                    }
                }

                // Copy template row to all scaled rows
                int dst_y_start = src_y * scale;
                for (int dy = 0; dy < scale; dy++) {
                    std::memcpy(&packed_data[(dst_y_start + dy) * bytes_per_row],
                               template_row.data(), bytes_per_row);
                }
            }

#ifdef FASTQR_BENCHMARK
            auto t_scale = std::chrono::high_resolution_clock::now();
            auto dur_scale = std::chrono::duration_cast<std::chrono::microseconds>(t_scale - t_qr_gen).count();
#endif

            bool result = write_indexed_png(output_path.c_str(), packed_data, final_size, final_size);

#ifdef FASTQR_BENCHMARK
            auto t_write = std::chrono::high_resolution_clock::now();
            auto dur_write = std::chrono::duration_cast<std::chrono::microseconds>(t_write - t_scale).count();
            auto dur_total = std::chrono::duration_cast<std::chrono::microseconds>(t_write - t_start).count();

            FILE* timing_log = fopen("/tmp/fastqr_timing.log", "a");
            if (timing_log) {
                fprintf(timing_log, "TIMING: QR=%ldus, Scale=%ldus, Write=%ldus, Total=%ldus\n",
                       dur_qr, dur_scale, dur_write, dur_total);
                fclose(timing_log);
            }
#endif

            return result;
        } else {
            // Non-integer scaling - use grayscale
            std::vector<unsigned char> final_image(final_size * final_size);
            double x_ratio = static_cast<double>(qr_size) / final_size;
            double y_ratio = static_cast<double>(qr_size) / final_size;

            for (int y = 0; y < final_size; y++) {
                int src_y = static_cast<int>(y * y_ratio);
                int src_row_idx = src_y * qr_size;
                for (int x = 0; x < final_size; x++) {
                    int src_x = static_cast<int>(x * x_ratio);
                    final_image[y * final_size + x] = (qr_data[src_row_idx + src_x] & 1) ? 0 : 255;
                }
            }

            // Add logo if specified
            if (!options.logo_path.empty()) {
                add_logo_to_image(final_image, final_size, 1, options.logo_path, options.logo_size_percent);
            }

            return write_grayscale_png(output_path.c_str(), final_image, final_size, final_size);
        }
    } else if (is_bw) {
        // Black/white but with logo - use RGB to preserve logo colors
        std::vector<unsigned char> final_image(final_size * final_size * 3);
        int scale = final_size / qr_size;

        if (scale * qr_size == final_size) {
            // Integer scaling
            std::vector<unsigned char> scaled_row(final_size * 3);

            for (int src_y = 0; src_y < qr_size; src_y++) {
                for (int src_x = 0; src_x < qr_size; src_x++) {
                    unsigned char val = (qr_data[src_y * qr_size + src_x] & 1) ? 0 : 255;
                    int dst_x_start = src_x * scale;
                    for (int dx = 0; dx < scale; dx++) {
                        int idx = (dst_x_start + dx) * 3;
                        scaled_row[idx] = scaled_row[idx + 1] = scaled_row[idx + 2] = val;
                    }
                }

                int dst_y_start = src_y * scale;
                unsigned char* dst_ptr = &final_image[dst_y_start * final_size * 3];
                std::memcpy(dst_ptr, scaled_row.data(), final_size * 3);
                for (int dy = 1; dy < scale; dy++) {
                    std::memcpy(dst_ptr + dy * final_size * 3, dst_ptr, final_size * 3);
                }
            }
        } else {
            // Non-integer scaling
            double x_ratio = static_cast<double>(qr_size) / final_size;
            double y_ratio = static_cast<double>(qr_size) / final_size;

            for (int y = 0; y < final_size; y++) {
                int src_y = static_cast<int>(y * y_ratio);
                int src_row_idx = src_y * qr_size;
                for (int x = 0; x < final_size; x++) {
                    int src_x = static_cast<int>(x * x_ratio);
                    unsigned char val = (qr_data[src_row_idx + src_x] & 1) ? 0 : 255;
                    int idx = (y * final_size + x) * 3;
                    final_image[idx] = final_image[idx + 1] = final_image[idx + 2] = val;
                }
            }
        }

        // Add logo (RGB)
        add_logo_to_image(final_image, final_size, 3, options.logo_path, options.logo_size_percent);

        return write_rgb_png(output_path.c_str(), final_image, final_size, final_size);
    } else {
        // COLOR PATH: RGB output for custom colors
        bool is_grayscale = (options.foreground.r == options.foreground.g &&
                            options.foreground.g == options.foreground.b &&
                            options.background.r == options.background.g &&
                            options.background.g == options.background.b);

        if (is_grayscale) {
            // Use grayscale PNG
            std::vector<unsigned char> final_image(final_size * final_size);
            unsigned char fg = options.foreground.r;
            unsigned char bg = options.background.r;

            int scale = final_size / qr_size;
            if (scale * qr_size == final_size) {
                // Integer scaling
                std::vector<unsigned char> scaled_row(final_size);

                for (int src_y = 0; src_y < qr_size; src_y++) {
                    for (int src_x = 0; src_x < qr_size; src_x++) {
                        unsigned char val = (qr_data[src_y * qr_size + src_x] & 1) ? fg : bg;
                        int dst_x_start = src_x * scale;
                        std::memset(&scaled_row[dst_x_start], val, scale);
                    }

                    int dst_y_start = src_y * scale;
                    unsigned char* dst_ptr = &final_image[dst_y_start * final_size];
                    std::memcpy(dst_ptr, scaled_row.data(), final_size);
                    for (int dy = 1; dy < scale; dy++) {
                        std::memcpy(dst_ptr + dy * final_size, dst_ptr, final_size);
                    }
                }
            } else {
                // Non-integer scaling
                double x_ratio = static_cast<double>(qr_size) / final_size;
                double y_ratio = static_cast<double>(qr_size) / final_size;

                for (int y = 0; y < final_size; y++) {
                    int src_y = static_cast<int>(y * y_ratio);
                    int src_row_idx = src_y * qr_size;
                    for (int x = 0; x < final_size; x++) {
                        int src_x = static_cast<int>(x * x_ratio);
                        final_image[y * final_size + x] = (qr_data[src_row_idx + src_x] & 1) ? fg : bg;
                    }
                }
            }

            // Add logo if specified
            if (!options.logo_path.empty()) {
                add_logo_to_image(final_image, final_size, 1, options.logo_path, options.logo_size_percent);
            }

            return write_grayscale_png(output_path.c_str(), final_image, final_size, final_size);
        } else {
            // Full RGB for non-grayscale colors
            std::vector<unsigned char> final_image(final_size * final_size * 3);

            int scale = final_size / qr_size;
            if (scale * qr_size == final_size) {
                // Integer scaling
                std::vector<unsigned char> scaled_row(final_size * 3);

                for (int src_y = 0; src_y < qr_size; src_y++) {
                    for (int src_x = 0; src_x < qr_size; src_x++) {
                        bool is_black = qr_data[src_y * qr_size + src_x] & 1;
                        unsigned char r = is_black ? options.foreground.r : options.background.r;
                        unsigned char g = is_black ? options.foreground.g : options.background.g;
                        unsigned char b = is_black ? options.foreground.b : options.background.b;

                        int dst_x_start = src_x * scale;
                        for (int dx = 0; dx < scale; dx++) {
                            int idx = (dst_x_start + dx) * 3;
                            scaled_row[idx] = r;
                            scaled_row[idx + 1] = g;
                            scaled_row[idx + 2] = b;
                        }
                    }

                    int dst_y_start = src_y * scale;
                    unsigned char* dst_ptr = &final_image[dst_y_start * final_size * 3];
                    std::memcpy(dst_ptr, scaled_row.data(), final_size * 3);
                    for (int dy = 1; dy < scale; dy++) {
                        std::memcpy(dst_ptr + dy * final_size * 3, dst_ptr, final_size * 3);
                    }
                }
            } else {
                // Non-integer scaling
                double x_ratio = static_cast<double>(qr_size) / final_size;
                double y_ratio = static_cast<double>(qr_size) / final_size;

                for (int y = 0; y < final_size; y++) {
                    int src_y = static_cast<int>(y * y_ratio);
                    int src_row_idx = src_y * qr_size;
                    for (int x = 0; x < final_size; x++) {
                        int src_x = static_cast<int>(x * x_ratio);
                        bool is_black = qr_data[src_row_idx + src_x] & 1;
                        int idx = (y * final_size + x) * 3;
                        final_image[idx] = is_black ? options.foreground.r : options.background.r;
                        final_image[idx + 1] = is_black ? options.foreground.g : options.background.g;
                        final_image[idx + 2] = is_black ? options.foreground.b : options.background.b;
                    }
                }
            }

            // Add logo if specified
            if (!options.logo_path.empty()) {
                add_logo_to_image(final_image, final_size, 3, options.logo_path, options.logo_size_percent);
            }

            return write_rgb_png(output_path.c_str(), final_image, final_size, final_size);
        }
    }
}

int generate_to_buffer(const std::string& data, void* buffer, size_t buffer_size, const QROptions& options) {
    // For buffer generation, temporarily write to a temp file then read it back
    // This is a simplified implementation - could be optimized to write directly to memory
    std::string temp_file = "/tmp/fastqr_temp.png";

    if (!generate(data, temp_file, options)) {
        return -1;
    }

    FILE* fp = fopen(temp_file.c_str(), "rb");
    if (!fp) return -1;

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    if (static_cast<size_t>(file_size) > buffer_size) {
        fclose(fp);
        std::remove(temp_file.c_str());
        return -1;
    }

    size_t read_size = fread(buffer, 1, file_size, fp);
    fclose(fp);
    std::remove(temp_file.c_str());

    return static_cast<int>(read_size);
}

const char* version() {
    return FASTQR_VERSION;
}

} // namespace fastqr

// ============================================================================
// C API Implementation for FFI bindings
// ============================================================================

extern "C" {

int fastqr_generate(const char* data, const char* output_path, const QROptions* c_options) {
    if (!data || !output_path) {
        return 0;
    }

    fastqr::QROptions options;

    if (c_options) {
        options.size = c_options->size;
        options.optimize_size = (c_options->optimize_size != 0);
        options.foreground.r = c_options->foreground_r;
        options.foreground.g = c_options->foreground_g;
        options.foreground.b = c_options->foreground_b;
        options.background.r = c_options->background_r;
        options.background.g = c_options->background_g;
        options.background.b = c_options->background_b;

        switch (c_options->ec_level) {
            case 0: options.ec_level = fastqr::ErrorCorrectionLevel::LOW; break;
            case 1: options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM; break;
            case 2: options.ec_level = fastqr::ErrorCorrectionLevel::QUARTILE; break;
            case 3: options.ec_level = fastqr::ErrorCorrectionLevel::HIGH; break;
            default: options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM; break;
        }

        if (c_options->logo_path) {
            options.logo_path = c_options->logo_path;
        }
        options.logo_size_percent = c_options->logo_size_percent;

        if (c_options->format) {
            options.format = c_options->format;
        }
        options.quality = c_options->quality;
    }

    bool result = fastqr::generate(data, output_path, options);
    return result ? 1 : 0;
}

const char* fastqr_version(void) {
    return fastqr::version();
}

} // extern "C"

