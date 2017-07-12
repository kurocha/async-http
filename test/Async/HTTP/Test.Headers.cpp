//
//  Test.Headers.cpp
//  This file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 12/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Async/HTTP/Headers.hpp>

namespace Async
{
	namespace HTTP
	{
		UnitTest::Suite HeadersTestSuite {
			"Async::HTTP::Headers",
			
			{"it can compare header keys",
				[](UnitTest::Examiner & examiner) {
					HeaderKeyEqual equal;
					
					examiner.check(equal("abc", "abC"));
					examiner.check(equal("Abc", "abc"));
					
					examiner.check(!equal("", "ABC"));
					examiner.check(!equal("ABC", ""));
					
					examiner.check(equal("ABC", "abc"));
					examiner.check(equal("abc", "abc"));
					examiner.check(equal("abc", "ABC"));
					
					examiner.check(!equal("Content-Length", "Content_Length"));
				}
			},
			
			{"it can set and get case-sensitive headers",
				[](UnitTest::Examiner & examiner) {
					Headers headers;
					
					headers["Content-Length"] = "10";
					
					examiner.expect(headers["Content-Length"]) == "10";
					examiner.expect(headers["content-length"]) == "10";
				}
			},
		};
	}
}
