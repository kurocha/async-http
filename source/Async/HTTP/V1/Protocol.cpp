//
//  Protocol.cpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Protocol.hpp"

#include "RequestParser.hpp"
#include "ResponseParser.hpp"

#include <sstream>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			Protocol::Protocol(Network::Socket & socket, Reactor & reactor) : Protocol::Stream(socket, reactor), _buffer(1024*8)
			{
			}
			
			Protocol::~Protocol()
			{
			}
			
			template <typename ParserT>
			bool Protocol::read_into_parser(ParserT & parser)
			{
				Readable readable(_descriptor, _reactor);
				
				while (!parser.complete()) {
					auto result = _buffer.read_from(_descriptor, readable);
					
					if (_buffer.empty()) {
						return false;
					}
					
					auto amount = parser.parse(_buffer.begin(), _buffer.end());
					
					if (amount == 0) {
						return false;
					} else {
						_buffer.consume(amount);
					}
				}
				
				return true;
			}
			
			bool Protocol::read_request(Request & request)
			{
				auto parser = RequestParser(request);
				
				return read_into_parser(parser);
			}
			
			bool Protocol::read_response(Response & response)
			{
				Readable readable(_descriptor, _reactor);
				
				auto parser = ResponseParser(response);
				
				return read_into_parser(parser);
			}
			
			void Protocol::write_response(const Response & response)
			{
				write_response(response.status, response.version, response.headers, response.body);
			}
			
			void Protocol::write_request(const Request & request)
			{
				write_request(request.method, request.target, request.version, request.headers, request.body);
			}
			
			void Protocol::write_request(const std::string & method, const std::string & target, const Version & version, const Headers & headers, const std::string & body)
			{
				std::stringstream stream;
				
				stream << method << ' ' << target << ' ' << version << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << "\r\n";
				}
				
				stream << "Content-Length: " << body.size() << "\r\n\r\n";
				
				stream.flush();
				
				write(stream.str());
				
				if (!body.empty())
					write(body);
			}
			
			void Protocol::write_response(const std::uint32_t & status, const Version & version, const Headers & headers, const std::string & body)
			{
				std::stringstream stream;
				
				stream << version << " " << status << "\r\n";
				
				for (auto & header : headers) {
					stream << header.first << ": " << header.second << "\r\n";
				}
				
				stream << "Content-Length: " << body.size() << "\r\n\r\n";
				
				stream.flush();
				
				write(stream.str());
				
				if (!body.empty())
					write(body);
			}

		}
	}
}
