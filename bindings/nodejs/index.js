/**
 * FastQR - Fast QR Code Generator
 * Node.js Binding
 */

const platform = require('./lib/platform');
const ffi = require('ffi-napi');
const ref = require('ref-napi');
const path = require('path');

let fastqr;

// Try to load pre-built binary first, fall back to compiled addon
if (platform.isPrebuiltAvailable()) {
  // Load via FFI
  const libPath = platform.getPrebuiltPath();

  // Define C struct for options
  const QROptionsStruct = ref.types.void; // Use void* for simplicity, C function handles NULL

  const lib = ffi.Library(libPath, {
    'fastqr_generate': ['int', ['string', 'string', 'pointer']],
    'fastqr_version': ['string', []]
  });

  // Wrap FFI functions to match Node addon interface
  fastqr = {
    generate: function(data, outputPath, options = {}) {
      // For now, pass NULL - C function uses defaults
      // TODO: Build C struct for full options support
      const result = lib.fastqr_generate(data, outputPath, ref.NULL);
      return result === 1; // C returns 1 for success, 0 for failure
    },
    version: function() {
      return lib.fastqr_version();
    },
    VERSION: lib.fastqr_version()
  };
} else if (platform.isAddonAvailable()) {
  // Load compiled addon
  fastqr = require(platform.getAddonPath());
} else {
  throw new Error(
    'FastQR native binding not found. ' +
    'Please run: npm install --build-from-source'
  );
}

/**
 * QR code generation options
 * @typedef {Object} QROptions
 * @property {number} [size=300] - Output size in pixels (QR codes are square)
 * @property {boolean} [optimizeSize=false] - Auto round-up to nearest integer multiple for best performance
 * @property {number} [width=300] - @deprecated Use size instead
 * @property {number} [height=300] - @deprecated Use size instead
 * @property {number[]} [foreground=[0,0,0]] - QR code color as [R, G, B]
 * @property {number[]} [background=[255,255,255]] - Background color as [R, G, B]
 * @property {string} [errorLevel='M'] - Error correction level: 'L', 'M', 'Q', 'H'
 * @property {string} [logo] - Path to logo image
 * @property {number} [logoSize=20] - Logo size as percentage
 * @property {number} [quality=95] - Image quality (1-100)
 * @property {string} [format='png'] - Output format: 'png', 'jpg', 'webp'
 */

/**
 * Generate QR code
 * @param {string} data - Data to encode (UTF-8 supported)
 * @param {string} outputPath - Path to save the QR code image
 * @param {QROptions} [options={}] - Generation options
 * @returns {boolean} true if successful
 *
 * @example
 * const fastqr = require('fastqr');
 *
 * // Basic usage
 * fastqr.generate('Hello World', 'qr.png');
 *
 * // With options
 * fastqr.generate('Hello', 'qr.png', {
 *   size: 500,
 *   optimizeSize: true,
 *   foreground: [255, 0, 0],
 *   background: [255, 255, 200],
 *   errorLevel: 'H'
 * });
 *
 * // With logo
 * fastqr.generate('Company', 'qr.png', {
 *   size: 600,
 *   logo: 'logo.png',
 *   logoSize: 25
 * });
 *
 * // UTF-8 support
 * fastqr.generate('Xin chào Việt Nam! 🇻🇳', 'vietnamese.png');
 * fastqr.generate('こんにちは日本', 'japanese.png');
 */
function generate(data, outputPath, options = {}) {
    if (!data || typeof data !== 'string') {
        throw new TypeError('Data must be a non-empty string');
    }
    if (!outputPath || typeof outputPath !== 'string') {
        throw new TypeError('Output path must be a non-empty string');
    }

    const result = fastqr.generate(data, outputPath, options);
    if (!result) {
        throw new Error('Failed to generate QR code');
    }
    return result;
}

/**
 * Get library version
 * @returns {string} Version string
 */
function version() {
    return fastqr.version();
}

/**
 * Generate multiple QR codes in batch mode (7x faster!)
 * @param {string[]} dataArray - Array of strings to encode
 * @param {string} outputDir - Directory to save QR codes (will be created if it doesn't exist)
 * @param {QROptions} [options={}] - Generation options (same as generate)
 * @returns {Object} Result with success and failed counts
 *
 * @example
 * const fastqr = require('fastqr');
 *
 * // Batch generation
 * const data = ['QR 1', 'QR 2', 'QR 3'];
 * fastqr.generateBatch(data, 'output_dir/', { size: 500 });
 * // Creates: output_dir/1.png, output_dir/2.png, output_dir/3.png
 */
function generateBatch(dataArray, outputDir, options = {}) {
    if (!Array.isArray(dataArray) || dataArray.length === 0) {
        throw new TypeError('Data array must be a non-empty array');
    }
    if (!outputDir || typeof outputDir !== 'string') {
        throw new TypeError('Output directory must be a non-empty string');
    }

    const fs = require('fs');
    const { execFileSync } = require('child_process');
    const os = require('os');
    const path = require('path');

    // Create output directory
    if (!fs.existsSync(outputDir)) {
        fs.mkdirSync(outputDir, { recursive: true });
    }

    // Create temporary batch file
    const tempFile = path.join(os.tmpdir(), `fastqr_batch_${Date.now()}.txt`);
    try {
        fs.writeFileSync(tempFile, dataArray.join('\n'), 'utf8');

        // Get CLI path
        const cliPath = platform.getPrebuiltPath().replace('.dylib', '').replace('.so', '');
        const actualCliPath = cliPath.endsWith('fastqr') ? cliPath : path.join(path.dirname(cliPath), 'fastqr');

        // Build command arguments
        const args = ['-F', tempFile, outputDir];
        if (options.size) args.push('-s', options.size.toString());
        if (options.optimizeSize) args.push('-o');
        if (options.foreground) args.push('-f', options.foreground.join(','));
        if (options.background) args.push('-b', options.background.join(','));
        if (options.errorLevel) args.push('-e', options.errorLevel);
        if (options.logo) args.push('-l', options.logo);
        if (options.logoSize) args.push('-p', options.logoSize.toString());
        if (options.quality) args.push('-q', options.quality.toString());

        execFileSync(actualCliPath, args, { stdio: 'pipe' });

        return { success: dataArray.length, failed: 0 };
    } catch (error) {
        throw new Error(`Batch generation failed: ${error.message}`);
    } finally {
        if (fs.existsSync(tempFile)) {
            fs.unlinkSync(tempFile);
        }
    }
}

module.exports = {
    generate,
    generateBatch,
    version,
    VERSION: fastqr.VERSION
};

