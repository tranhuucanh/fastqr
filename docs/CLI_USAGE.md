# FastQR CLI Usage Guide

Complete guide for using the FastQR command-line tool.

## Installation

### macOS (Homebrew)
```bash
brew tap tranhuucanh/fastqr
brew install fastqr
```

### Ubuntu/Debian
```bash
wget https://github.com/tranhuucanh/fastqr/releases/download/v1.0.0/fastqr-1.0.0-linux-x64.deb
sudo dpkg -i fastqr-1.0.0-linux-x64.deb
```

### From Source
```bash
git clone https://github.com/tranhuucanh/fastqr.git
cd fastqr
mkdir build && cd build
cmake ..
make
sudo make install
```

## Basic Usage

```bash
fastqr <data> <output_file>
```

**Example:**
```bash
fastqr "Hello World" qr.png
```

## Command-Line Options

### Help and Version

```bash
# Show help
fastqr --help
fastqr -h

# Show version
fastqr --version
fastqr -v
```

### Size (`-s`, `--size`)

Specify output size in pixels. QR codes are always square.

**Format:** `SIZE`

```bash
# 500x500 pixels
fastqr -s 500 "Large QR" large.png

# 1000x1000 pixels
fastqr -s 1000 "Very Large" xl.png

# 2000x2000 pixels (exact size!)
fastqr -s 2000 "Huge QR" huge.png
```

**Default:** `300` (300x300 pixels)

### Optimize (`-o`, `--optimize`)

Auto round-up size to nearest integer multiple of QR code module size for best performance.

```bash
# Optimized size (fastest generation)
fastqr -s 500 -o "Fast QR" fast.png

# Best for batch processing
fastqr -F batch.txt output_dir/ -s 500 -o
```

**Note:** With `-o`, the actual output size may be slightly larger than requested (e.g., 500 → 493 or 506) to match QR code module alignment, resulting in sharper pixels and faster generation.

### Foreground Color (`-f`, `--foreground`)

Set QR code color using RGB values (0-255).

**Format:** `R,G,B`

```bash
# Red QR code
fastqr -f 255,0,0 "Red" red.png

# Blue QR code
fastqr -f 0,0,255 "Blue" blue.png

# Green QR code
fastqr -f 0,255,0 "Green" green.png

# Custom color (orange)
fastqr -f 255,165,0 "Orange" orange.png

# Dark gray
fastqr -f 50,50,50 "Dark Gray" gray.png
```

**Default:** `0,0,0` (black)

### Background Color (`-b`, `--background`)

Set background color using RGB values (0-255).

**Format:** `R,G,B`

```bash
# White background (default)
fastqr -b 255,255,255 "White BG" white_bg.png

# Light yellow background
fastqr -b 255,255,200 "Yellow BG" yellow_bg.png

# Light blue background
fastqr -b 200,220,255 "Blue BG" blue_bg.png

# Transparent-looking (very light gray)
fastqr -b 245,245,245 "Light Gray BG" light_bg.png
```

**Default:** `255,255,255` (white)

### Combine Colors

```bash
# Red QR on yellow background
fastqr -f 255,0,0 -b 255,255,200 "Red on Yellow" red_yellow.png

# Blue QR on light blue background
fastqr -f 0,0,200 -b 200,220,255 "Blue Theme" blue_theme.png

# White QR on black background (inverted)
fastqr -f 255,255,255 -b 0,0,0 "Inverted" inverted.png
```

### Error Correction Level (`-e`, `--error-level`)

Set error correction capability. Higher levels allow QR code to be readable even when partially damaged or obscured.

**Values:** `L`, `M`, `Q`, `H`

```bash
# Low (~7% recovery)
fastqr -e L "Low EC" low_ec.png

# Medium (~15% recovery) - DEFAULT
fastqr -e M "Medium EC" medium_ec.png

# Quartile (~25% recovery)
fastqr -e Q "Quartile EC" quartile_ec.png

# High (~30% recovery) - Best for logos
fastqr -e H "High EC" high_ec.png
```

**Default:** `M` (Medium)

**When to use:**
- `L`: Simple QR codes, maximum data capacity
- `M`: General purpose (default)
- `Q`: QR codes with small logos
- `H`: QR codes with large logos or harsh environments

### Logo (`-l`, `--logo`)

Embed a logo image in the center of the QR code.

```bash
# Add logo with default size (20%)
fastqr -l logo.png "Company" with_logo.png

# Logo with specific size (25%)
fastqr -l logo.png -p 25 "Company" logo_25.png

# Logo with high error correction (recommended)
fastqr -e H -l logo.png "Company" logo_high_ec.png

# Full example with logo
fastqr -s 800 \
       -e H \
       -l company_logo.png \
       -p 30 \
       "https://company.com" \
       company_qr.png
```

### Logo Size (`-p`, `--logo-size`)

Specify logo size as percentage of QR code size.

**Range:** `1-50` (percentage)

```bash
# Small logo (15%)
fastqr -l logo.png -p 15 "Small Logo" small_logo.png

# Medium logo (25%)
fastqr -l logo.png -p 25 "Medium Logo" medium_logo.png

# Large logo (35%)
fastqr -l logo.png -p 35 "Large Logo" large_logo.png
```

**Default:** `20` (20%)

**Recommendation:** Use 20-30% for best results. Higher percentages require higher error correction (`-e H`).

### Quality (`-q`, `--quality`)

Set image quality for lossy formats (JPG, WebP).

**Range:** `1-100`

```bash
# Maximum quality
fastqr -q 100 "Max Quality" max.png

# High quality (default)
fastqr -q 95 "High Quality" high.png

# Medium quality (smaller file)
fastqr -q 75 "Medium Quality" medium.png

# Low quality (very small file)
fastqr -q 50 "Low Quality" low.png

# For JPG
fastqr -q 90 "JPG Quality" output.jpg
```

**Default:** `95`

**Note:** For PNG (lossless), quality setting has minimal effect.

### Batch Mode (`-F`, `--file`)

Process multiple QR codes at once - **7x faster** than calling fastqr multiple times!

```bash
# Create batch input file (one QR text per line)
cat > batch.txt << EOF
QR Code 1
QR Code 2
QR Code 3
EOF

# Generate batch
fastqr -F batch.txt output_dir/

# With options (applied to all QR codes)
fastqr -F batch.txt output_dir/ -s 500 -o

# With colors and logo
fastqr -F batch.txt output_dir/ \
       -s 600 \
       -f 255,0,0 \
       -e H \
       -l logo.png
```

**Output:** Creates numbered files: `output_dir/1.png`, `output_dir/2.png`, `output_dir/3.png`, ...

**Performance:**
- 100 QR codes: ~0.05s (vs ~0.3s with 100 calls)
- 1000 QR codes: ~0.4s (vs ~3s with 1000 calls)

## Output Formats

FastQR automatically detects format from file extension:

```bash
# PNG (default, lossless)
fastqr "Data" output.png

# JPEG (lossy, smaller file)
fastqr -q 90 "Data" output.jpg

# WebP (modern, smaller file)
fastqr -q 85 "Data" output.webp
```

## UTF-8 Support

FastQR fully supports UTF-8 characters including emojis!

```bash
# Vietnamese
fastqr "Xin chào Việt Nam! 🇻🇳" vietnamese.png

# Japanese (Hiragana)
fastqr "こんにちは" japanese_hiragana.png

# Japanese (Kanji)
fastqr "日本語" japanese_kanji.png

# Chinese
fastqr "你好世界" chinese.png

# Emojis
fastqr "Hello 👋 World 🌍" emoji.png

# Mixed
fastqr "Hello world - こんにちは - 你好 - Xin chào" mixed.png
```

## Complete Examples

### 1. Simple QR Code
```bash
fastqr "https://example.com" simple.png
```

### 2. Large QR Code
```bash
fastqr -s 1000 "https://example.com" large.png
```

### 3. Colored QR Code
```bash
fastqr -s 500 \
       -f 0,100,200 \
       -b 240,248,255 \
       "Colored QR Code" \
       colored.png
```

### 4. QR Code with Logo
```bash
fastqr -s 800 \
       -e H \
       -l company_logo.png \
       -p 25 \
       "https://company.com" \
       company_qr.png
```

### 5. Business Card QR
```bash
fastqr -s 600 \
       -f 0,0,0 \
       -b 255,255,255 \
       -e H \
       "BEGIN:VCARD
VERSION:3.0
FN:John Doe
ORG:Company Inc
TEL:+1234567890
EMAIL:john@company.com
URL:https://company.com
END:VCARD" \
       vcard.png
```

### 6. WiFi QR Code
```bash
fastqr -s 500 \
       -e H \
       "WIFI:T:WPA;S:NetworkName;P:password123;;" \
       wifi.png
```

### 7. Vietnamese Event
```bash
fastqr -s 700 \
       -f 255,0,0 \
       -b 255,255,200 \
       "Sự kiện: Hội thảo công nghệ
Thời gian: 20/10/2025
Địa điểm: TP.HCM" \
       event.png
```

### 8. Maximum Quality Logo QR
```bash
fastqr -s 2000 \
       -e H \
       -l logo_hires.png \
       -p 30 \
       -q 100 \
       "https://premium.com" \
       premium.png
```

### 9. Batch Generation (7x faster!)
```bash
# Create batch file
cat > products.txt << EOF
Product 001
Product 002
Product 003
EOF

# Generate 1000 QR codes in ~0.4s
fastqr -F products.txt qr_codes/ -s 500 -o
```

## Tips and Best Practices

### Size Selection
- **Small (300-500px)**: Screen display, web pages
- **Medium (500-1000px)**: Print materials, posters
- **Large (1000-2000px)**: Banners, billboards
- **Very Large (2000+px)**: High-resolution prints

### Error Correction
- Use `H` level when adding logos
- Use `H` level for outdoor/damaged environments
- Use `L` or `M` for simple QR codes (more data capacity)

### Colors
- Ensure high contrast (dark QR on light background)
- Avoid very light QR codes (may not scan well)
- Test scannability after changing colors

### Logos
- Use square logos for best results
- Keep logo size 20-30% for reliable scanning
- Use transparent background PNGs
- Always use high error correction (`-e H`)

### File Formats
- Use PNG for lossless quality
- Use JPG for smaller file sizes (web)
- Use WebP for modern browsers (best compression)

## Troubleshooting

### QR Code Won't Scan
1. Increase error correction: `-e H`
2. Reduce logo size: `-p 20` or remove logo
3. Increase contrast (darker QR, lighter background)
4. Increase size: `-s 800`
5. Use optimize flag for sharper pixels: `-o`

### File Size Too Large
1. Use JPG format: `output.jpg`
2. Reduce quality: `-q 75`
3. Use WebP format: `output.webp`

### Colors Not Showing
1. Check RGB values are correct (0-255)
2. Ensure high contrast between foreground and background

### Logo Not Visible
1. Increase logo size: `-p 30`
2. Use high error correction: `-e H`
3. Check logo file exists and is readable

## Getting Help

```bash
# Show all options
fastqr --help

# Show version
fastqr --version

# Report issues
https://github.com/tranhuucanh/fastqr/issues
```

## See Also

- [README.md](../README.md) - Overview and installation
- [Ruby Usage](RUBY_USAGE.md) - Ruby/Rails guide
- [Node.js Usage](NODEJS_USAGE.md) - Node.js guide
- [PHP Usage](PHP_USAGE.md) - PHP guide

