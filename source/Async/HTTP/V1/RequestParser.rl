//
//  RequestParser.cpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <Async/HTTP/V1/RequestParser.hpp>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			%%{
				machine RequestParser;
				
				include Parser "Parser.rl";
				
				main := http_request;
				
				access this->_;
				
				write data;
			}%%
			
			RequestParser::RequestParser(Request & request) : Parser<Request>(request)
			{
				%%{
					write init;
				}%%
			}
			
			RequestParser::~RequestParser()
			{
			}
			
			const Byte * RequestParser::parse(const Byte * p, const Byte * pe) {
				const Byte * eof = nullptr;
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
}
