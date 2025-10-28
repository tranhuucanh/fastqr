<?php

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

