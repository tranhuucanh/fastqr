#!/usr/bin/env ruby
require 'fastqr'

puts "🧪 Testing FastQR gem..."
puts "FastQR version: #{FastQR.version}"
puts ""

# Test 1: Basic QR code generation
puts "Test 1: Generate basic QR code..."
output_file = "test_output.png"
result = FastQR.generate("Hello World", output_file)

if result && File.exist?(output_file)
  size = File.size(output_file)
  puts "✅ Success! Generated #{output_file} (#{size} bytes)"
else
  puts "❌ Failed to generate QR code"
  exit 1
end

# Test 2: QR code with custom size
puts "\nTest 2: Generate QR code with custom size (500)..."
output_file2 = "test_output_500.png"
result = FastQR.generate("FastQR Test", output_file2, size: 500)

if result && File.exist?(output_file2)
  size = File.size(output_file2)
  puts "✅ Success! Generated #{output_file2} (#{size} bytes)"
else
  puts "❌ Failed to generate QR code with custom size"
  exit 1
end

# Test 3: UTF-8 support (Vietnamese)
puts "\nTest 3: Generate QR code with Vietnamese text..."
output_file3 = "test_output_vietnamese.png"
result = FastQR.generate("Xin chào Việt Nam 🇻🇳", output_file3)

if result && File.exist?(output_file3)
  size = File.size(output_file3)
  puts "✅ Success! Generated #{output_file3} (#{size} bytes)"
else
  puts "❌ Failed to generate Vietnamese QR code"
  exit 1
end

puts "\n" + "="*50
puts "🎉 All tests passed!"
puts "="*50
puts "\nGenerated files:"
Dir.glob("test_output*.png").each do |file|
  puts "  - #{file} (#{File.size(file)} bytes)"
end

