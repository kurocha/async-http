//
//  Headers.cpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 12/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Headers.hpp"

namespace Async
{
	namespace HTTP
	{
		bool case_insensitive_equal(char a, char b)
		{
			if (a >= 'A' && a <= 'Z') a |= 0b00100000;
			if (b >= 'A' && b <= 'Z') b |= 0b00100000;
			
			return a == b;
		}
		
		bool HeaderKeyEqual::operator()(const std::string & a, const std::string & b) const
		{
			if (a.size() != b.size()) return false;
			
			for (std::size_t i = 0, size = a.size(); i < size; i += 1) {
				if (!case_insensitive_equal(a[i], b[i])) return false;
			}
			
			return true;
		}
		
		std::size_t HeaderKeyHash::operator()(const std::string & key) const
		{
			// computes the hash a lower case string using a variant of the the Fowler-Noll-Vo hash function.
			std::size_t result = 2166136261;
			
			for (auto c : key) {
				// We or with a binary constant to make all characters normalized to lower case.
				result = (result * 16777619) ^ (c | 0b00100000);
			}
			
			return result;
		}
		
		bool keep_alive(const Headers & headers, bool default_value)
		{
			auto connection = headers.find("Connection");
			
			if (connection != headers.end()) {
				if (connection->second == "keep-alive") {
					return true;
				} else if (connection->second == "close") {
					return false;
				}
			}
			
			return default_value;
		}
	}
}
