# FastQR - Fast QR Code Generator Library
# Copyright (C) 2025 Tran Huu Canh and FastQR Contributors
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library; if not, see <https://www.gnu.org/licenses/>.
#
# Homepage: https://github.com/tranhuucanh/fastqr

require 'mkmf'
require 'rbconfig'

# Check if pre-built binary exists
def check_prebuilt_binary
  os = RbConfig::CONFIG['host_os']
  arch = RbConfig::CONFIG['host_cpu']

  platform = case os
  when /darwin/
    case arch
    when /arm64|aarch64/
      'macos-arm64'
    when /x86_64|x64/
      'macos-x86_64'
    else
      nil
    end
  when /linux/
    case arch
    when /x86_64|x64/
      'linux-x86_64'
    when /arm64|aarch64/
      'linux-aarch64'
    else
      nil
    end
  else
    nil
  end

  return false unless platform

  # When installed as gem, binaries are in bindings/ruby/prebuilt/
  # When running from repo, they're in ../../prebuilt/
  prebuilt_dir = File.expand_path("./prebuilt/#{platform}", __dir__)
  binary_path = File.join(prebuilt_dir, 'bin', 'fastqr')

  if File.exist?(binary_path)
    puts "✅ Found pre-built binary at #{binary_path}"
    puts "⏭️  Skipping compilation"

    # Create a dummy Makefile that does nothing
    File.open('Makefile', 'w') do |f|
      f.puts "all:\n\t@echo 'Using pre-built binary'\n"
      f.puts "install:\n\t@echo 'Using pre-built binary'\n"
      f.puts "clean:\n\t@echo 'Nothing to clean'\n"
    end

    return true
  end

  false
end

# Try to use pre-built binary first
exit 0 if check_prebuilt_binary

# If no pre-built binary, compile from source
puts "⚠️  No pre-built binary found, compiling from source..."

# Check for required libraries
unless have_library('qrencode')
  abort "ERROR: libqrencode is required. Install it first."
end

unless have_library('png')
  abort "ERROR: libpng is required. Install it first."
end

# Check for headers
unless have_header('qrencode.h')
  abort "ERROR: qrencode.h not found"
end

unless have_header('png.h')
  abort "ERROR: png.h not found"
end

# Add C++14 support
$CXXFLAGS << " -std=c++14"

# Set source directory
$srcs = ['fastqr_ruby.cpp', '../../src/fastqr.cpp']
$INCFLAGS << " -I$(srcdir)/../../include"

create_makefile('fastqr/fastqr')

