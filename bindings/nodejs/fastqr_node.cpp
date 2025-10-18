/*
 * FastQR Node.js Binding
 * Copyright (C) 2025 FastQR Project
 */

#include "fastqr.h"
#include <napi.h>

// Convert JS object to QROptions
fastqr::QROptions js_to_options(const Napi::Object& obj) {
    fastqr::QROptions options;

    // Size (preferred) or width/height (backward compatibility)
    if (obj.Has("size")) {
        options.size = obj.Get("size").As<Napi::Number>().Int32Value();
    } else {
        if (obj.Has("width")) {
            options.size = obj.Get("width").As<Napi::Number>().Int32Value();
        }
        if (obj.Has("height")) {
            options.size = obj.Get("height").As<Napi::Number>().Int32Value();
        }
    }

    // Optimize size
    if (obj.Has("optimizeSize")) {
        options.optimize_size = obj.Get("optimizeSize").As<Napi::Boolean>().Value();
    }

    // Foreground color
    if (obj.Has("foreground")) {
        auto fg = obj.Get("foreground").As<Napi::Array>();
        options.foreground.r = fg.Get(uint32_t(0)).As<Napi::Number>().Uint32Value();
        options.foreground.g = fg.Get(uint32_t(1)).As<Napi::Number>().Uint32Value();
        options.foreground.b = fg.Get(uint32_t(2)).As<Napi::Number>().Uint32Value();
    }

    // Background color
    if (obj.Has("background")) {
        auto bg = obj.Get("background").As<Napi::Array>();
        options.background.r = bg.Get(uint32_t(0)).As<Napi::Number>().Uint32Value();
        options.background.g = bg.Get(uint32_t(1)).As<Napi::Number>().Uint32Value();
        options.background.b = bg.Get(uint32_t(2)).As<Napi::Number>().Uint32Value();
    }

    // Error correction level
    if (obj.Has("errorLevel")) {
        std::string level = obj.Get("errorLevel").As<Napi::String>().Utf8Value();
        if (level == "L") options.ec_level = fastqr::ErrorCorrectionLevel::LOW;
        else if (level == "M") options.ec_level = fastqr::ErrorCorrectionLevel::MEDIUM;
        else if (level == "Q") options.ec_level = fastqr::ErrorCorrectionLevel::QUARTILE;
        else if (level == "H") options.ec_level = fastqr::ErrorCorrectionLevel::HIGH;
    }

    // Logo
    if (obj.Has("logo")) {
        options.logo_path = obj.Get("logo").As<Napi::String>().Utf8Value();
    }
    if (obj.Has("logoSize")) {
        options.logo_size_percent = obj.Get("logoSize").As<Napi::Number>().Int32Value();
    }

    // Quality and format
    if (obj.Has("quality")) {
        options.quality = obj.Get("quality").As<Napi::Number>().Int32Value();
    }
    if (obj.Has("format")) {
        options.format = obj.Get("format").As<Napi::String>().Utf8Value();
    }

    return options;
}

// generate(data, outputPath, options)
Napi::Value Generate(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 2) {
        Napi::TypeError::New(env, "Expected at least 2 arguments")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "First argument must be a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsString()) {
        Napi::TypeError::New(env, "Second argument must be a string")
            .ThrowAsJavaScriptException();
        return env.Null();
    }

    std::string data = info[0].As<Napi::String>().Utf8Value();
    std::string output_path = info[1].As<Napi::String>().Utf8Value();

    fastqr::QROptions options;
    if (info.Length() >= 3 && info[2].IsObject()) {
        options = js_to_options(info[2].As<Napi::Object>());
    }

    bool result = fastqr::generate(data, output_path, options);

    return Napi::Boolean::New(env, result);
}

// version()
Napi::Value Version(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    return Napi::String::New(env, fastqr::version());
}

// Initialize module
Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set("generate", Napi::Function::New(env, Generate));
    exports.Set("version", Napi::Function::New(env, Version));
    exports.Set("VERSION", Napi::String::New(env, fastqr::version()));

    return exports;
}

NODE_API_MODULE(fastqr, Init)

