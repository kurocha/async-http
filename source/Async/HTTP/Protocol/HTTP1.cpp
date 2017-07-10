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

namespace Async
{
	namespace HTTP
	{
		namespace Protocol
		{
			HTTP1::HTTP1(Network::Socket & socket, Reactor & reactor) : StreamProtocol(socket, reactor), _buffer(1024*8, true)
			{
			}
			
			HTTP1::~HTTP1()
			{
			}
			
			void HTTP1::fill_buffer()
			{
				if (_buffer.size() == 0) {
					auto mark = read(_buffer.end(), _buffer.top());
					
					_buffer.expand(mark - _buffer.end());
				}
			}
			
			Request HTTP1::read_request()
			{
				Request request;
				
				auto parser = RequestParser(request);
				
				while (!parser.complete()) {
					fill_buffer();
					
					auto mark = parser.parse(_buffer.begin(), _buffer.end());
					
					if (mark != _buffer.end()) {
						// uh oh.
					} else {
						_buffer.resize(0);
					}
				}
				
				return request;
			}
			
			Response HTTP1::read_response()
			{
				Response response;
				
				auto parser = ResponseParser(response);
				
				while (!parser.complete()) {
					fill_buffer();
					
					auto mark = parser.parse(_buffer.begin(), _buffer.end());
					
					if (mark != _buffer.end()) {
						// uh oh.
					} else {
						_buffer.resize(0);
					}
				}
				
				return response;
			}
			
			void HTTP1::write_request(std::string method, std::string target, std::string version, std::map<std::string, std::string> headers, std::string body)
			{
				std::stringstream stream;
				
				stream << method << ' ' << target << ' ' << version << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << "\r\n";
				}
				
				if (body.size() > 0) {
					stream << "Content-Length: " << body.size() << "\r\n\r\n";
				}
				
				stream.flush();
				
				write(stream.str());
				
				if (body.size() > 0)
					write(body);
			}
			
			void HTTP1::write_response(std::uint32_t status, std::map<std::string, std::string> headers, std::string body)
			{
				std::stringstream stream;
				
				stream << "HTTP/1.1 " << status << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << std::endl;
				}
				
				if (body.size() > 0) {
					stream << "Content-Length: " << body.size() << "\r\n\r\n";
				}
				
				stream.flush();
				
				write(stream.str());
				
				if (body.size() > 0)
					write(body);
			}
		}
	}
}
