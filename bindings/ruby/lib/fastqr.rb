# frozen_string_literal: true

require_relative "fastqr/version"
require_relative "fastqr/platform"

module FastQR
  class Error < StandardError; end

  # Get library version
  #
  # @return [String] Version string
  def self.version
    cli_path = Platform.find_binary
    output = `#{cli_path} -v 2>&1`.strip
    output.sub('FastQR v', '')
  rescue => e
    VERSION
  end

  # Generate QR code with options
  #
  # @param data [String] Data to encode (UTF-8 supported)
  # @param output_path [String] Path to save the QR code image
  # @param options [Hash] Generation options
  # @option options [Integer] :size Size in pixels (default: 300, QR codes are square)
  # @option options [Boolean] :optimize_size Auto round-up to nearest integer multiple for best performance (default: false)
  # @option options [Array<Integer>] :foreground RGB color array [r, g, b] (default: [0, 0, 0])
  # @option options [Array<Integer>] :background RGB color array [r, g, b] (default: [255, 255, 255])
  # @option options [String] :error_level Error correction level: 'L', 'M', 'Q', 'H' (default: 'M')
  # @option options [String] :logo Path to logo image
  # @option options [Integer] :logo_size Logo size as percentage (default: 20)
  # @option options [Integer] :quality Image quality 1-100 (default: 95)
  # @option options [String] :format Output format: 'png', 'jpg', 'webp' (default: 'png')
  # @return [Boolean] true if successful
  #
  # @example Basic usage
  #   FastQR.generate("Hello World", "qr.png")
  #
  # @example With options
  #   FastQR.generate("Hello", "qr.png",
  #     size: 500,
  #     foreground: [255, 0, 0],
  #     background: [255, 255, 200],
  #     error_level: 'H'
  #   )
  #
  # @example With logo
  #   FastQR.generate("Company", "qr.png",
  #     size: 600,
  #     logo: "logo.png",
  #     logo_size: 25
  #   )
  def self.generate(data, output_path, options = {})
    raise Error, "Data cannot be empty" if data.nil? || data.empty?
    raise Error, "Output path cannot be empty" if output_path.nil? || output_path.empty?

    cli_path = Platform.find_binary
    args = [data, output_path]

    # Build command arguments from options
    args += ['-s', options[:size].to_s] if options[:size]
    args += ['-o'] if options[:optimize_size]
    args += ['-f', options[:foreground].join(',')] if options[:foreground]
    args += ['-b', options[:background].join(',')] if options[:background]
    args += ['-e', options[:error_level]] if options[:error_level]
    args += ['-l', options[:logo]] if options[:logo]
    args += ['-p', options[:logo_size].to_s] if options[:logo_size]
    args += ['-q', options[:quality].to_s] if options[:quality]

    # Execute CLI binary
    result = system(cli_path, *args, out: File::NULL, err: File::NULL)
    raise Error, "Failed to generate QR code" unless result

    true
  end

  # Generate multiple QR codes in batch mode (7x faster!)
  #
  # @param data_array [Array<String>] Array of strings to encode
  # @param output_dir [String] Directory to save QR codes (will be created if it doesn't exist)
  # @param options [Hash] Generation options (same as generate)
  # @return [Hash] Result with :success and :failed counts
  #
  # @example Batch generation
  #   data = ["QR 1", "QR 2", "QR 3"]
  #   FastQR.generate_batch(data, "output_dir/", size: 500)
  #   # Creates: output_dir/1.png, output_dir/2.png, output_dir/3.png
  def self.generate_batch(data_array, output_dir, options = {})
    raise Error, "Data array cannot be empty" if data_array.nil? || data_array.empty?
    raise Error, "Output directory cannot be empty" if output_dir.nil? || output_dir.empty?

    # Create output directory
    require 'fileutils'
    FileUtils.mkdir_p(output_dir)

    # Create a temporary batch file
    require 'tempfile'
    temp_file = Tempfile.new(['fastqr_batch', '.txt'])
    begin
      data_array.each { |line| temp_file.puts(line) }
      temp_file.close

      # Call CLI with batch mode
      cli_path = Platform.find_binary

      # Build command
      cmd_parts = [cli_path, '-F', temp_file.path, output_dir]
      cmd_parts += ['-s', options[:size].to_s] if options[:size]
      cmd_parts += ['-o'] if options[:optimize_size]
      cmd_parts += ['-f', options[:foreground].join(',')] if options[:foreground]
      cmd_parts += ['-b', options[:background].join(',')] if options[:background]
      cmd_parts += ['-e', options[:error_level]] if options[:error_level]
      cmd_parts += ['-l', options[:logo]] if options[:logo]
      cmd_parts += ['-p', options[:logo_size].to_s] if options[:logo_size]
      cmd_parts += ['-q', options[:quality].to_s] if options[:quality]

      result = system(*cmd_parts, out: File::NULL, err: File::NULL)
      raise Error, "Batch generation failed" unless result

      {success: data_array.size, failed: 0}
    ensure
      temp_file.unlink
    end
  end
end

