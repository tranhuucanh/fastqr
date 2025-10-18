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

  const lib = ffi.Library(libPath, {
    'fastqr_generate': ['bool', ['string', 'string', 'pointer']],
    'fastqr_version': ['string', []]
  });

  // Wrap FFI functions to match Node addon interface
  fastqr = {
    generate: function(data, outputPath, options = {}) {
      // TODO: Convert options to C struct
      // For now, use default options (NULL pointer)
      return lib.fastqr_generate(data, outputPath, ref.NULL);
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
 * @property {number} [width=300] - Output width in pixels
 * @property {number} [height=300] - Output height in pixels
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
 *   width: 500,
 *   height: 500,
 *   foreground: [255, 0, 0],
 *   background: [255, 255, 200],
 *   errorLevel: 'H'
 * });
 *
 * // With logo
 * fastqr.generate('Company', 'qr.png', {
 *   width: 600,
 *   height: 600,
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

module.exports = {
    generate,
    version,
    VERSION: fastqr.VERSION
};

