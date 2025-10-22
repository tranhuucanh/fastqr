# frozen_string_literal: true

module FastQR
  # Platform detection and pre-built binary management
  module Platform
    class << self
      # Detects the current platform
      # @return [String] Platform identifier (e.g., 'macos-arm64', 'linux-x86_64')
      def detect
        os = RbConfig::CONFIG['host_os']
        arch = RbConfig::CONFIG['host_cpu']

        case os
        when /darwin/
          case arch
          when /arm64|aarch64/
            'macos-arm64'
          when /x86_64|x64/
            'macos-x86_64'
          else
            raise "Unsupported macOS architecture: #{arch}"
          end
        when /linux/
          case arch
          when /x86_64|x64/
            'linux-x86_64'
          when /arm64|aarch64/
            'linux-aarch64'
          else
            raise "Unsupported Linux architecture: #{arch}"
          end
        else
          raise "Unsupported platform: #{os}"
        end
      end

      # Check if pre-built binary is available
      # @return [Boolean] true if binary exists
      def prebuilt_available?
        binary_path = find_binary
        binary_path && File.exist?(binary_path)
      rescue StandardError
        false
      end

      # Get path to the library file
      # @return [String] Path to libfastqr.dylib or libfastqr.so
      def lib_path
        platform = detect
        base_dir = File.expand_path('../../prebuilt', __dir__)
        lib_file = platform.start_with?('macos') ? 'libfastqr.dylib' : 'libfastqr.so'
        File.join(base_dir, platform, 'lib', lib_file)
      end

      # Get binary name
      # @return [String] 'fastqr'
      def binary_name
        'fastqr'
      end

      # Extracts pre-built binary from tarball
      # @param tarball_path [String] Path to the tarball
      # @param dest_dir [String] Destination directory
      def extract_binary(tarball_path, dest_dir)
        FileUtils.mkdir_p(dest_dir)
        system("tar -xzf '#{tarball_path}' -C '#{dest_dir}'") or raise "Failed to extract #{tarball_path}"
      end

      # Finds the fastqr binary
      # @return [String] Path to fastqr binary
      def find_binary
        platform = detect
        base_dir = File.expand_path('../../prebuilt', __dir__)
        binary_path = File.join(base_dir, platform, 'bin', 'fastqr')

        if platform.start_with?('linux')
          # Linux: Check for AppImage first
          if File.exist?(binary_path) && File.executable?(binary_path)
            # Test if AppImage can run (version check)
            begin
              output = `#{binary_path} -v 2>&1`.strip
              if $?.success? && output.include?('FastQR')
                return binary_path
              end
            rescue => e
              # AppImage failed, continue to fallback
            end
          end
        else
          # macOS/Windows: Use regular binary
          return binary_path if File.exist?(binary_path)
        end

        # Try to extract from tarball
        tarball_path = File.join(base_dir, "#{platform}.tar.gz")
        if File.exist?(tarball_path)
          puts "Extracting pre-built binary from #{tarball_path}..."
          extract_binary(tarball_path, File.join(base_dir, platform))

          if File.exist?(binary_path)
            File.chmod(0755, binary_path)
            return binary_path
          end
        end

        raise "Pre-built binary not found for #{platform}"
      end
    end
  end
end
