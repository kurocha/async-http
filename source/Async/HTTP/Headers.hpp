//
//  Headers.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 12/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <string>
#include <unordered_map>

namespace Async
{
	namespace HTTP
	{
		struct HeaderKeyEqual
		{
			bool operator()(const std::string & a, const std::string & b) const;
		};
		
		struct HeaderKeyHash
		{
			std::size_t operator()(const std::string & key) const;
		};
		
		using Headers = std::unordered_map<std::string, std::string, HeaderKeyHash, HeaderKeyEqual>;
		
		bool keep_alive(const Headers & headers, bool default_value = true);
	}
}
