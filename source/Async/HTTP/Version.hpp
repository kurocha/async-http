//
//  Version.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 12/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <iosfwd>

namespace Async
{
	namespace HTTP
	{
		enum class Version
		{
			HTTP_10, HTTP_11, HTTP_2
		};
		
		const char * version_string(Version);
		
		bool default_keep_alive(Version);
		
		bool chunked_transfer_encoding(Version);
		
		std::ostream & operator<<(std::ostream & out, Version version);
	}
}
