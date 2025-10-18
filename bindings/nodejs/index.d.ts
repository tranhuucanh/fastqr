/**
 * FastQR TypeScript definitions
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

