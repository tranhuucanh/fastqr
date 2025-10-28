# FastQR PHP Usage Guide

Complete guide for using FastQR in PHP and Laravel applications.

## Installation

```bash
composer require fastqr/fastqr
```

**Note:** No system dependencies required! Pre-built binaries are included. 🎉

**Requirements:**
- PHP 7.4 or higher
- FFI extension (usually enabled by default)

## Basic Usage

```php
<?php
require 'vendor/autoload.php';

use FastQR\FastQR;

// Generate QR code
FastQR::generate('Hello World', 'qr.png');
```

## API Reference

### `FastQR::generate($data, $outputPath, $options = [])`

Generate a QR code and save to file.

**Parameters:**
- `$data` (string, required) - Data to encode (UTF-8 supported)
- `$outputPath` (string, required) - Path to save the QR code image
- `$options` (array, optional) - Generation options

**Returns:** `bool` - `true` if successful

**Throws:** `RuntimeException` if generation fails

### Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `'size'` | int | `300` | Output size in pixels (QR codes are square) |
| `'optimizeSize'` | bool | `false` | Auto round-up for best performance |
| `'width'` | int | - | (Deprecated) Use `'size'` instead |
| `'height'` | int | - | (Deprecated) Use `'size'` instead |
| `'foreground'` | array[3] | `[0, 0, 0]` | QR code color (RGB) |
| `'background'` | array[3] | `[255, 255, 255]` | Background color (RGB) |
| `'errorLevel'` | string | `'M'` | Error correction: 'L', 'M', 'Q', 'H' |
| `'logo'` | string | `null` | Path to logo image |
| `'logoSize'` | int | `20` | Logo size as percentage (1-50) |
| `'quality'` | int | `95` | Image quality (1-100) |
| `'format'` | string | `'png'` | Output format: 'png', 'jpg', 'webp' |

### `FastQR::generateBatch($dataArray, $outputDir, $options = [])`

Generate multiple QR codes at once - **7x faster** than calling `generate` multiple times!

**Parameters:**
- `$dataArray` (array, required) - Array of strings to encode
- `$outputDir` (string, required) - Directory to save QR codes (created if doesn't exist)
- `$options` (array, optional) - Same options as `generate`

**Returns:** array - `['success' => int, 'failed' => int]`

**Example:**
```php
$data = ['QR 1', 'QR 2', 'QR 3'];
$result = FastQR::generateBatch($data, 'output/', ['size' => 500, 'optimizeSize' => true]);
// Creates: output/1.png, output/2.png, output/3.png
echo "Generated {$result['success']} QR codes";
```

### `FastQR::version()`

Get library version.

**Returns:** string (e.g., "1.0.0")

```php
echo FastQR::version();
// => "1.0.0"
```

## Examples

### 1. Basic QR Code

```php
<?php
use FastQR\FastQR;

FastQR::generate('https://example.com', 'qr.png');
```

### 2. Custom Size

```php
FastQR::generate('Large QR', 'large.png', [
    'size' => 1000
]);
```

### 3. Colored QR Code

```php
// Red QR on yellow background
FastQR::generate('Colored', 'colored.png', [
    'foreground' => [255, 0, 0],      // Red
    'background' => [255, 255, 200]   // Light yellow
]);
```

### 4. QR Code with Logo

```php
FastQR::generate('Company', 'company.png', [
    'size' => 800,
    'logo' => 'logo.png',
    'logoSize' => 25,
    'errorLevel' => 'H'  // High error correction for logo
]);
```

### 5. High Error Correction

```php
FastQR::generate('Important Data', 'qr.png', [
    'errorLevel' => 'H'  // ~30% recovery capability
]);
```

### 6. UTF-8 Support

```php
// Vietnamese
FastQR::generate('Xin chào Việt Nam! 🇻🇳', 'vietnamese.png');

// Japanese
FastQR::generate('こんにちは日本', 'japanese.png');

// Emoji
FastQR::generate('Hello 👋 World 🌍', 'emoji.png');
```

### 7. Different Formats

```php
// PNG (default)
FastQR::generate('Data', 'output.png');

// JPEG
FastQR::generate('Data', 'output.jpg', ['quality' => 90]);

// WebP
FastQR::generate('Data', 'output.webp', ['quality' => 85]);
```

## Laravel Integration

### Controller Example

```php
<?php

namespace App\Http\Controllers;

use FastQR\FastQR;
use Illuminate\Http\Request;
use Illuminate\Support\Facades\Storage;
use Illuminate\Support\Str;

class QRCodeController extends Controller
{
    public function generate(Request $request)
    {
        $validated = $request->validate([
            'data' => 'required|string',
            'size' => 'nullable|integer|min:100|max:5000',
        ]);

        $filename = 'qr_' . Str::random(16) . '.png';
        $path = storage_path('app/public/qrcodes/' . $filename);

        // Ensure directory exists
        if (!is_dir(dirname($path))) {
            mkdir(dirname($path), 0755, true);
        }

        FastQR::generate($validated['data'], $path, [
            'size' => $validated['size'] ?? 500,
            'errorLevel' => 'M'
        ]);

        return response()->json([
            'success' => true,
            'url' => asset('storage/qrcodes/' . $filename),
            'filename' => $filename
        ]);
    }

    public function generateWithLogo(Request $request)
    {
        $validated = $request->validate([
            'data' => 'required|string',
            'logo' => 'required|file|image|max:2048'
        ]);

        // Save logo
        $logoPath = $request->file('logo')->store('temp', 'local');
        $fullLogoPath = storage_path('app/' . $logoPath);

        // Generate QR
        $filename = 'qr_' . time() . '.png';
        $qrPath = storage_path('app/public/qrcodes/' . $filename);

        FastQR::generate($validated['data'], $qrPath, [
            'size' => 800,
            'logo' => $fullLogoPath,
            'logoSize' => 25,
            'errorLevel' => 'H'
        ]);

        // Clean up logo
        Storage::delete($logoPath);

        return response()->file($qrPath);
    }

    public function download(Request $request)
    {
        $data = $request->input('data');

        $filename = 'qr_' . time() . '.png';
        $path = storage_path('app/temp/' . $filename);

        if (!is_dir(dirname($path))) {
            mkdir(dirname($path), 0755, true);
        }

        FastQR::generate($data, $path, [
            'size' => 600
        ]);

        return response()->download($path)->deleteFileAfterSend(true);
    }
}
```

### Routes

```php
<?php

use App\Http\Controllers\QRCodeController;

Route::prefix('api/qr')->group(function () {
    Route::post('/generate', [QRCodeController::class, 'generate']);
    Route::post('/with-logo', [QRCodeController::class, 'generateWithLogo']);
    Route::post('/download', [QRCodeController::class, 'download']);
});
```

### Service Class

```php
<?php

namespace App\Services;

use FastQR\FastQR;
use Illuminate\Support\Str;

class QRCodeService
{
    protected $outputPath;

    public function __construct()
    {
        $this->outputPath = storage_path('app/public/qrcodes');

        if (!is_dir($this->outputPath)) {
            mkdir($this->outputPath, 0755, true);
        }
    }

    public function generate(string $data, array $options = []): string
    {
        $filename = 'qr_' . Str::random(16) . '.png';
        $filepath = $this->outputPath . '/' . $filename;

        $defaultOptions = [
            'size' => 500,
            'height' => 500,
            'errorLevel' => 'M'
        ];

        FastQR::generate($data, $filepath, array_merge($defaultOptions, $options));

        return $filename;
    }

    public function generateWithLogo(string $data, string $logoPath, array $options = []): string
    {
        $options['logo'] = $logoPath;
        $options['logoSize'] = $options['logoSize'] ?? 25;
        $options['errorLevel'] = 'H';

        return $this->generate($data, $options);
    }

    public function getPath(string $filename): string
    {
        return $this->outputPath . '/' . $filename;
    }

    public function getUrl(string $filename): string
    {
        return asset('storage/qrcodes/' . $filename);
    }

    public function delete(string $filename): bool
    {
        $filepath = $this->getPath($filename);

        if (file_exists($filepath)) {
            return unlink($filepath);
        }

        return false;
    }

    public function cleanup(int $olderThanDays = 7): int
    {
        $deleted = 0;
        $threshold = time() - ($olderThanDays * 24 * 60 * 60);

        $files = glob($this->outputPath . '/qr_*.png');

        foreach ($files as $file) {
            if (filemtime($file) < $threshold) {
                if (unlink($file)) {
                    $deleted++;
                }
            }
        }

        return $deleted;
    }
}
```

### Model Integration

```php
<?php

namespace App\Models;

use Illuminate\Database\Eloquent\Model;
use FastQR\FastQR;

class Event extends Model
{
    protected $fillable = ['name', 'date', 'location', 'url', 'qr_code'];

    protected static function boot()
    {
        parent::boot();

        static::created(function ($event) {
            $event->generateQRCode();
        });
    }

    public function generateQRCode()
    {
        $filename = 'event_' . $this->id . '.png';
        $path = storage_path('app/public/qrcodes/' . $filename);

        if (!is_dir(dirname($path))) {
            mkdir(dirname($path), 0755, true);
        }

        $eventInfo = "Event: {$this->name}\n";
        $eventInfo .= "Date: {$this->date->format('d/m/Y')}\n";
        $eventInfo .= "Location: {$this->location}\n";
        $eventInfo .= "URL: {$this->url}";

        FastQR::generate($eventInfo, $path, [
            'size' => 600,
            'height' => 600,
            'errorLevel' => 'H'
        ]);

        $this->update(['qr_code' => $filename]);
    }

    public function getQRCodeUrlAttribute()
    {
        return $this->qr_code ? asset('storage/qrcodes/' . $this->qr_code) : null;
    }
}
```

### Job Example

```php
<?php

namespace App\Jobs;

use Illuminate\Bus\Queueable;
use Illuminate\Contracts\Queue\ShouldQueue;
use Illuminate\Foundation\Bus\Dispatchable;
use Illuminate\Queue\InteractsWithQueue;
use Illuminate\Queue\SerializesModels;
use App\Models\User;
use FastQR\FastQR;

class GenerateUserQRCode implements ShouldQueue
{
    use Dispatchable, InteractsWithQueue, Queueable, SerializesModels;

    protected $userId;

    public function __construct(int $userId)
    {
        $this->userId = $userId;
    }

    public function handle()
    {
        $user = User::findOrFail($this->userId);

        $vcard = "BEGIN:VCARD\n";
        $vcard .= "VERSION:3.0\n";
        $vcard .= "FN:{$user->name}\n";
        $vcard .= "EMAIL:{$user->email}\n";
        $vcard .= "TEL:{$user->phone}\n";
        $vcard .= "END:VCARD";

        $filename = 'user_' . $user->id . '.png';
        $path = storage_path('app/public/qrcodes/' . $filename);

        FastQR::generate($vcard, $path, [
            'size' => 600,
            'height' => 600,
            'errorLevel' => 'H'
        ]);

        $user->update(['qr_code' => $filename]);
    }
}
```

### Command Example

```php
<?php

namespace App\Console\Commands;

use Illuminate\Console\Command;
use FastQR\FastQR;

class GenerateQRCode extends Command
{
    protected $signature = 'qr:generate {data} {filename?}';
    protected $description = 'Generate a QR code';

    public function handle()
    {
        $data = $this->argument('data');
        $filename = $this->argument('filename') ?? 'qr_' . time() . '.png';
        $path = storage_path('app/public/qrcodes/' . $filename);

        if (!is_dir(dirname($path))) {
            mkdir(dirname($path), 0755, true);
        }

        try {
            FastQR::generate($data, $path, [
                'size' => 600,
                'height' => 600
            ]);

            $this->info("✓ QR code generated: {$filename}");
            $this->info("  Path: {$path}");
        } catch (\Exception $e) {
            $this->error("✗ Failed: " . $e->getMessage());
            return 1;
        }

        return 0;
    }
}
```

## WordPress Integration

### Plugin Example

```php
<?php
/**
 * Plugin Name: FastQR Generator
 * Description: Generate QR codes using FastQR
 * Version: 1.0.0
 */

require_once plugin_dir_path(__FILE__) . 'vendor/autoload.php';

use FastQR\FastQR;

// Add admin menu
add_action('admin_menu', 'fastqr_add_admin_menu');

function fastqr_add_admin_menu() {
    add_menu_page(
        'FastQR Generator',
        'QR Codes',
        'manage_options',
        'fastqr-generator',
        'fastqr_admin_page',
        'dashicons-grid-view'
    );
}

// Admin page
function fastqr_admin_page() {
    if (isset($_POST['generate_qr'])) {
        $data = sanitize_text_field($_POST['qr_data']);
        $filename = 'qr_' . time() . '.png';
        $upload_dir = wp_upload_dir();
        $qr_path = $upload_dir['path'] . '/' . $filename;

        FastQR::generate($data, $qr_path, [
            'size' => 500,
            'height' => 500
        ]);

        $qr_url = $upload_dir['url'] . '/' . $filename;

        echo '<div class="notice notice-success"><p>QR Code generated!</p></div>';
        echo '<img src="' . esc_url($qr_url) . '" alt="QR Code" />';
    }

    ?>
    <div class="wrap">
        <h1>Generate QR Code</h1>
        <form method="post">
            <table class="form-table">
                <tr>
                    <th><label for="qr_data">Data</label></th>
                    <td>
                        <textarea id="qr_data" name="qr_data" rows="5" cols="50" required></textarea>
                    </td>
                </tr>
            </table>
            <?php submit_button('Generate QR Code', 'primary', 'generate_qr'); ?>
        </form>
    </div>
    <?php
}

// Shortcode
add_shortcode('fastqr', 'fastqr_shortcode');

function fastqr_shortcode($atts) {
    $atts = shortcode_atts([
        'data' => '',
        'size' => 300,
        'height' => 300
    ], $atts);

    if (empty($atts['data'])) {
        return '';
    }

    $filename = 'qr_' . md5($atts['data']) . '.png';
    $upload_dir = wp_upload_dir();
    $qr_path = $upload_dir['path'] . '/' . $filename;
    $qr_url = $upload_dir['url'] . '/' . $filename;

    if (!file_exists($qr_path)) {
        FastQR::generate($atts['data'], $qr_path, [
            'width' => intval($atts['width']),
            'height' => intval($atts['height'])
        ]);
    }

    return '<img src="' . esc_url($qr_url) . '" alt="QR Code" class="fastqr-image" />';
}
```

Usage in WordPress:
```
[fastqr data="https://example.com" size="400"]
```

## Plain PHP Examples

### Simple Script

```php
<?php
require 'vendor/autoload.php';

use FastQR\FastQR;

// Get data from form
$data = $_POST['data'] ?? 'Default Data';

// Generate QR code
$filename = 'qr_' . time() . '.png';
$path = __DIR__ . '/qrcodes/' . $filename;

// Ensure directory exists
if (!is_dir(__DIR__ . '/qrcodes')) {
    mkdir(__DIR__ . '/qrcodes', 0755, true);
}

try {
    FastQR::generate($data, $path, [
        'width' => 500,
        'height' => 500
    ]);

    echo "QR code generated: <a href='qrcodes/{$filename}'>Download</a><br>";
    echo "<img src='qrcodes/{$filename}' alt='QR Code' />";
} catch (Exception $e) {
    echo "Error: " . $e->getMessage();
}
```

### API Endpoint

```php
<?php
require 'vendor/autoload.php';

use FastQR\FastQR;

header('Content-Type: application/json');

if ($_SERVER['REQUEST_METHOD'] !== 'POST') {
    http_response_code(405);
    echo json_encode(['error' => 'Method not allowed']);
    exit;
}

$input = json_decode(file_get_contents('php://input'), true);
$data = $input['data'] ?? null;

if (empty($data)) {
    http_response_code(400);
    echo json_encode(['error' => 'Data is required']);
    exit;
}

try {
    $filename = 'qr_' . uniqid() . '.png';
    $path = __DIR__ . '/qrcodes/' . $filename;

    if (!is_dir(dirname($path))) {
        mkdir(dirname($path), 0755, true);
    }

    $options = [
        'width' => $input['width'] ?? 500,
        'height' => $input['height'] ?? 500,
        'errorLevel' => $input['errorLevel'] ?? 'M'
    ];

    FastQR::generate($data, $path, $options);

    echo json_encode([
        'success' => true,
        'url' => '/qrcodes/' . $filename,
        'filename' => $filename
    ]);
} catch (Exception $e) {
    http_response_code(500);
    echo json_encode(['error' => $e->getMessage()]);
}
```

## Error Handling

```php
<?php
use FastQR\FastQR;

try {
    FastQR::generate('Data', 'output.png', [
        'width' => 1000,
        'height' => 1000
    ]);
    echo "✓ QR code generated successfully\n";
} catch (RuntimeException $e) {
    echo "✗ Failed to generate QR code: " . $e->getMessage() . "\n";
    // Log error
    error_log("QR generation failed: " . $e->getMessage());
}
```

## Testing

### PHPUnit Example

```php
<?php
use PHPUnit\Framework\TestCase;
use FastQR\FastQR;

class FastQRTest extends TestCase
{
    private $outputPath;

    protected function setUp(): void
    {
        $this->outputPath = __DIR__ . '/test_qr.png';
    }

    protected function tearDown(): void
    {
        if (file_exists($this->outputPath)) {
            unlink($this->outputPath);
        }
    }

    public function testGenerateBasicQR()
    {
        $result = FastQR::generate('Test Data', $this->outputPath);

        $this->assertTrue($result);
        $this->assertFileExists($this->outputPath);
        $this->assertGreaterThan(0, filesize($this->outputPath));
    }

    public function testGenerateWithOptions()
    {
        $result = FastQR::generate('Test', $this->outputPath, [
            'size' => 500,
            'height' => 500,
            'errorLevel' => 'H'
        ]);

        $this->assertTrue($result);
        $this->assertFileExists($this->outputPath);
    }

    public function testEmptyDataThrowsException()
    {
        $this->expectException(RuntimeException::class);
        FastQR::generate('', $this->outputPath);
    }

    public function testVersion()
    {
        $version = FastQR::version();
        $this->assertMatchesRegularExpression('/^\d+\.\d+\.\d+$/', $version);
    }
}
```

## Performance Tips

- Cache generated QR codes using data hash as filename
- Use lower quality for web display (`quality` => 75)
- Use PNG for best quality, JPG for smaller files
- Clean up old temporary files regularly

## Troubleshooting

### "Library not found" Error

The package includes pre-built binaries. If you see this error, check PHP FFI:

```php
<?php
// Check if FFI is enabled
if (!extension_loaded('ffi')) {
    echo "FFI extension is not enabled\n";
} else {
    echo "FFI extension is enabled\n";
}
```

### File Permission Errors

Ensure output directory has write permissions:

```bash
chmod 755 storage/app/public/qrcodes
```

## See Also

- [CLI Usage](CLI_USAGE.md) - Command-line usage
- [Ruby Usage](RUBY_USAGE.md) - Ruby/Rails guide
- [Node.js Usage](NODEJS_USAGE.md) - Node.js guide
- [GitHub Repository](https://github.com/tranhuucanh/fastqr)
- [Packagist Package](https://packagist.org/packages/fastqr/fastqr)

