//
//  Response.hpp
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
		struct Response
		{
			std::string version;
			uint32_t status;
			std::string reason;
			
			std::map<std::string, std::string> headers;
			
			std::string body;
			
			bool is_continue() {return status == 100;}
			bool is_success() {return status >= 200 && status < 300;}
			bool is_redirection() {return status >= 300 && status < 400;}
			bool is_failure() {return status >= 400 && status < 600;}
		};
	}
}
