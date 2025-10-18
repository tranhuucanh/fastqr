<?php

/**
 * FastQR - Fast QR Code Generator
 * PHP Binding using FFI
 *
 * Copyright (C) 2025 FastQR Project
 * Licensed under LGPL-2.1
 */

namespace FastQR;

use FFI;
use RuntimeException;

class FastQR
{
    private static ?FFI $ffi = null;
    private static bool $initialized = false;

    /**
     * Initialize FFI
     */
    private static function init(): void
    {
        if (self::$initialized) {
            return;
        }

        // Detect platform
        $os = PHP_OS_FAMILY === 'Darwin' ? 'macos' : (PHP_OS_FAMILY === 'Linux' ? 'linux' : 'unknown');
        $arch = php_uname('m');
        if ($arch === 'x86_64' || $arch === 'amd64') {
            $arch = 'x86_64';
        } elseif ($arch === 'aarch64' || $arch === 'arm64') {
            $arch = 'arm64';
        }
        $platform = "$os-$arch";
        $ext = $os === 'macos' ? 'dylib' : 'so';

        // Try to find the library (pre-built first, then system)
        $libPaths = [
            __DIR__ . "/../../prebuilt/$platform/lib/libfastqr.$ext",  // Pre-built binary
            '/usr/local/lib/libfastqr.' . $ext,                         // System install
            '/usr/lib/libfastqr.' . $ext,
            __DIR__ . '/../../../build/libfastqr.' . $ext,               // Local build
        ];

        $libPath = null;
        foreach ($libPaths as $path) {
            if (file_exists($path)) {
                $libPath = $path;
                break;
            }
        }

        if ($libPath === null) {
            throw new RuntimeException(
                "FastQR library not found for platform: $platform\n" .
                "Please install fastqr or build from source."
            );
        }

        // Define C interface
        $header = '
            typedef struct {
                int width;
                int height;
                unsigned char foreground_r;
                unsigned char foreground_g;
                unsigned char foreground_b;
                unsigned char background_r;
                unsigned char background_g;
                unsigned char background_b;
                int ec_level;
                const char* logo_path;
                int logo_size_percent;
                const char* format;
                int quality;
            } QROptions;

            bool fastqr_generate(const char* data, const char* output_path, QROptions* options);
            const char* fastqr_version(void);
        ';

        self::$ffi = FFI::cdef($header, $libPath);
        self::$initialized = true;
    }

    /**
     * Generate QR code
     *
     * @param string $data Data to encode (UTF-8 supported)
     * @param string $outputPath Path to save the QR code image
     * @param array $options Generation options
     * @return bool true if successful
     *
     * @throws RuntimeException if generation fails
     *
     * @example
     * ```php
     * use FastQR\FastQR;
     *
     * // Basic usage
     * FastQR::generate('Hello World', 'qr.png');
     *
     * // With options
     * FastQR::generate('Hello', 'qr.png', [
     *     'width' => 500,
     *     'height' => 500,
     *     'foreground' => [255, 0, 0],
     *     'background' => [255, 255, 200],
     *     'errorLevel' => 'H'
     * ]);
     *
     * // With logo
     * FastQR::generate('Company', 'qr.png', [
     *     'width' => 600,
     *     'height' => 600,
     *     'logo' => 'logo.png',
     *     'logoSize' => 25
     * ]);
     *
     * // UTF-8 support
     * FastQR::generate('Xin chào Việt Nam! 🇻🇳', 'vietnamese.png');
     * FastQR::generate('こんにちは日本', 'japanese.png');
     * ```
     */
    public static function generate(string $data, string $outputPath, array $options = []): bool
    {
        self::init();

        if (empty($data)) {
            throw new RuntimeException('Data cannot be empty');
        }
        if (empty($outputPath)) {
            throw new RuntimeException('Output path cannot be empty');
        }

        // Create options struct
        $opts = self::$ffi->new('QROptions');
        $opts->width = $options['width'] ?? 300;
        $opts->height = $options['height'] ?? 300;

        // Foreground color
        $fg = $options['foreground'] ?? [0, 0, 0];
        $opts->foreground_r = $fg[0] ?? 0;
        $opts->foreground_g = $fg[1] ?? 0;
        $opts->foreground_b = $fg[2] ?? 0;

        // Background color
        $bg = $options['background'] ?? [255, 255, 255];
        $opts->background_r = $bg[0] ?? 255;
        $opts->background_g = $bg[1] ?? 255;
        $opts->background_b = $bg[2] ?? 255;

        // Error correction level
        $ecLevel = $options['errorLevel'] ?? 'M';
        $opts->ec_level = match($ecLevel) {
            'L' => 0,
            'M' => 1,
            'Q' => 2,
            'H' => 3,
            default => 1
        };

        // Logo
        $opts->logo_path = $options['logo'] ?? null;
        $opts->logo_size_percent = $options['logoSize'] ?? 20;

        // Format and quality
        $opts->format = $options['format'] ?? 'png';
        $opts->quality = $options['quality'] ?? 95;

        // Call C function
        $result = self::$ffi->fastqr_generate($data, $outputPath, FFI::addr($opts));

        if (!$result) {
            throw new RuntimeException('Failed to generate QR code');
        }

        return true;
    }

    /**
     * Get library version
     *
     * @return string Version string
     */
    public static function version(): string
    {
        self::init();
        return self::$ffi->fastqr_version();
    }
}

