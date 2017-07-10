
#
#  This file is part of the "Teapot" project, and is released under the MIT license.
#

teapot_version "1.0"

# Project Metadata

define_project "async-http" do |project|
	project.title = "Async HTTP"
	
	project.summary = 'A brief one line summary of the project.'
	
	project.license = "MIT License"
	
	project.add_author 'Samuel Williams', email: 'samuel.williams@oriontransfer.co.nz'
	
	project.version = "1.0.0"
end

# Build Targets

define_target 'async-http-library' do |target|
	target.build do
		source_root = target.package.path + 'source'
		cache_prefix = Path.join(environment[:build_prefix], "async-http-#{environment.checksum}")
		
		copy headers: source_root.glob('Async/**/*.hpp')
		
		convert source_file: source_root + 'Async/HTTP/RequestParser.rl', destination_path: cache_prefix + 'Async/HTTP/RequestParser.cpp'
		convert source_file: source_root + 'Async/HTTP/ResponseParser.rl', destination_path: cache_prefix + 'Async/HTTP/ResponseParser.cpp'
		
		build static_library: "AsyncHTTP", source_files: (source_root.glob('Async/**/*.cpp') + cache_prefix.glob('Async/**/*.cpp'))
	end
	
	target.depends 'Build/Files'
	target.depends 'Build/Clang'
	
	target.depends :platform
	target.depends "Language/C++14", private: true
	
	target.depends "Build/Files"
	target.depends "Build/Clang"
	target.depends "Convert/Ragel", private: true
	
	target.depends "Library/Buffers"
	target.depends "Library/Logger"
	
	target.depends "Library/Async"
	target.depends "Library/AsyncNetwork"
	
	target.provides "Library/AsyncHTTP" do
		append linkflags [
			->{install_prefix + 'lib/libAsyncHTTP.a'},
		]
	end
end

define_target "async-http-tests" do |target|
	target.build do |*arguments|
		test_root = target.package.path + 'test'
		
		run tests: "AsyncHTTP", source_files: test_root.glob('Async/**/*.cpp'), arguments: arguments
	end
	
	target.depends :platform
	target.depends "Language/C++14", private: true
	
	target.depends "Library/UnitTest"
	target.depends "Library/AsyncHTTP"

	target.provides "Test/AsyncHTTP"
end

# Configurations

define_configuration "async-http" do |configuration|
	configuration[:source] = "https://github.com/kurocha"
	
	configuration.require "memory"
	configuration.require "time"
	configuration.require "concurrent"
	configuration.require "logger"
	
	configuration.require "async"
	configuration.require "async-network"
	
	configuration.require "buffers"
	
	configuration.require "ragel"
	
	# Provides all the build related infrastructure:
	configuration.require "platforms"
	configuration.require "build-files"
	
	# Provides unit testing infrastructure and generators:
	configuration.require "unit-test"
	
	# Provides some useful C++ generators:
	configuration.require "generate-cpp-class"
	
	configuration.require 'generate-project'
	configuration.require 'generate-travis'
end
