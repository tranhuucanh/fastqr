require 'mkmf'

# Check for required libraries
unless have_library('qrencode')
  abort "ERROR: libqrencode is required. Install it first."
end

unless have_library('vips')
  abort "ERROR: libvips is required. Install it first."
end

# Check for headers
unless have_header('qrencode.h')
  abort "ERROR: qrencode.h not found"
end

unless have_header('vips/vips.h')
  abort "ERROR: vips/vips.h not found"
end

# Add C++14 support
$CXXFLAGS << " -std=c++14"

# Set source directory
$srcs = ['fastqr_ruby.cpp', '../../src/fastqr.cpp']
$INCFLAGS << " -I$(srcdir)/../../include"

create_makefile('fastqr/fastqr')

