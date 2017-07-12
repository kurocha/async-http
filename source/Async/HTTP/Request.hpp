//
//  Request.hpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 21/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <string>

#include "Headers.hpp"
#include "Version.hpp"

namespace Async
{
	namespace HTTP
	{
		struct Request
		{
			std::string method, target;
			
			Version version;
			
			Headers headers;
			
			std::string body;
			
			bool should_keep_alive() const {
				// keep-alive defaults to true for anything other than HTTP_10:
				return keep_alive(headers, default_keep_alive(version));
			}
		};
	}
}
