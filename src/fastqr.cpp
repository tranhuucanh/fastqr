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
#include <vips/vips.h>
#include <cstring>
#include <memory>
#include <iostream>
#include <vector>

#define FASTQR_VERSION "1.0.0"

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

// Create VipsImage from QR code with specified colors
static VipsImage* qr_to_vips_image(const QRcode* qr, const QROptions& options) {
    if (!qr || qr->width <= 0) {
        return nullptr;
    }

    int qr_size = qr->width;
    unsigned char* data = qr->data;

    // Create RGB image buffer
    std::vector<unsigned char> rgb_data(qr_size * qr_size * 3);

    for (int y = 0; y < qr_size; y++) {
        for (int x = 0; x < qr_size; x++) {
            int idx = y * qr_size + x;
            int rgb_idx = idx * 3;

            // Check if this pixel is part of QR code (bit 0 set means black module)
            bool is_black = data[idx] & 1;

            if (is_black) {
                rgb_data[rgb_idx] = options.foreground.r;
                rgb_data[rgb_idx + 1] = options.foreground.g;
                rgb_data[rgb_idx + 2] = options.foreground.b;
            } else {
                rgb_data[rgb_idx] = options.background.r;
                rgb_data[rgb_idx + 1] = options.background.g;
                rgb_data[rgb_idx + 2] = options.background.b;
            }
        }
    }

    // Create VipsImage from buffer
    VipsImage* image = vips_image_new_from_memory(
        rgb_data.data(),
        rgb_data.size(),
        qr_size,
        qr_size,
        3,
        VIPS_FORMAT_UCHAR
    );

    if (!image) {
        std::cerr << "Failed to create VipsImage" << std::endl;
        return nullptr;
    }

    // Make a copy since we're using stack memory
    VipsImage* copy = vips_image_copy_memory(image);
    g_object_unref(image);

    return copy;
}

// Resize image to exact dimensions
static VipsImage* resize_image(VipsImage* in, int target_width, int target_height) {
    VipsImage* out = nullptr;

    double h_scale = static_cast<double>(target_width) / in->Xsize;
    double v_scale = static_cast<double>(target_height) / in->Ysize;

    if (vips_resize(in, &out, h_scale, "vscale", v_scale, "kernel", VIPS_KERNEL_NEAREST, NULL)) {
        std::cerr << "Failed to resize image: " << vips_error_buffer() << std::endl;
        return nullptr;
    }

    return out;
}

// Add logo to center of QR code
static VipsImage* add_logo(VipsImage* qr_image, const std::string& logo_path, int logo_size_percent) {
    VipsImage* logo = nullptr;

    // Load logo
    if (vips_thumbnail(logo_path.c_str(), &logo, 100, NULL)) {
        std::cerr << "Failed to load logo: " << vips_error_buffer() << std::endl;
        return qr_image;
    }

    // Calculate logo size
    int qr_width = qr_image->Xsize;
    int qr_height = qr_image->Ysize;
    int logo_size = (qr_width * logo_size_percent) / 100;

    // Resize logo
    double scale = static_cast<double>(logo_size) / std::max(logo->Xsize, logo->Ysize);
    VipsImage* logo_resized = nullptr;
    if (vips_resize(logo, &logo_resized, scale, NULL)) {
        g_object_unref(logo);
        return qr_image;
    }
    g_object_unref(logo);

    // Convert logo to RGB if needed
    VipsImage* logo_rgb = nullptr;
    if (vips_colourspace(logo_resized, &logo_rgb, VIPS_INTERPRETATION_sRGB, NULL)) {
        g_object_unref(logo_resized);
        return qr_image;
    }
    g_object_unref(logo_resized);

    // Calculate position (center)
    int x = (qr_width - logo_rgb->Xsize) / 2;
    int y = (qr_height - logo_rgb->Ysize) / 2;

    // Composite logo onto QR code
    VipsImage* out = nullptr;
    if (vips_insert(qr_image, logo_rgb, &out, x, y, NULL)) {
        std::cerr << "Failed to insert logo: " << vips_error_buffer() << std::endl;
        g_object_unref(logo_rgb);
        return qr_image;
    }
    g_object_unref(logo_rgb);

    return out;
}

bool generate(const std::string& data, const std::string& output_path, const QROptions& options) {
    // Initialize libvips (can be called multiple times safely)
    if (VIPS_INIT("fastqr")) {
        std::cerr << "Failed to initialize libvips" << std::endl;
        return false;
    }

    // Generate QR code
    auto qr = generate_qr_code(data, options.ec_level);
    if (!qr) {
        return false;
    }

    // Convert to VipsImage with colors
    VipsImage* image = qr_to_vips_image(qr.get(), options);
    if (!image) {
        return false;
    }

    // Resize to target dimensions
    VipsImage* resized = resize_image(image, options.width, options.height);
    g_object_unref(image);
    if (!resized) {
        return false;
    }

    // Add logo if specified
    VipsImage* final_image = resized;
    if (!options.logo_path.empty()) {
        VipsImage* with_logo = add_logo(resized, options.logo_path, options.logo_size_percent);
        if (with_logo != resized) {
            g_object_unref(resized);
            final_image = with_logo;
        }
    }

    // Save to file
    int result = vips_image_write_to_file(final_image, output_path.c_str(),
                                          "Q", options.quality, NULL);
    g_object_unref(final_image);

    if (result) {
        std::cerr << "Failed to save image: " << vips_error_buffer() << std::endl;
        return false;
    }

    return true;
}

int generate_to_buffer(const std::string& data, void* buffer, size_t buffer_size, const QROptions& options) {
    // Initialize libvips
    if (VIPS_INIT("fastqr")) {
        return -1;
    }

    // Generate QR code
    auto qr = generate_qr_code(data, options.ec_level);
    if (!qr) {
        return -1;
    }

    // Convert to VipsImage
    VipsImage* image = qr_to_vips_image(qr.get(), options);
    if (!image) {
        return -1;
    }

    // Resize
    VipsImage* resized = resize_image(image, options.width, options.height);
    g_object_unref(image);
    if (!resized) {
        return -1;
    }

    // Add logo if specified
    VipsImage* final_image = resized;
    if (!options.logo_path.empty()) {
        VipsImage* with_logo = add_logo(resized, options.logo_path, options.logo_size_percent);
        if (with_logo != resized) {
            g_object_unref(resized);
            final_image = with_logo;
        }
    }

    // Write to buffer
    void* out_buffer = nullptr;
    size_t out_size = 0;

    std::string format_str = "." + options.format;
    if (vips_image_write_to_buffer(final_image, format_str.c_str(),
                                   &out_buffer, &out_size,
                                   "Q", options.quality, NULL)) {
        g_object_unref(final_image);
        return -1;
    }
    g_object_unref(final_image);

    // Copy to user buffer
    if (out_size > buffer_size) {
        g_free(out_buffer);
        return -1;
    }

    std::memcpy(buffer, out_buffer, out_size);
    g_free(out_buffer);

    return static_cast<int>(out_size);
}

const char* version() {
    return FASTQR_VERSION;
}

} // namespace fastqr

