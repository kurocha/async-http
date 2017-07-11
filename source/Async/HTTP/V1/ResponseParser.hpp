//
//  ResponseParser.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include "Parser.hpp"
#include "../Response.hpp"

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			class ResponseParser : public Parser<Response>
			{
			public:
				ResponseParser(Response & request);
				virtual ~ResponseParser();
				
				const Byte * parse(const Byte * begin, const Byte * end);
			};
		}
	}
}
