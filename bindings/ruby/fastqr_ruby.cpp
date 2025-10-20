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
#include <ruby.h>

static VALUE rb_mFastQR;
static VALUE rb_cQROptions;

// Convert Ruby hash to QROptions
static fastqr::QROptions hash_to_options(VALUE opts) {
    fastqr::QROptions options;

    if (NIL_P(opts)) {
        return options;
    }

    VALUE val;

    // Size (preferred) or width/height (backward compatibility)
    val = rb_hash_aref(opts, ID2SYM(rb_intern("size")));
    if (!NIL_P(val)) {
        options.size = NUM2INT(val);
    } else {
        // Backward compatibility: if width or height specified, use them
        val = rb_hash_aref(opts, ID2SYM(rb_intern("width")));
        if (!NIL_P(val)) options.size = NUM2INT(val);

        val = rb_hash_aref(opts, ID2SYM(rb_intern("height")));
        if (!NIL_P(val)) options.size = NUM2INT(val);
    }

    // Optimize size
    val = rb_hash_aref(opts, ID2SYM(rb_intern("optimize_size")));
    if (!NIL_P(val)) options.optimize_size = RTEST(val);

    // Foreground color
    val = rb_hash_aref(opts, ID2SYM(rb_intern("foreground")));
    if (!NIL_P(val) && TYPE(val) == T_ARRAY) {
        options.foreground.r = NUM2INT(rb_ary_entry(val, 0));
        options.foreground.g = NUM2INT(rb_ary_entry(val, 1));
        options.foreground.b = NUM2INT(rb_ary_entry(val, 2));
    }

    // Background color
    val = rb_hash_aref(opts, ID2SYM(rb_intern("background")));
    if (!NIL_P(val) && TYPE(val) == T_ARRAY) {
        options.background.r = NUM2INT(rb_ary_entry(val, 0));
        options.background.g = NUM2INT(rb_ary_entry(val, 1));
        options.background.b = NUM2INT(rb_ary_entry(val, 2));
    }

    // Error correction level
    val = rb_hash_aref(opts, ID2SYM(rb_intern("error_level")));
    if (!NIL_P(val)) {
        const char* level = StringValueCStr(val);
        if (strcmp(level, "L") == 0) options.ec_level = fastqr::ErrorCorrectionLevel::LOW;
        else if (strcmp(level, "M") == 0) options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM;
        else if (strcmp(level, "Q") == 0) options.ec_level = fastqr::ErrorCorrectionLevel::QUARTILE;
        else if (strcmp(level, "H") == 0) options.ec_level = fastqr::ErrorCorrectionLevel::HIGH;
    }

    // Logo path
    val = rb_hash_aref(opts, ID2SYM(rb_intern("logo")));
    if (!NIL_P(val)) {
        options.logo_path = StringValueCStr(val);
    }

    // Logo size
    val = rb_hash_aref(opts, ID2SYM(rb_intern("logo_size")));
    if (!NIL_P(val)) {
        options.logo_size_percent = NUM2INT(val);
    }

    // Quality
    val = rb_hash_aref(opts, ID2SYM(rb_intern("quality")));
    if (!NIL_P(val)) {
        options.quality = NUM2INT(val);
    }

    // Format
    val = rb_hash_aref(opts, ID2SYM(rb_intern("format")));
    if (!NIL_P(val)) {
        options.format = StringValueCStr(val);
    }

    return options;
}

// FastQR.generate(data, output_path, options = {})
static VALUE rb_fastqr_generate(int argc, VALUE *argv, VALUE self) {
    VALUE data, output_path, opts;
    rb_scan_args(argc, argv, "21", &data, &output_path, &opts);

    std::string data_str = StringValueCStr(data);
    std::string output_str = StringValueCStr(output_path);
    fastqr::QROptions options = hash_to_options(opts);

    bool result = fastqr::generate(data_str, output_str, options);

    return result ? Qtrue : Qfalse;
}

// FastQR.version
static VALUE rb_fastqr_version(VALUE self) {
    return rb_str_new_cstr(fastqr::version());
}

extern "C" void Init_fastqr() {
    rb_mFastQR = rb_define_module("FastQR");

    rb_define_module_function(rb_mFastQR, "generate",
                              RUBY_METHOD_FUNC(rb_fastqr_generate), -1);
    rb_define_module_function(rb_mFastQR, "version",
                              RUBY_METHOD_FUNC(rb_fastqr_version), 0);

    // Constants
    rb_define_const(rb_mFastQR, "VERSION", rb_str_new_cstr(fastqr::version()));
}

