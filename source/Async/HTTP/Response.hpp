//
//  Response.hpp
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
		struct Response
		{
			Version version;
			std::uint16_t status;
			std::string reason;
			
			Headers headers;
			
			std::string body;
			
			bool is_continue() const noexcept {return status == 100;}
			bool is_success() const noexcept {return status >= 200 && status < 300;}
			bool is_redirection() const noexcept {return status >= 300 && status < 400;}
			bool is_failure() const noexcept {return status >= 400 && status < 600;}
			
			bool should_keep_alive() const {
				// keep-alive defaults to true for anything other than HTTP_10:
				return keep_alive(headers, default_keep_alive(version));
			}
		};
	}
}
