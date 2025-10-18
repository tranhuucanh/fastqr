/**
 * Platform detection for FastQR
 */

const os = require('os');
const path = require('path');
const fs = require('fs');

/**
 * Detect current platform
 */
function detectPlatform() {
  const platform = os.platform();
  const arch = os.arch();

  let osName;
  switch (platform) {
    case 'darwin':
      osName = 'macos';
      break;
    case 'linux':
      osName = 'linux';
      break;
    case 'win32':
      osName = 'windows';
      break;
    default:
      osName = platform;
  }

  let archName;
  switch (arch) {
    case 'x64':
      archName = 'x86_64';
      break;
    case 'arm64':
      archName = 'arm64';
      break;
    default:
      archName = arch;
  }

  return `${osName}-${archName}`;
}

/**
 * Get library extension for current platform
 */
function getLibExtension() {
  const platform = os.platform();
  switch (platform) {
    case 'darwin':
      return 'dylib';
    case 'linux':
      return 'so';
    case 'win32':
      return 'dll';
    default:
      return 'so';
  }
}

/**
 * Get path to pre-built binary
 */
function getPrebuiltPath() {
  const platform = detectPlatform();
  const ext = getLibExtension();
  return path.join(__dirname, '..', 'prebuilt', platform, 'lib', `libfastqr.${ext}`);
}

/**
 * Check if pre-built binary is available
 */
function isPrebuiltAvailable() {
  const prebuiltPath = getPrebuiltPath();
  return fs.existsSync(prebuiltPath);
}

/**
 * Get path to compiled addon
 */
function getAddonPath() {
  return path.join(__dirname, '..', 'build', 'Release', 'fastqr.node');
}

/**
 * Check if compiled addon is available
 */
function isAddonAvailable() {
  return fs.existsSync(getAddonPath());
}

module.exports = {
  detectPlatform,
  getLibExtension,
  getPrebuiltPath,
  isPrebuiltAvailable,
  getAddonPath,
  isAddonAvailable
};

