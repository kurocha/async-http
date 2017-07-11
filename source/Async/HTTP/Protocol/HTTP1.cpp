//
//  HTTP1.cpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 10/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "HTTP1.hpp"

#include "../RequestParser.hpp"
#include "../ResponseParser.hpp"

#include <Buffers/OutputStream.hpp>

#include <sstream>

#include <Logger/Console.hpp>

namespace Async
{
	namespace HTTP
	{
		namespace Protocol
		{
			using namespace Logger;
			
			HTTP1::HTTP1(Network::Socket & socket, Reactor & reactor) : StreamProtocol(socket, reactor), _buffer(1024*8, true)
			{
			}
			
			HTTP1::~HTTP1()
			{
			}
			
			bool HTTP1::fill_buffer()
			{
				if (_buffer.size() == 0) {
					auto mark = read(_buffer.end(), _buffer.top());
					
					Console::debug(__PRETTY_FUNCTION__, mark - _buffer.end());
					
					if (mark == _buffer.end()) {
						return false;
					} else {
						_buffer.expand(mark - _buffer.end());
						return true;
					}
				}
				
				return true;
			}
			
			bool HTTP1::read_request(Request & request)
			{
				auto parser = RequestParser(request);
				
				while (!parser.complete()) {
					if (fill_buffer() == false) {
						// connection shutdown
						return false;
					}
					
					auto mark = parser.parse(_buffer.begin(), _buffer.end());
					
					if (mark != _buffer.end()) {
						Console::error("Couldn't parse buffer", (void*)mark, (void*) _buffer.end());
						Console::error("contents:", std::string((const char *)mark, (const char *)_buffer.end()));
						return false;
					} else {
						_buffer.resize(0);
					}
				}
				
				return true;
			}
			
			bool HTTP1::read_response(Response & response)
			{
				auto parser = ResponseParser(response);
				
				while (!parser.complete()) {
					if (fill_buffer() == false) {
						// connection shutdown
						return false;
					}
					
					auto mark = parser.parse(_buffer.begin(), _buffer.end());
					
					if (mark != _buffer.end()) {
						Console::error("Couldn't parse buffer", (void*)mark, (void*) _buffer.end());
						Console::error("contents:", std::string((const char *)mark, (const char *)_buffer.end()));
						return false;
					} else {
						_buffer.resize(0);
					}
				}
				
				return true;
			}
			
			void HTTP1::write_response(const Response & response)
			{
				write_response(response.status, response.headers, response.body);
			}
			
			void HTTP1::write_request(const Request & request)
			{
				write_request(request.method, request.target, request.version, request.headers, request.body);
			}
			
			void HTTP1::write_request(const std::string & method, const std::string & target, const std::string & version, const std::map<std::string, std::string> & headers, const std::string & body)
			{
				std::stringstream stream;
				
				stream << method << ' ' << target << ' ' << version << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << "\r\n";
				}
				
				stream << "Content-Length: " << body.size() << "\r\n\r\n";
				
				stream.flush();
				
				write(stream.str());
				
				if (body.size() > 0)
					write(body);
			}
			
			void HTTP1::write_response(const std::uint32_t & status, const std::map<std::string, std::string> & headers, const std::string & body)
			{
				std::stringstream stream;
				
				stream << "HTTP/1.1 " << status << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << "\r\n";
				}
				
				stream << "Content-Length: " << body.size() << "\r\n\r\n";
				
				stream.flush();
				
				write(stream.str());
				
				if (body.size() > 0)
					write(body);
			}
		}
	}
}
