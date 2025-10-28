# FastQR Ruby/Rails Usage Guide

Complete guide for using FastQR in Ruby and Ruby on Rails applications.

## Installation

```bash
gem install fastqr
```

Or add to your `Gemfile`:

```ruby
gem 'fastqr', '~> 1.0'
```

Then run:
```bash
bundle install
```

**Note:** No system dependencies required! Pre-built binaries are included. 🎉

## Basic Usage

```ruby
require 'fastqr'

# Generate QR code
FastQR.generate("Hello World", "qr.png")
```

## API Reference

### `FastQR.generate(data, output_path, options = {})`

Generate a QR code and save to file.

**Parameters:**
- `data` (String, required) - Data to encode (UTF-8 supported)
- `output_path` (String, required) - Path to save the QR code image
- `options` (Hash, optional) - Generation options

**Returns:** `true` if successful

**Raises:** `FastQR::Error` if generation fails

### Options

| Option | Type | Default | Description |
|--------|------|---------|-------------|
| `:size` | Integer | `300` | Output size in pixels (QR codes are square) |
| `:optimize_size` | Boolean | `false` | Auto round-up for best performance |
| `:width` | Integer | - | (Deprecated) Use `:size` instead |
| `:height` | Integer | - | (Deprecated) Use `:size` instead |
| `:foreground` | Array[3] | `[0, 0, 0]` | QR code color (RGB) |
| `:background` | Array[3] | `[255, 255, 255]` | Background color (RGB) |
| `:error_level` | String | `'M'` | Error correction: 'L', 'M', 'Q', 'H' |
| `:logo` | String | `nil` | Path to logo image |
| `:logo_size` | Integer | `20` | Logo size as percentage (1-50) |
| `:quality` | Integer | `95` | Image quality (1-100) |
| `:format` | String | `'png'` | Output format: 'png', 'jpg', 'webp' |

### `FastQR.generate_batch(data_array, output_dir, options = {})`

Generate multiple QR codes at once - **7x faster** than calling `generate` multiple times!

**Parameters:**
- `data_array` (Array[String], required) - Array of strings to encode
- `output_dir` (String, required) - Directory to save QR codes (created if doesn't exist)
- `options` (Hash, optional) - Same options as `generate`

**Returns:** Hash with `:success` and `:failed` counts

**Example:**
```ruby
data = ["QR 1", "QR 2", "QR 3"]
FastQR.generate_batch(data, "output/", size: 500, optimize_size: true)
# Creates: output/1.png, output/2.png, output/3.png
```

### `FastQR.version`

Get library version.

**Returns:** String (e.g., "1.0.0")

```ruby
puts FastQR.version
# => "1.0.0"
```

## Examples

### 1. Basic QR Code

```ruby
FastQR.generate("https://example.com", "qr.png")
```

### 2. Custom Size

```ruby
FastQR.generate("Large QR", "large.png",
  size: 1000
)
```

### 3. Optimized Size (faster generation)

```ruby
FastQR.generate("Fast QR", "fast.png",
  size: 500,
  optimize_size: true
)
```

### 4. Colored QR Code

```ruby
# Red QR on yellow background
FastQR.generate("Colored", "colored.png",
  size: 500,
  foreground: [255, 0, 0],      # Red
  background: [255, 255, 200]   # Light yellow
)
```

### 5. QR Code with Logo

```ruby
FastQR.generate("Company", "company.png",
  size: 800,
  logo: "logo.png",
  logo_size: 25,
  error_level: 'H'  # High error correction for logo
)
```

### 6. High Error Correction

```ruby
FastQR.generate("Important Data", "qr.png",
  error_level: 'H'  # ~30% recovery capability
)
```

### 7. UTF-8 Support

```ruby
# Vietnamese
FastQR.generate("Xin chào Việt Nam! 🇻🇳", "vietnamese.png")

# Japanese
FastQR.generate("こんにちは日本", "japanese.png")

# Emoji
FastQR.generate("Hello 👋 World 🌍", "emoji.png")
```

### 8. Different Formats

```ruby
# PNG (default)
FastQR.generate("Data", "output.png")

# JPEG
FastQR.generate("Data", "output.jpg", quality: 90)

# WebP
FastQR.generate("Data", "output.webp", quality: 85)
```

### 9. Batch Generation (7x faster!)

```ruby
# Generate 1000 QR codes
data = (1..1000).map { |i| "Product #{i}" }

# Old way (slow - ~3 seconds)
# data.each_with_index do |text, i|
#   FastQR.generate(text, "qr_#{i+1}.png", size: 500)
# end

# New way (fast - ~0.4 seconds!)
result = FastQR.generate_batch(data, "qr_codes/",
  size: 500,
  optimize_size: true
)
puts "Generated #{result[:success]} QR codes"
# Creates: qr_codes/1.png, qr_codes/2.png, ..., qr_codes/1000.png
```

## Rails Integration

### Controller Example

```ruby
class QrCodesController < ApplicationController
  def create
    url = params[:url]
    filename = "qr_#{SecureRandom.hex(8)}.png"
    filepath = Rails.root.join('public', 'qrcodes', filename)

    # Ensure directory exists
    FileUtils.mkdir_p(File.dirname(filepath))

    # Generate QR code
    FastQR.generate(url, filepath.to_s,
      size: 500,
      error_level: 'M'
    )

    # Send file to user
    send_file filepath, type: 'image/png', disposition: 'inline'
  end

  def generate_with_logo
    data = params[:data]
    logo_path = Rails.root.join('app', 'assets', 'images', 'logo.png')
    output_path = Rails.root.join('tmp', "qr_#{Time.now.to_i}.png")

    FastQR.generate(data, output_path.to_s,
      size: 600,
      logo: logo_path.to_s,
      logo_size: 25,
      error_level: 'H',
      foreground: [0, 100, 200]
    )

    send_file output_path, type: 'image/png'
  end
end
```

### Model Example

```ruby
class Event < ApplicationRecord
  after_create :generate_qr_code

  def qr_code_path
    Rails.root.join('public', 'events', 'qrcodes', "#{id}.png")
  end

  def generate_qr_code
    FileUtils.mkdir_p(File.dirname(qr_code_path))

    event_info = <<~INFO
      Event: #{name}
      Date: #{event_date.strftime('%d/%m/%Y')}
      Location: #{location}
      URL: #{url}
    INFO

    FastQR.generate(event_info, qr_code_path.to_s,
      size: 500,
      error_level: 'H'
    )
  end

  def qr_code_url
    "/events/qrcodes/#{id}.png"
  end
end
```

### View Helper

```ruby
# app/helpers/qr_code_helper.rb
module QrCodeHelper
  def generate_inline_qr(data, options = {})
    filename = "qr_#{Digest::MD5.hexdigest(data)}.png"
    filepath = Rails.root.join('tmp', filename)

    unless File.exist?(filepath)
      FastQR.generate(data, filepath.to_s, options)
    end

    image_tag("/tmp/#{filename}", alt: 'QR Code', class: 'qr-code')
  end
end
```

### Background Job Example (Sidekiq)

```ruby
class GenerateQrCodeJob < ApplicationJob
  queue_as :default

  def perform(user_id)
    user = User.find(user_id)
    vcard = generate_vcard(user)
    output_path = Rails.root.join('public', 'users', 'qrcodes', "#{user.id}.png")

    FileUtils.mkdir_p(File.dirname(output_path))

    FastQR.generate(vcard, output_path.to_s,
      size: 600,
      error_level: 'H'
    )

    user.update(qr_code_generated: true)
  end

  private

  def generate_vcard(user)
    <<~VCARD
      BEGIN:VCARD
      VERSION:3.0
      FN:#{user.full_name}
      EMAIL:#{user.email}
      TEL:#{user.phone}
      END:VCARD
    VCARD
  end
end
```

### API Endpoint Example

```ruby
# app/controllers/api/v1/qr_codes_controller.rb
module Api
  module V1
    class QrCodesController < ApplicationController
      def create
        data = qr_params[:data]
        options = build_options(qr_params)

        # Generate to temp file
        filename = "qr_#{SecureRandom.uuid}.png"
        filepath = Rails.root.join('tmp', filename)

        FastQR.generate(data, filepath.to_s, options)

        # Return base64 encoded image
        image_data = File.read(filepath)
        base64_image = Base64.strict_encode64(image_data)

        File.delete(filepath)

        render json: {
          success: true,
          image: "data:image/png;base64,#{base64_image}",
          filename: filename
        }
      rescue FastQR::Error => e
        render json: { success: false, error: e.message }, status: :unprocessable_entity
      end

      private

      def qr_params
        params.permit(:data, :size, :error_level, :quality)
      end

      def build_options(params)
        {}.tap do |opts|
          opts[:width] = params[:width].to_i if params[:width]
          opts[:height] = params[:height].to_i if params[:height]
          opts[:error_level] = params[:error_level] if params[:error_level]
          opts[:quality] = params[:quality].to_i if params[:quality]
        end
      end
    end
  end
end
```

## Advanced Usage

### Batch Generation

```ruby
urls = [
  "https://example.com/page1",
  "https://example.com/page2",
  "https://example.com/page3"
]

urls.each_with_index do |url, index|
  FastQR.generate(url, "qr_#{index + 1}.png",
    size: 500
  )
end
```

### Dynamic Colors

```ruby
def generate_branded_qr(data, brand_color)
  r, g, b = brand_color

  FastQR.generate(data, "branded_qr.png",
    size: 600,
    foreground: [r, g, b],
    background: [255, 255, 255],
    error_level: 'H'
  )
end

generate_branded_qr("Company Data", [0, 120, 215])  # Microsoft blue
```

### Error Handling

```ruby
begin
  FastQR.generate("Data", "output.png",
    size: 1000
  )
  puts "✓ QR code generated successfully"
rescue FastQR::Error => e
  puts "✗ Failed to generate QR code: #{e.message}"
  # Log error or notify admin
  Rails.logger.error("QR generation failed: #{e.message}")
end
```

### Validation Helper

```ruby
class QrCodeValidator
  def self.valid_size?(size)
    size.between?(100, 5000)
  end

  def self.valid_error_level?(level)
    %w[L M Q H].include?(level.to_s.upcase)
  end

  def self.validate_options(options)
    errors = []

    if options[:size]
      size = options[:size]
      errors << "Invalid size" unless valid_size?(size)
    end

    if options[:error_level]
      errors << "Invalid error level" unless valid_error_level?(options[:error_level])
    end

    errors
  end
end
```

## Testing

### RSpec Example

```ruby
# spec/services/qr_code_generator_spec.rb
require 'rails_helper'

RSpec.describe 'QR Code Generation' do
  let(:output_path) { Rails.root.join('tmp', 'test_qr.png') }

  after do
    File.delete(output_path) if File.exist?(output_path)
  end

  it 'generates a basic QR code' do
    result = FastQR.generate("Test Data", output_path.to_s)

    expect(result).to be true
    expect(File.exist?(output_path)).to be true
    expect(File.size(output_path)).to be > 0
  end

  it 'generates QR code with custom options' do
    result = FastQR.generate("Test", output_path.to_s,
      size: 500,
      error_level: 'H'
    )

    expect(result).to be true
    expect(File.exist?(output_path)).to be true
  end

  it 'raises error for empty data' do
    expect {
      FastQR.generate("", output_path.to_s)
    }.to raise_error(FastQR::Error, /cannot be empty/)
  end
end
```

## Performance Tips

- Generate QR codes in background jobs for large batches
- Cache generated QR codes (use MD5 hash of data as filename)
- Use lower quality settings for web display (`quality: 75`)
- Use PNG for best quality, JPG for smaller files

## Troubleshooting

### "Library not found" Error

The gem includes pre-built binaries. If you see this error:

```ruby
# Check if binary is available
puts FastQR::Platform.platform
puts FastQR::Platform.lib_path
puts File.exist?(FastQR::Platform.lib_path)
```

### QR Code Not Generated

Check file permissions and ensure output directory exists:

```ruby
require 'fileutils'
FileUtils.mkdir_p(File.dirname(output_path))
```

## See Also

- [CLI Usage](CLI_USAGE.md) - Command-line usage
- [Node.js Usage](NODEJS_USAGE.md) - Node.js guide
- [PHP Usage](PHP_USAGE.md) - PHP guide
- [GitHub Repository](https://github.com/tranhuucanh/fastqr)

