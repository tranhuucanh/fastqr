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

export interface Color {
    r: number;
    g: number;
    b: number;
}

export interface QROptions {
    /** Output size in pixels (default: 300, QR codes are square) */
    size?: number;

    /** Auto round-up to nearest integer multiple for best performance (default: false) */
    optimizeSize?: boolean;

    /** @deprecated Use size instead */
    width?: number;

    /** @deprecated Use size instead */
    height?: number;

    /** QR code color as [R, G, B] (default: [0, 0, 0]) */
    foreground?: [number, number, number];

    /** Background color as [R, G, B] (default: [255, 255, 255]) */
    background?: [number, number, number];

    /** Error correction level: 'L', 'M', 'Q', 'H' (default: 'M') */
    errorLevel?: 'L' | 'M' | 'Q' | 'H';

    /** Path to logo image */
    logo?: string;

    /** Logo size as percentage (default: 20) */
    logoSize?: number;

    /** Image quality 1-100 (default: 95) */
    quality?: number;

    /** Output format: 'png', 'jpg', 'webp' (default: 'png') */
    format?: 'png' | 'jpg' | 'jpeg' | 'webp';
}

/**
 * Generate QR code
 * @param data - Data to encode (UTF-8 supported)
 * @param outputPath - Path to save the QR code image
 * @param options - Generation options
 * @returns true if successful
 *
 * @example
 * ```typescript
 * import * as fastqr from 'fastqr';
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
 * ```
 */
export function generate(data: string, outputPath: string, options?: QROptions): boolean;

/**
 * Get library version
 * @returns Version string
 */
export function version(): string;

/** Library version constant */
export const VERSION: string;

