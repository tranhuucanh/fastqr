/**
 * FastQR - Fast QR Code Generator
 * Node.js Binding
 */

const platform = require('./lib/platform');
const { execFileSync } = require('child_process');
const fs = require('fs');
const path = require('path');

let fastqr;

// Use pre-built CLI binary (no FFI needed!)
if (platform.isPrebuiltAvailable()) {
  const cliPath = path.join(__dirname, 'prebuilt', platform.getPlatformString(), 'bin', 'fastqr');

  if (!fs.existsSync(cliPath)) {
    throw new Error(
      'FastQR CLI binary not found. Expected at: ' + cliPath + '\n' +
      'Please reinstall the package: npm install fastqr-pro'
    );
  }

  // Wrap CLI to match API interface
  fastqr = {
    generate: function(data, outputPath, options = {}) {
      const args = [data, outputPath];

      // Support both new 'size' and legacy 'width'/'height'
      const size = options.size || options.width || options.height || 300;
      args.push('-s', size.toString());

      if (options.optimizeSize) args.push('-o');
      if (options.foreground) args.push('-f', options.foreground.join(','));
      if (options.background) args.push('-b', options.background.join(','));
      if (options.errorLevel) args.push('-e', options.errorLevel);
      if (options.logo) args.push('-l', options.logo);
      if (options.logoSize) args.push('-p', options.logoSize.toString());
      if (options.quality) args.push('-q', options.quality.toString());

      try {
        execFileSync(cliPath, args, { stdio: 'pipe' });
        return true;
      } catch (error) {
        return false;
      }
    },
    version: function() {
      try {
        const output = execFileSync(cliPath, ['-v'], { encoding: 'utf8' });
        return output.trim().replace('FastQR v', '');
      } catch (error) {
        return 'unknown';
      }
    },
    VERSION: null  // Will be set below
  };

  fastqr.VERSION = fastqr.version();
} else {
  throw new Error(
    'FastQR native binding not found. ' +
    'No pre-built binary available for your platform: ' + process.platform + '-' + process.arch
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
        const cliPath = path.join(__dirname, 'prebuilt', platform.getPlatformString(), 'bin', 'fastqr');

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

        execFileSync(cliPath, args, { stdio: 'pipe' });

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

