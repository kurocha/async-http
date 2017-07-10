//
//  RequestParser.hpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Buffers/Buffer.hpp>
#include <string>

#include "Request.hpp"

namespace Async
{
	namespace HTTP
	{
		using namespace Buffers;
		
		class RequestParser
		{
		public:
			typedef unsigned char Byte;
			
			RequestParser(Request & request);
			~RequestParser();
			
			const Byte * parse(const Byte * begin, const Byte * end);
			
			bool complete() {return _complete;}
			
		private:
			bool _complete = false;
			Request & _request;
			
			std::string read_marked(const Byte * begin, const Byte * end) {
				auto string = _split + std::string(begin, end);
				
				_split.clear();
				_marked = false;
				
				return string;
			}
			
			bool is_chunked_body() {
				return _request.headers["Transfer-Encoding"] == "chunked";
			}
			
			std::size_t content_length() {
				// TODO This needs to be more robust.
				return std::stoul(_request.headers["Content-Length"], nullptr, 10);
			}
			
			std::size_t _remaining_body_size = 0;
			
			std::size_t append_body(const Byte * begin, const Byte * end) {
				std::size_t available = end - begin;
				
				std::size_t required = std::min(available, _remaining_body_size);
				
				if (required == 0) return required;
				
				end = begin + required;
				
				_request.body.append(begin, end);
				_remaining_body_size -= (end - begin);
				
				return required;
			}
			
			// The name of the header we are currently parsing.
			std::string _header_name;
			
			// If we only partially parse a field, it will be saved here until the next call to parse.
			std::string _split;
			
			// Whether we are buffering a string from the parser.
			bool _marked = false;
			
			// The parser state.
			int _cs;
		};
	}
}
