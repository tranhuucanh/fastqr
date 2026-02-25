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
#include <cstring>

extern "C" {

// C-compatible options structure
struct QROptions_C {
    int size;
    int optimize_size;
    unsigned char foreground_r;
    unsigned char foreground_g;
    unsigned char foreground_b;
    unsigned char background_r;
    unsigned char background_g;
    unsigned char background_b;
    int ec_level;  // 0=L, 1=M, 2=Q, 3=H
    const char* logo_path;
    int logo_size_percent;
    const char* format;
    int quality;
};

// Convert C struct to C++ options
fastqr::QROptions c_to_cpp_options(const QROptions_C* c_opts) {
    fastqr::QROptions opts;

    if (c_opts) {
        opts.size = c_opts->size;
        opts.optimize_size = c_opts->optimize_size != 0;

        opts.foreground.r = c_opts->foreground_r;
        opts.foreground.g = c_opts->foreground_g;
        opts.foreground.b = c_opts->foreground_b;

        opts.background.r = c_opts->background_r;
        opts.background.g = c_opts->background_g;
        opts.background.b = c_opts->background_b;

        switch (c_opts->ec_level) {
            case 0: opts.ec_level = fastqr::ErrorCorrectionLevel::LOW; break;
            case 1: opts.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM; break;
            case 2: opts.ec_level = fastqr::ErrorCorrectionLevel::QUARTILE; break;
            case 3: opts.ec_level = fastqr::ErrorCorrectionLevel::HIGH; break;
            default: opts.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM; break;
        }

        if (c_opts->logo_path) {
            opts.logo_path = c_opts->logo_path;
        }
        opts.logo_size_percent = c_opts->logo_size_percent;

        if (c_opts->format) {
            opts.format = c_opts->format;
        }
        opts.quality = c_opts->quality;
        opts.margin = c_opts->margin;
        opts.margin_modules = c_opts->margin_modules;
    }

    return opts;
}

// C-compatible generate function
bool fastqr_generate(const char* data, const char* output_path, QROptions_C* options) {
    if (!data || !output_path) {
        return false;
    }

    std::string data_str(data);
    std::string output_str(output_path);
    fastqr::QROptions opts = c_to_cpp_options(options);

    return fastqr::generate(data_str, output_str, opts);
}

// C-compatible version function
const char* fastqr_version(void) {
    return fastqr::version();
}

} // extern "C"

