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
                int size;
                int optimize_size;
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
     *     'size' => 500,
     *     'optimizeSize' => true,
     *     'foreground' => [255, 0, 0],
     *     'background' => [255, 255, 200],
     *     'errorLevel' => 'H'
     * ]);
     *
     * // With logo
     * FastQR::generate('Company', 'qr.png', [
     *     'size' => 600,
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

        // Size (preferred) or width/height (backward compatibility)
        if (isset($options['size'])) {
            $opts->size = $options['size'];
        } elseif (isset($options['width']) || isset($options['height'])) {
            $opts->size = $options['width'] ?? $options['height'] ?? 300;
        } else {
            $opts->size = 300;
        }

        // Optimize size
        $opts->optimize_size = $options['optimizeSize'] ?? false ? 1 : 0;

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

    /**
     * Generate multiple QR codes in batch mode (7x faster!)
     *
     * @param array $dataArray Array of strings to encode
     * @param string $outputDir Directory to save QR codes (will be created if it doesn't exist)
     * @param array $options Generation options (same as generate)
     * @return array Result with 'success' and 'failed' counts
     *
     * @throws RuntimeException if generation fails
     *
     * @example
     * ```php
     * use FastQR\FastQR;
     *
     * // Batch generation
     * $data = ['QR 1', 'QR 2', 'QR 3'];
     * FastQR::generateBatch($data, 'output_dir/', ['size' => 500]);
     * // Creates: output_dir/1.png, output_dir/2.png, output_dir/3.png
     * ```
     */
    public static function generateBatch(array $dataArray, string $outputDir, array $options = []): array
    {
        if (empty($dataArray)) {
            throw new RuntimeException('Data array cannot be empty');
        }
        if (empty($outputDir)) {
            throw new RuntimeException('Output directory cannot be empty');
        }

        // Create output directory
        if (!is_dir($outputDir)) {
            mkdir($outputDir, 0755, true);
        }

        // Create temporary batch file
        $tempFile = tempnam(sys_get_temp_dir(), 'fastqr_batch_') . '.txt';
        try {
            file_put_contents($tempFile, implode("\n", $dataArray));

            // Find CLI binary
            $os = PHP_OS_FAMILY === 'Darwin' ? 'macos' : (PHP_OS_FAMILY === 'Linux' ? 'linux' : 'unknown');
            $arch = php_uname('m');
            if ($arch === 'x86_64' || $arch === 'amd64') {
                $arch = 'x86_64';
            } elseif ($arch === 'aarch64' || $arch === 'arm64') {
                $arch = 'arm64';
            }
            $platform = "$os-$arch";

            $cliPaths = [
                __DIR__ . "/../../prebuilt/$platform/bin/fastqr",
                '/usr/local/bin/fastqr',
                __DIR__ . '/../../../build/fastqr',
            ];

            $cliPath = null;
            foreach ($cliPaths as $path) {
                if (file_exists($path)) {
                    $cliPath = $path;
                    break;
                }
            }

            if ($cliPath === null) {
                throw new RuntimeException("FastQR CLI not found for platform: $platform");
            }

            // Build command
            $cmd = escapeshellarg($cliPath) . ' -F ' . escapeshellarg($tempFile) . ' ' . escapeshellarg($outputDir);

            if (isset($options['size'])) {
                $cmd .= ' -s ' . (int)$options['size'];
            }
            if (!empty($options['optimizeSize'])) {
                $cmd .= ' -o';
            }
            if (isset($options['foreground'])) {
                $fg = $options['foreground'];
                $cmd .= ' -f ' . implode(',', $fg);
            }
            if (isset($options['background'])) {
                $bg = $options['background'];
                $cmd .= ' -b ' . implode(',', $bg);
            }
            if (isset($options['errorLevel'])) {
                $cmd .= ' -e ' . escapeshellarg($options['errorLevel']);
            }
            if (isset($options['logo'])) {
                $cmd .= ' -l ' . escapeshellarg($options['logo']);
            }
            if (isset($options['logoSize'])) {
                $cmd .= ' -p ' . (int)$options['logoSize'];
            }
            if (isset($options['quality'])) {
                $cmd .= ' -q ' . (int)$options['quality'];
            }

            exec($cmd . ' 2>&1', $output, $returnCode);

            if ($returnCode !== 0) {
                throw new RuntimeException('Batch generation failed: ' . implode("\n", $output));
            }

            return ['success' => count($dataArray), 'failed' => 0];
        } finally {
            if (file_exists($tempFile)) {
                unlink($tempFile);
            }
        }
    }
}

