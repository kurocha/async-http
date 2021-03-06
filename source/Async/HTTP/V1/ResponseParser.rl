//
//  ResponseParser.cpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <Async/HTTP/V1/ResponseParser.hpp>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			%%{
				machine ResponseParser;
				
				include Parser "Parser.rl";
				
				main := http_response;
				
				access this->_;
				
				write data;
			}%%
			
			ResponseParser::ResponseParser(Response & response) : Parser<Response>(response)
			{
				%%{
					write init;
				}%%
			}
			
			ResponseParser::~ResponseParser()
			{
			}
			
			std::size_t ResponseParser::parse(const Byte * begin, const Byte * end)
			{
				auto p = begin;
				auto pe = end;
				
				const Byte * eof = end;
				const Byte * mark = _marked ? p : nullptr;
				
				%%{
					write exec;
				}%%
				
				if (_marked) {
					_split.append(mark, pe);
				}
				
				return p-begin;
			}
		}
	}
}
