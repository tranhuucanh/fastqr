# frozen_string_literal: true

module FastQR
  module Platform
    # Detect current platform
    def self.os
      @os ||= case RbConfig::CONFIG['host_os']
      when /darwin/i then 'macos'
      when /linux/i then 'linux'
      when /mswin|mingw|cygwin/i then 'windows'
      else 'unknown'
      end
    end

    # Detect CPU architecture
    def self.arch
      @arch ||= case RbConfig::CONFIG['host_cpu']
      when /x86_64|x64|amd64/i then 'x86_64'
      when /aarch64|arm64/i then 'arm64'
      when /arm/i then 'arm'
      else RbConfig::CONFIG['host_cpu']
      end
    end

    # Get platform string (e.g., "macos-arm64")
    def self.platform
      "#{os}-#{arch}"
    end

    # Get library extension
    def self.lib_ext
      case os
      when 'macos' then 'dylib'
      when 'linux' then 'so'
      when 'windows' then 'dll'
      else 'so'
      end
    end

    # Check if pre-built binary is available
    def self.prebuilt_available?
      File.exist?(lib_path)
    end

    # Get path to pre-built library
    def self.lib_path
      File.expand_path("../../prebuilt/#{platform}/lib/libfastqr.#{lib_ext}", __dir__)
    end
  end
end

