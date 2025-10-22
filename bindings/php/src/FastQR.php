<?php

/**
 * FastQR - Fast QR Code Generator
 * PHP Binding using CLI binary (no FFI needed!)
 *
 * Copyright (C) 2025 FastQR Project
 * Licensed under LGPL-2.1
 */

namespace FastQR;

use RuntimeException;

class FastQR
{
    private static ?string $cliPath = null;

    /**
     * Find CLI binary
     */
    private static function findBinary(): string
    {
        if (self::$cliPath !== null) {
            return self::$cliPath;
        }

        // Detect platform
        $os = PHP_OS_FAMILY === 'Darwin' ? 'macos' : (PHP_OS_FAMILY === 'Linux' ? 'linux' : 'unknown');
        $arch = php_uname('m');
        if ($arch === 'x86_64' || $arch === 'amd64') {
            $arch = 'x86_64';
        } elseif ($arch === 'aarch64' || $arch === 'arm64') {
            $arch = $os === 'macos' ? 'arm64' : 'aarch64';
        }
        $platform = "$os-$arch";

        // Try to find the binary (pre-built first, then system)
        $binaryPaths = [
            __DIR__ . "/../../prebuilt/$platform/bin/fastqr",  // Pre-built binary (AppImage for Linux)
            '/usr/local/bin/fastqr',                            // System install
            '/usr/bin/fastqr',
            __DIR__ . '/../../../build/fastqr',                 // Local build
        ];

        foreach ($binaryPaths as $path) {
            if (file_exists($path) && is_executable($path)) {
                // For Linux, test if AppImage can run
                if ($os === 'linux' && strpos($path, 'prebuilt') !== false) {
                    try {
                        $output = shell_exec(escapeshellarg($path) . ' -v 2>&1');
                        if ($output && strpos($output, 'FastQR') !== false) {
                            self::$cliPath = $path;
                            return $path;
                        }
                    } catch (Exception $e) {
                        // AppImage failed, continue to next path
                        continue;
                    }
                } else {
                    // macOS/Windows or system binary
                    self::$cliPath = $path;
                    return $path;
                }
            }
        }

        throw new RuntimeException(
            "FastQR CLI binary not found for platform: $platform\n" .
            "Searched in:\n" . implode("\n", $binaryPaths) . "\n" .
            "Please install fastqr or build from source."
        );
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
        if (empty($data)) {
            throw new RuntimeException('Data cannot be empty');
        }
        if (empty($outputPath)) {
            throw new RuntimeException('Output path cannot be empty');
        }

        $cliPath = self::findBinary();

        // Build command arguments
        $args = [
            escapeshellarg($cliPath),
            escapeshellarg($data),
            escapeshellarg($outputPath)
        ];

        // Size (preferred) or width/height (backward compatibility)
        if (isset($options['size'])) {
            $args[] = '-s ' . (int)$options['size'];
        } elseif (isset($options['width']) || isset($options['height'])) {
            $size = $options['width'] ?? $options['height'] ?? 300;
            $args[] = '-s ' . (int)$size;
        }

        // Optimize size
        if (!empty($options['optimizeSize'])) {
            $args[] = '-o';
        }

        // Foreground color
        if (isset($options['foreground'])) {
            $fg = $options['foreground'];
            $args[] = '-f ' . implode(',', $fg);
        }

        // Background color
        if (isset($options['background'])) {
            $bg = $options['background'];
            $args[] = '-b ' . implode(',', $bg);
        }

        // Error correction level
        if (isset($options['errorLevel'])) {
            $args[] = '-e ' . escapeshellarg($options['errorLevel']);
        }

        // Logo
        if (isset($options['logo'])) {
            $args[] = '-l ' . escapeshellarg($options['logo']);
        }
        if (isset($options['logoSize'])) {
            $args[] = '-p ' . (int)$options['logoSize'];
        }

        // Quality
        if (isset($options['quality'])) {
            $args[] = '-q ' . (int)$options['quality'];
        }

        // Execute command
        $cmd = implode(' ', $args) . ' 2>&1';
        exec($cmd, $output, $returnCode);

        if ($returnCode !== 0) {
            throw new RuntimeException('Failed to generate QR code: ' . implode("\n", $output));
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
        $cliPath = self::findBinary();
        $cmd = escapeshellarg($cliPath) . ' -v 2>&1';
        $output = shell_exec($cmd);

        if ($output === null) {
            return 'unknown';
        }

        return trim(str_replace('FastQR v', '', $output));
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

            $cliPath = self::findBinary();

            // Build command
            $args = [
                escapeshellarg($cliPath),
                '-F',
                escapeshellarg($tempFile),
                escapeshellarg($outputDir)
            ];

            if (isset($options['size'])) {
                $args[] = '-s ' . (int)$options['size'];
            }
            if (!empty($options['optimizeSize'])) {
                $args[] = '-o';
            }
            if (isset($options['foreground'])) {
                $fg = $options['foreground'];
                $args[] = '-f ' . implode(',', $fg);
            }
            if (isset($options['background'])) {
                $bg = $options['background'];
                $args[] = '-b ' . implode(',', $bg);
            }
            if (isset($options['errorLevel'])) {
                $args[] = '-e ' . escapeshellarg($options['errorLevel']);
            }
            if (isset($options['logo'])) {
                $args[] = '-l ' . escapeshellarg($options['logo']);
            }
            if (isset($options['logoSize'])) {
                $args[] = '-p ' . (int)$options['logoSize'];
            }
            if (isset($options['quality'])) {
                $args[] = '-q ' . (int)$options['quality'];
            }

            $cmd = implode(' ', $args) . ' 2>&1';
            exec($cmd, $output, $returnCode);

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
