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
            'linux-arm64'
          else
            raise "Unsupported Linux architecture: #{arch}"
          end
        else
          raise "Unsupported platform: #{os}"
        end
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
        prebuilt_dir = File.expand_path("../../prebuilt/#{platform}", __dir__)
        binary_path = File.join(prebuilt_dir, 'fastqr')

        return binary_path if File.exist?(binary_path)

        # Try to extract from tarball
        tarball_path = File.expand_path("../../prebuilt/#{platform}.tar.gz", __dir__)
        if File.exist?(tarball_path)
          puts "Extracting pre-built binary from #{tarball_path}..."
          extract_binary(tarball_path, prebuilt_dir)

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
