
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "3.0"

# Project Metadata

define_project "async-http" do |project|
	project.title = "Async HTTP"
	
	project.summary = 'Provides HTTP/1.x client and server implementations.'
	
	project.license = "MIT License"
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	
	project.version = "1.0.0"
end

# Build Targets

define_target 'async-http-headers' do |target|
	source_root = target.package.path + 'source'
	
	target.provides "Headers/AsyncHTTP" do
		append header_search_paths source_root
	end
end

define_target 'async-http-library' do |target|
	target.depends "Language/C++14"
	
	target.depends "Library/Async", public: true
	target.depends "Library/AsyncNetwork", public: true
	target.depends "Library/URI", public: true
	
	target.depends "Library/Memory", public: true
	target.depends "Library/Logger", public: true
	
	target.depends "Convert/Ragel"
	
	target.depends "Headers/AsyncHTTP", public: true
	
	target.provides "Library/AsyncHTTP" do
		source_root = target.package.path + 'source'
		cache_prefix = environment[:build_prefix] + environment.checksum
		
		parsers = source_root.glob('Async/HTTP/**/*Parser.rl')
		
		implementation_files = parsers.collect do |file|
			implementation_file = cache_prefix / (file.relative_path + '.cpp')
			convert source_file: file, destination_path: implementation_file
		end
		
		library_path = build static_library: "AsyncHTTP", source_files: source_root.glob('Async/HTTP/**/*.cpp') + implementation_files
		
		append linkflags library_path
	end
end

define_target "async-http-tests" do |target|
	target.depends 'Library/UnitTest'
	target.depends "Language/C++14", private: true
	
	target.depends "Library/AsyncHTTP"
	
	target.provides "Test/AsyncHTTP" do |*arguments|
		test_root = target.package.path + 'test'
		
		run source_files: test_root.glob('Async/HTTP/**/*.cpp'), arguments: arguments
	end
end

# Configurations

define_configuration "development" do |configuration|
	configuration[:source] = "https://github.com/kurocha/"
	configuration.import "async-http"
	
	# Provides all the build related infrastructure:
	configuration.require "platforms"
	configuration.require "build-files"
	
	# Provides unit testing infrastructure and generators:
	configuration.require "unit-test"
	
	# Provides some useful C++ generators:
	configuration.require "generate-travis"
	configuration.require "generate-project"
	configuration.require "generate-cpp-class"
end

define_configuration "async-http" do |configuration|
	configuration.public!
	
	configuration.require "async-network"
	configuration.require "logger"
end
