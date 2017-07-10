//
//  ResponseParser.cpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <Async/HTTP/ResponseParser.hpp>

namespace Async
{
	namespace HTTP
	{
		%%{
			machine ResponseParser;
			
			action mark {
				mark = p;
				_marked = true;
			}
			
			action http_method {
				_response.method = read_marked(mark, p);
			}
			
			action http_status {
				_response.status = std::stoul(read_marked(mark, p));
			}
			
			action http_reason {
				// ignore
				mark = nullptr;
			}
			
			action http_target {
				_response.target = read_marked(mark, p);
			}
			
			action http_version {
				_response.version = read_marked(mark, p);
			}
			
			action http_header_name {
				_header_name = read_marked(mark, p);
			}
			
			action http_header_value {
				_response.headers[_header_name] = read_marked(mark, p);
			}
			
			action http_body {
				if (is_chunked_body()) {
					fnext http_chunked_body;
				} else {
					_remaining_body_size = content_length();
					fnext http_content_body;
				}
			}
			
			action http_read_content {
				// We may need to read data.
				if (_remaining_body_size > 0) {
					// TODO verify no chance of overflow.
					p += append_body(p, pe) - 1;
					// p will be incremented, so we point it at the last consumed character.
				}
				
				// We may need more data:
				if (_remaining_body_size > 0) {
					// Stay in current state.
					fnext *fcurs;
				} else {
					// Advance to next state.
				}
			}
			
			action http_chunk_size {
				auto hex_size = read_marked(mark, p);
				_remaining_body_size = std::stoul(hex_size, nullptr, 16);
				
				if (_remaining_body_size == 0) {
				}
			}
			
			action http_complete {
				_complete = true;
			}
			
			include HTTP1 "HTTP1.rl";
			
			main := http_response;
			
			access this->_;
			
			write data;
		}%%
		
		ResponseParser::ResponseParser(Response & response) : _response(response)
		{
			%%{
				write init;
			}%%
		}
		
		ResponseParser::~ResponseParser()
		{
		}
		
		const Byte * ResponseParser::parse(const Byte * p, const Byte * pe) {
			const Byte * eof = pe;
			const Byte * mark = _marked ? p : nullptr;
			
			%%{
				write exec;
			}%%
			
			if (_marked) {
				_split.append(mark, pe);
			}
			
			return p;
		}
	}
}
