# FastQR Node.js Usage Guide

Complete guide for using FastQR in Node.js and JavaScript applications.

## Installation

```bash
npm install fastqr-pro
```

Or with Yarn:

```bash
yarn add fastqr-pro
```

**Note:** No system dependencies required! Pre-built binaries are included. 🎉

## Basic Usage

```javascript
const fastqr = require('fastqr');

// Generate QR code
fastqr.generate('Hello World', 'qr.png');
```

## API Reference

### `fastqr.generate(data, outputPath, options)`

Generate a QR code and save to file.

**Parameters:**
- `data` (string, required) - Data to encode (UTF-8 supported)
- `outputPath` (string, required) - Path to save the QR code image
- `options` (object, optional) - Generation options

**Returns:** `boolean` - `true` if successful

**Throws:** `Error` if generation fails

### Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `size` | number | `300` | Output size in pixels (QR codes are square) |
| `optimizeSize` | boolean | `false` | Auto round-up for best performance |
| `width` | number | - | (Deprecated) Use `size` instead |
| `height` | number | - | (Deprecated) Use `size` instead |
| `foreground` | Array[3] | `[0, 0, 0]` | QR code color (RGB) |
| `background` | Array[3] | `[255, 255, 255]` | Background color (RGB) |
| `errorLevel` | string | `'M'` | Error correction: 'L', 'M', 'Q', 'H' |
| `logo` | string | `undefined` | Path to logo image |
| `logoSize` | number | `20` | Logo size as percentage (1-50) |
| `quality` | number | `95` | Image quality (1-100) |
| `format` | string | `'png'` | Output format: 'png', 'jpg', 'webp' |
| `margin` | number | `0` | Margin (quiet zone) in pixels (absolute) |
| `marginModules` | number | `4` | Margin in modules (relative, ISO standard) |

### `fastqr.generateBatch(dataArray, outputDir, options)`

Generate multiple QR codes at once - **7x faster** than calling `generate` multiple times!

**Parameters:**
- `dataArray` (Array[string], required) - Array of strings to encode
- `outputDir` (string, required) - Directory to save QR codes (created if doesn't exist)
- `options` (object, optional) - Same options as `generate`

**Returns:** `object` - `{ success: number, failed: number }`

**Example:**
```javascript
const data = ['QR 1', 'QR 2', 'QR 3'];
const result = fastqr.generateBatch(data, 'output/', { size: 500, optimizeSize: true });
// Creates: output/1.png, output/2.png, output/3.png
console.log(`Generated ${result.success} QR codes`);
```

### `fastqr.version()`

Get library version.

**Returns:** string (e.g., "1.0.0")

```javascript
console.log(fastqr.version());
// => "1.0.0"
```

### `fastqr.VERSION`

Version constant.

```javascript
console.log(fastqr.VERSION);
// => "1.0.0"
```

## Examples

### 1. Basic QR Code

```javascript
const fastqr = require('fastqr');

fastqr.generate('https://example.com', 'qr.png');
```

### 2. Custom Size

```javascript
fastqr.generate('Large QR', 'large.png', {
  size: 1000
});
```

### 3. Optimized Size (faster generation)

```javascript
fastqr.generate('Fast QR', 'fast.png', {
  size: 500,
  optimizeSize: true
});
```

### 4. Colored QR Code

```javascript
// Red QR on yellow background
fastqr.generate('Colored', 'colored.png', {
  size: 500,
  foreground: [255, 0, 0],      // Red
  background: [255, 255, 200]   // Light yellow
});
```

### 5. QR Code with Logo

```javascript
fastqr.generate('Company', 'company.png', {
  size: 800,
  logo: 'logo.png',
  logoSize: 25,
  errorLevel: 'H'  // High error correction for logo
});
```

### 6. High Error Correction

```javascript
fastqr.generate('Important Data', 'qr.png', {
  errorLevel: 'H'  // ~30% recovery capability
});
```

### 7. UTF-8 Support

```javascript
// Vietnamese
fastqr.generate('Xin chào Việt Nam! 🇻🇳', 'vietnamese.png');

// Japanese
fastqr.generate('こんにちは日本', 'japanese.png');

// Emoji
fastqr.generate('Hello 👋 World 🌍', 'emoji.png');
```

### 8. QR Code with Margin (Quiet Zone)

```javascript
// ISO standard (4 modules) - RECOMMENDED
fastqr.generate('https://example.com', 'qr_iso.png', {
  size: 500,
  marginModules: 4
});

// Absolute pixels
fastqr.generate('https://example.com', 'qr_margin.png', {
  size: 400,
  margin: 20
});

// Margin with custom background color
fastqr.generate('Data', 'qr_margin_bg.png', {
  size: 400,
  marginModules: 4,
  background: [255, 220, 220]
});

// Disable margin (for max performance)
fastqr.generate('Data', 'qr_no_margin.png', {
  size: 500,
  marginModules: 0
});
```

### 9. Batch Generation (7x faster!)

```javascript
// Generate 1000 QR codes
const data = Array.from({ length: 1000 }, (_, i) => `Product ${i + 1}`);

// Old way (slow - ~3 seconds)
// for (let i = 0; i < data.length; i++) {
//   fastqr.generate(data[i], `qr_${i+1}.png`, { size: 500 });
// }

// New way (fast - ~0.4 seconds!)
const result = fastqr.generateBatch(data, 'qr_codes/', {
  size: 500,
  optimizeSize: true
});
console.log(`Generated ${result.success} QR codes`);
// Creates: qr_codes/1.png, qr_codes/2.png, ..., qr_codes/1000.png
```

### 10. Different Formats

```javascript
// PNG (default)
fastqr.generate('Data', 'output.png');

// JPEG
fastqr.generate('Data', 'output.jpg', { quality: 90 });

// WebP
fastqr.generate('Data', 'output.webp', { quality: 85 });
```

## Express.js Integration

### Basic Route

```javascript
const express = require('express');
const fastqr = require('fastqr');
const path = require('path');
const fs = require('fs').promises;

const app = express();
app.use(express.json());

// Generate QR code endpoint
app.post('/api/qr/generate', async (req, res) => {
  try {
    const { data, options = {} } = req.body;

    if (!data) {
      return res.status(400).json({ error: 'Data is required' });
    }

    // Generate unique filename
    const filename = `qr_${Date.now()}_${Math.random().toString(36).substr(2, 9)}.png`;
    const filepath = path.join(__dirname, 'public', 'qrcodes', filename);

    // Ensure directory exists
    await fs.mkdir(path.dirname(filepath), { recursive: true });

    // Generate QR code
    fastqr.generate(data, filepath, {
      size: options.size || 500,
      errorLevel: options.errorLevel || 'M'
    });

    res.json({
      success: true,
      url: `/qrcodes/${filename}`,
      filename
    });
  } catch (error) {
    console.error('QR generation error:', error);
    res.status(500).json({ error: 'Failed to generate QR code' });
  }
});

app.listen(3000, () => {
  console.log('Server running on http://localhost:3000');
});
```

### Advanced Route with Options

```javascript
app.post('/api/qr/advanced', async (req, res) => {
  try {
    const {
      data,
      width = 600,
      height = 600,
      foreground = [0, 0, 0],
      background = [255, 255, 255],
      errorLevel = 'M',
      logo,
      logoSize = 20
    } = req.body;

    if (!data) {
      return res.status(400).json({ error: 'Data is required' });
    }

    const filename = `qr_${Date.now()}.png`;
    const filepath = path.join(__dirname, 'tmp', filename);

    await fs.mkdir(path.dirname(filepath), { recursive: true });

    const options = {
      width,
      height,
      foreground,
      background,
      errorLevel
    };

    if (logo) {
      options.logo = logo;
      options.logoSize = logoSize;
    }

    fastqr.generate(data, filepath, options);

    res.sendFile(filepath);
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});
```

### Middleware Example

```javascript
// QR code generation middleware
function qrGenerator(options = {}) {
  return async (req, res, next) => {
    req.generateQR = async (data, customOptions = {}) => {
      const filename = `qr_${Date.now()}.png`;
      const filepath = path.join(__dirname, 'tmp', filename);

      await fs.mkdir(path.dirname(filepath), { recursive: true });

      const qrOptions = { ...options, ...customOptions };

      fastqr.generate(data, filepath, qrOptions);

      return { filepath, filename };
    };

    next();
  };
}

// Use middleware
app.use(qrGenerator({ size: 500, optimizeSize: true }));

app.post('/qr', async (req, res) => {
  const { data } = req.body;
  const { filepath } = await req.generateQR(data);
  res.sendFile(filepath);
});
```

### Return Base64 Image

```javascript
app.post('/api/qr/base64', async (req, res) => {
  try {
    const { data } = req.body;
    const filename = `qr_${Date.now()}.png`;
    const filepath = path.join(__dirname, 'tmp', filename);

    await fs.mkdir(path.dirname(filepath), { recursive: true });

    // Generate QR code
    fastqr.generate(data, filepath, {
      size: 500
    });

    // Read file and convert to base64
    const imageBuffer = await fs.readFile(filepath);
    const base64Image = imageBuffer.toString('base64');

    // Clean up temp file
    await fs.unlink(filepath);

    res.json({
      success: true,
      image: `data:image/png;base64,${base64Image}`
    });
  } catch (error) {
    res.status(500).json({ error: error.message });
  }
});
```

## TypeScript Usage

### Type Definitions

FastQR includes TypeScript definitions (`index.d.ts`).

```typescript
import * as fastqr from 'fastqr';

// Generate with type checking
fastqr.generate('Hello TypeScript', 'output.png', {
  size: 600,
  foreground: [0, 0, 255],
  errorLevel: 'H'
});

// Options interface
interface QROptions {
  width?: number;
  height?: number;
  foreground?: [number, number, number];
  background?: [number, number, number];
  errorLevel?: 'L' | 'M' | 'Q' | 'H';
  logo?: string;
  logoSize?: number;
  quality?: number;
  format?: 'png' | 'jpg' | 'jpeg' | 'webp';
}

const options: QROptions = {
  size: 800,
  errorLevel: 'H',
  foreground: [255, 0, 0]
};

fastqr.generate('TypeScript QR', 'ts_qr.png', options);
```

### Express with TypeScript

```typescript
import express, { Request, Response } from 'express';
import * as fastqr from 'fastqr';
import path from 'path';
import fs from 'fs/promises';

interface QRRequest {
  data: string;
  options?: fastqr.QROptions;
}

const app = express();
app.use(express.json());

app.post('/api/qr', async (req: Request<{}, {}, QRRequest>, res: Response) => {
  try {
    const { data, options = {} } = req.body;

    const filename = `qr_${Date.now()}.png`;
    const filepath = path.join(__dirname, 'public', filename);

    fastqr.generate(data, filepath, options);

    res.json({ success: true, filename });
  } catch (error) {
    res.status(500).json({ error: 'Generation failed' });
  }
});
```

## Async/Await Patterns

### Promise Wrapper

```javascript
function generateQRAsync(data, outputPath, options) {
  return new Promise((resolve, reject) => {
    try {
      const result = fastqr.generate(data, outputPath, options);
      resolve(result);
    } catch (error) {
      reject(error);
    }
  });
}

// Usage
async function createQR() {
  try {
    await generateQRAsync('Async Data', 'async_qr.png', {
      size: 500
    });
    console.log('✓ QR code generated');
  } catch (error) {
    console.error('✗ Generation failed:', error);
  }
}
```

### Batch Generation

```javascript
async function generateBatch(items) {
  const promises = items.map(async (item, index) => {
    const filename = `qr_${index}.png`;
    const filepath = path.join(__dirname, 'output', filename);

    return generateQRAsync(item.data, filepath, {
      size: item.size || 500
    });
  });

  try {
    await Promise.all(promises);
    console.log(`✓ Generated ${items.length} QR codes`);
  } catch (error) {
    console.error('✗ Batch generation failed:', error);
  }
}

// Old way (slow - loop with individual calls)
// const items = [
//   { data: 'https://example.com/1', size: 500 },
//   { data: 'https://example.com/2', size: 600 },
//   { data: 'https://example.com/3', size: 700 }
// ];

// New way (fast - batch mode)
const dataArray = ['https://example.com/1', 'https://example.com/2', 'https://example.com/3'];
fastqr.generateBatch(dataArray, 'output/', { size: 500, optimizeSize: true });

generateBatch(items);
```

## Advanced Examples

### Dynamic Logo QR

```javascript
const sharp = require('sharp');  // For logo processing

async function generateBrandedQR(data, brandColor, logoPath) {
  const [r, g, b] = brandColor;
  const filename = `branded_${Date.now()}.png`;

  fastqr.generate(data, filename, {
    size: 800,
    foreground: [r, g, b],
    logo: logoPath,
    logoSize: 25,
    errorLevel: 'H'
  });

  return filename;
}

// Usage
const filename = await generateBrandedQR(
  'Company Data',
  [0, 120, 215],  // Microsoft blue
  'company_logo.png'
);
```

### QR Code Service Class

```javascript
class QRCodeService {
  constructor(outputDir = 'public/qrcodes') {
    this.outputDir = outputDir;
    this.ensureDirectory();
  }

  async ensureDirectory() {
    const fs = require('fs').promises;
    await fs.mkdir(this.outputDir, { recursive: true });
  }

  generate(data, options = {}) {
    const filename = `qr_${Date.now()}_${Math.random().toString(36).substr(2, 9)}.png`;
    const filepath = path.join(this.outputDir, filename);

    const defaultOptions = {
      size: 500,
      errorLevel: 'M'
    };

    fastqr.generate(data, filepath, { ...defaultOptions, ...options });

    return { filepath, filename };
  }

  generateWithLogo(data, logoPath, options = {}) {
    return this.generate(data, {
      ...options,
      logo: logoPath,
      logoSize: options.logoSize || 25,
      errorLevel: 'H'
    });
  }

  async cleanup(olderThan = 24 * 60 * 60 * 1000) { // 24 hours
    const fs = require('fs').promises;
    const files = await fs.readdir(this.outputDir);
    const now = Date.now();

    for (const file of files) {
      const filepath = path.join(this.outputDir, file);
      const stats = await fs.stat(filepath);

      if (now - stats.mtimeMs > olderThan) {
        await fs.unlink(filepath);
      }
    }
  }
}

// Usage
const qrService = new QRCodeService();

const { filename } = qrService.generate('Hello World', {
  size: 600
});

const { filename: logoQR } = qrService.generateWithLogo(
  'Company',
  'logo.png',
  { logoSize: 30 }
);

// Cleanup old files daily
setInterval(() => qrService.cleanup(), 24 * 60 * 60 * 1000);
```

## Testing

### Jest Example

```javascript
const fastqr = require('fastqr');
const fs = require('fs');
const path = require('path');

describe('FastQR', () => {
  const outputPath = path.join(__dirname, 'test_qr.png');

  afterEach(() => {
    if (fs.existsSync(outputPath)) {
      fs.unlinkSync(outputPath);
    }
  });

  test('generates basic QR code', () => {
    const result = fastqr.generate('Test Data', outputPath);

    expect(result).toBe(true);
    expect(fs.existsSync(outputPath)).toBe(true);
    expect(fs.statSync(outputPath).size).toBeGreaterThan(0);
  });

  test('generates QR code with options', () => {
    const result = fastqr.generate('Test', outputPath, {
      size: 500,
      errorLevel: 'H'
    });

    expect(result).toBe(true);
    expect(fs.existsSync(outputPath)).toBe(true);
  });

  test('throws error for empty data', () => {
    expect(() => {
      fastqr.generate('', outputPath);
    }).toThrow();
  });

  test('returns version', () => {
    const version = fastqr.version();
    expect(version).toMatch(/^\d+\.\d+\.\d+$/);
  });
});
```

## Performance Tips

- Generate QR codes in worker threads for CPU-intensive operations
- Cache generated QR codes using data hash as filename
- Use lower quality for web display (`quality: 75`)
- Use PNG for best quality, JPG for smaller files

## Troubleshooting

### "Binary not found" Error

The package includes pre-built binaries. If you see this error:

```javascript
const platform = require('./lib/platform');
console.log('Platform:', platform.detectPlatform());
console.log('Binary available:', platform.isPrebuiltAvailable());
console.log('Binary path:', platform.getPrebuiltPath());
```

### File Permission Errors

Ensure output directory has write permissions:

```javascript
const fs = require('fs').promises;
await fs.mkdir(path.dirname(outputPath), { recursive: true });
```

## See Also

- [CLI Usage](CLI_USAGE.md) - Command-line usage
- [Ruby Usage](RUBY_USAGE.md) - Ruby/Rails guide
- [PHP Usage](PHP_USAGE.md) - PHP guide
- [GitHub Repository](https://github.com/tranhuucanh/fastqr)
- [npm Package](https://www.npmjs.com/package/fastqr)

