# frozen_string_literal: true

Gem::Specification.new do |spec|
  spec.name          = "fastqr"
  spec.version       = "1.0.21"
  spec.authors       = ["FastQR Project"]
  spec.email         = ["fastqr@example.com"]

  spec.summary       = "The fastest QR code generator on the planet."
  spec.description   = "Generate 1,000 QR codes in **0.37 seconds**. Full UTF-8 support. Custom colors. Logo embedding. Precise size control."
  spec.homepage      = "https://github.com/tranhuucanh/fastqr"
  spec.license       = "LGPL-2.1"
  spec.required_ruby_version = ">= 2.7.0"

  spec.metadata["homepage_uri"] = spec.homepage
  spec.metadata["source_code_uri"] = spec.homepage
  spec.metadata["changelog_uri"] = "#{spec.homepage}/blob/main/CHANGELOG.md"

  spec.files = Dir.chdir(File.expand_path(__dir__)) do
    files = `git ls-files -z`.split("\x0").reject do |f|
      (f == __FILE__) || f.match(%r{\A(?:(?:bin|test|spec|features)/|\.(?:git|travis|circleci)|appveyor)})
    end

    # Include pre-built binaries if they exist (even if not in git)
    # This is for CI builds where binaries are downloaded before gem build
    prebuilt_files = Dir.glob("bindings/ruby/prebuilt/**/*").select { |f| File.file?(f) }
    if prebuilt_files.any?
      puts "📦 Including #{prebuilt_files.length} pre-built binary files in gem"
      files += prebuilt_files
    end

    files
  end

  spec.extensions    = ["bindings/ruby/extconf.rb"]
  spec.require_paths = ["bindings/ruby/lib"]

  # No runtime dependencies - uses pre-built CLI binary

  spec.add_development_dependency "rake", "~> 13.0"
  spec.add_development_dependency "rake-compiler", "~> 1.2"
  spec.add_development_dependency "minitest", "~> 5.0"
end

