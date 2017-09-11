//
//  Parser.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <string>
#include <cassert>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			typedef unsigned char Byte;
			
			template <typename ObjectT>
			class Parser
			{
			public:
				Parser(ObjectT & object) : _object(object) {}
				virtual ~Parser() {}
				
				bool complete() {return _complete;}
				
				// Derivatives of this class should provide:
				// const Byte * parse(const Byte * begin, const Byte * end);
				
				bool is_chunked_body() {
					auto value = _object.headers.find("Transfer-Encoding");
					
					if (value == _object.headers.end()) {
						return false;
					} else {
						return value->second == "chunked";
					}
				}
				
				std::size_t content_length() {
					auto value = _object.headers.find("Content-Length");
					
					if (value == _object.headers.end()) {
						return 0;
					} else  {
						return std::stoul(value->second, nullptr, 10);
					}
				}
				
			protected:
				ObjectT & _object;
				
				// Whether the parser has completed successfully or not.
				bool _complete = false;
				
				// The name of the header we are currently parsing.
				std::string _header_name;
				
				// If we only partially parse a field, it will be saved here until the next call to parse.
				std::string _split;
				
				// Whether we are buffering a string from the parser.
				bool _marked = false;
				
				bool _ignore = false;
				std::size_t _marked_length = 0;
				
				// The parser state.
				int _cs;
				
				std::string read_marked(const Byte * begin, const Byte * end) {
					auto string = _split + std::string(begin, end);
					
					_split.clear();
					_marked = false;
					
					if (_ignore) {
						string.resize(_marked_length);
					}
					
					return string;
				}
				
				std::size_t _remaining_body_size = 0;
				
				std::size_t append_body(const Byte * begin, const Byte * end) {
					assert(begin != nullptr);
					
					std::size_t available = end - begin;
					
					std::size_t required = std::min(available, _remaining_body_size);
					
					if (required == 0) return required;
					
					end = begin + required;
					
					_object.body.append(begin, end);
					_remaining_body_size -= (end - begin);
					
					return required;
				}
			};
		}
	}
}
