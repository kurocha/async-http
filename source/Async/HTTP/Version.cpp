//
//  Version.cpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 12/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Version.hpp"

#include <iostream>

namespace Async
{
	namespace HTTP
	{
		const char * version_string(Version version)
		{
			switch (version) {
				case Version::HTTP_10: return "HTTP/1.0";
				case Version::HTTP_11: return "HTTP/1.1";
				case Version::HTTP_20: return "HTTP/2.0";
			}
		}
		
		bool default_keep_alive(Version version)
		{
			switch (version) {
				case Version::HTTP_10: return false;
				case Version::HTTP_11:
				case Version::HTTP_20: return true;
			}
		}
		
		bool chunked_transfer_encoding(Version version)
		{
			switch (version) {
				case Version::HTTP_10: return false;
				case Version::HTTP_11:
				case Version::HTTP_20: return true;
			}
		}
		
		std::ostream & operator<<(std::ostream & out, Version version)
		{
			return out << version_string(version);
		}
	}
}
