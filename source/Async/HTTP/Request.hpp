//
//  Request.hpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 21/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <string>
#include <map>

namespace Async
{
	namespace HTTP
	{
		struct Request
		{
			std::string method, target, version;
			std::map<std::string, std::string> headers;
			
			std::string body;
		};
	}
}
