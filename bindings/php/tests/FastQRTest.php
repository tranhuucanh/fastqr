<?php

/**
 * FastQR PHP Tests
 */

namespace FastQR\Tests;

use FastQR\FastQR;
use PHPUnit\Framework\TestCase;

class FastQRTest extends TestCase
{
    private string $outputDir;

    protected function setUp(): void
    {
        $this->outputDir = __DIR__ . '/output';
        if (!is_dir($this->outputDir)) {
            mkdir($this->outputDir, 0755, true);
        }
    }

    public function testVersion(): void
    {
        $version = FastQR::version();
        $this->assertNotEmpty($version);
        $this->assertMatchesRegularExpression('/^\d+\.\d+\.\d+$/', $version);
    }

    public function testBasicGeneration(): void
    {
        $output = $this->outputDir . '/basic.png';
        $result = FastQR::generate('Hello, FastQR from PHP!', $output);

        $this->assertTrue($result);
        $this->assertFileExists($output);
    }

    public function testColoredQR(): void
    {
        $output = $this->outputDir . '/colored.png';
        $result = FastQR::generate('Colored QR', $output, [
            'width' => 500,
            'height' => 500,
            'foreground' => [255, 0, 0],
            'background' => [255, 255, 200]
        ]);

        $this->assertTrue($result);
        $this->assertFileExists($output);
    }

    public function testVietnameseText(): void
    {
        $output = $this->outputDir . '/vietnamese.png';
        $result = FastQR::generate('Xin chào Việt Nam! 🇻🇳', $output);

        $this->assertTrue($result);
        $this->assertFileExists($output);
    }

    public function testJapaneseText(): void
    {
        $output = $this->outputDir . '/japanese.png';
        $result = FastQR::generate('こんにちは日本', $output, [
            'foreground' => [0, 0, 255]
        ]);

        $this->assertTrue($result);
        $this->assertFileExists($output);
    }

    public function testHighErrorCorrection(): void
    {
        $output = $this->outputDir . '/high_ec.png';
        $result = FastQR::generate('High Error Correction', $output, [
            'errorLevel' => 'H'
        ]);

        $this->assertTrue($result);
        $this->assertFileExists($output);
    }

    public function testEmptyDataThrowsException(): void
    {
        $this->expectException(\RuntimeException::class);
        FastQR::generate('', 'output.png');
    }

    public function testEmptyOutputPathThrowsException(): void
    {
        $this->expectException(\RuntimeException::class);
        FastQR::generate('Hello', '');
    }
}

