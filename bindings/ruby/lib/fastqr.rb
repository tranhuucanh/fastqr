# frozen_string_literal: true

require_relative "fastqr/version"
require_relative "fastqr/platform"

# Load pre-built binary if available, otherwise try to load compiled extension
begin
  if FastQR::Platform.prebuilt_available?
    # Load from pre-built binary
    require 'ffi'

    module FastQR
      module Native
        extend FFI::Library

        lib_path = Platform.lib_path
        ffi_lib lib_path

        # Define C functions
        attach_function :fastqr_generate_c, :fastqr_generate, [:string, :string, :pointer], :bool
        attach_function :fastqr_version, [], :string
      end
    end
  else
    # Fall back to compiled extension
    require_relative "fastqr/fastqr"
  end
rescue LoadError => e
  warn "Warning: Could not load FastQR native extension: #{e.message}"
  warn "Please run: gem install fastqr -- --with-system-libraries"
end

module FastQR
  class Error < StandardError; end

  # Generate QR code with options
  #
  # @param data [String] Data to encode (UTF-8 supported)
  # @param output_path [String] Path to save the QR code image
  # @param options [Hash] Generation options
  # @option options [Integer] :width Width in pixels (default: 300)
  # @option options [Integer] :height Height in pixels (default: 300)
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
  #     width: 500,
  #     height: 500,
  #     foreground: [255, 0, 0],
  #     background: [255, 255, 200],
  #     error_level: 'H'
  #   )
  #
  # @example With logo
  #   FastQR.generate("Company", "qr.png",
  #     width: 600,
  #     height: 600,
  #     logo: "logo.png",
  #     logo_size: 25
  #   )
  def self.generate(data, output_path, options = {})
    raise Error, "Data cannot be empty" if data.nil? || data.empty?
    raise Error, "Output path cannot be empty" if output_path.nil? || output_path.empty?

    result = super(data, output_path, options)
    raise Error, "Failed to generate QR code" unless result

    result
  end
end

