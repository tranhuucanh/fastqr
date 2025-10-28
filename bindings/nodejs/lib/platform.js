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

const os = require('os');
const path = require('path');
const fs = require('fs');
const { execSync } = require('child_process');

/**
 * Detects the current platform and architecture
 * @returns {string} Platform identifier (e.g., 'macos-arm64', 'linux-x86_64')
 */
function detectPlatform() {
  const platform = os.platform();
  const arch = os.arch();

  if (platform === 'darwin') {
    if (arch === 'arm64') return 'macos-arm64';
    if (arch === 'x64') return 'macos-x86_64';
  } else if (platform === 'linux') {
    if (arch === 'x64') return 'linux-x86_64';
    if (arch === 'arm64') return 'linux-aarch64';
  }

  throw new Error(`Unsupported platform: ${platform}-${arch}`);
}

/**
 * Extracts pre-built binary from tarball
 * @param {string} tarballPath - Path to the tarball
 * @param {string} destDir - Destination directory
 */
function extractBinary(tarballPath, destDir) {
  if (!fs.existsSync(destDir)) {
    fs.mkdirSync(destDir, { recursive: true });
  }

  execSync(`tar -xzf "${tarballPath}" -C "${destDir}"`, { stdio: 'inherit' });
}

/**
 * Finds the fastqr binary in prebuilt directory
 * @returns {string} Path to fastqr binary
 */
function findFastQRBinary() {
  const platform = detectPlatform();
  const prebuiltDir = path.join(__dirname, '..', 'prebuilt', platform);
  const binaryPath = path.join(prebuiltDir, 'bin', 'fastqr');

  if (platform.startsWith('linux')) {
    // Linux: Check for AppImage first
    if (fs.existsSync(binaryPath) && fs.accessSync(binaryPath, fs.constants.X_OK)) {
      try {
        // Test if AppImage can run (version check)
        const output = execSync(`${binaryPath} -v`, { encoding: 'utf8', stdio: 'pipe' });
        if (output.includes('FastQR')) {
          return binaryPath;
        }
      } catch (error) {
        // AppImage failed, continue to fallback
      }
    }
  } else {
    // macOS/Windows: Use regular binary
    if (fs.existsSync(binaryPath)) {
      fs.chmodSync(binaryPath, 0o755);
      return binaryPath;
    }
  }

  // Try to extract from tarball
  const tarballPath = path.join(__dirname, '..', 'prebuilt', `${platform}.tar.gz`);
  if (fs.existsSync(tarballPath)) {
    console.log(`Extracting pre-built binary from ${tarballPath}...`);
    extractBinary(tarballPath, prebuiltDir);

    if (fs.existsSync(binaryPath)) {
      fs.chmodSync(binaryPath, 0o755);
      return binaryPath;
    }
  }

  throw new Error(`Pre-built binary not found for ${platform}`);
}

/**
 * Checks if pre-built binary is available
 * @returns {boolean}
 */
function isPrebuiltAvailable() {
  try {
    const platform = detectPlatform();
    const prebuiltDir = path.join(__dirname, '..', 'prebuilt', platform, 'bin');
    const binaryPath = path.join(prebuiltDir, 'fastqr');
    return fs.existsSync(binaryPath);
  } catch (error) {
    return false;
  }
}

/**
 * Gets platform string
 * @returns {string}
 */
function getPlatformString() {
  return detectPlatform();
}

module.exports = {
  detectPlatform,
  extractBinary,
  findFastQRBinary,
  isPrebuiltAvailable,
  getPlatformString
};
