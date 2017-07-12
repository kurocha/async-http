//
//  Test.RequestParser.cpp
//  This file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Async/HTTP/V1/RequestParser.hpp>

#include <iterator>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			UnitTest::Suite RequestParserTestSuite {
				"Async::HTTP::V1::RequestParser",
				
				{"it can parse request line",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "GET /foobar HTTP/1.1\r\n";
						
						Request request;
						RequestParser request_parser(request);
						request_parser.parse(std::begin(buffer), std::end(buffer));
						
						examiner.expect(request_parser.complete()) == false;
						
						examiner.expect(request.method) == "GET";
						examiner.expect(request.target) == "/foobar";
						examiner.expect(request.version) == "HTTP/1.1";
					}
				},
				
				{"it can parse request line in partial chunks",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "GET /foobar HTTP/1.1\r\n";
						
						Request request;
						RequestParser request_parser(request);
						
						for (auto current = std::begin(buffer); (current+1) < std::end(buffer); current += 1) {
							request_parser.parse(current, current+1);
						}
						
						examiner.expect(request_parser.complete()) == false;
						
						examiner.expect(request.method) == "GET";
						examiner.expect(request.target) == "/foobar";
						examiner.expect(request.version) == "HTTP/1.1";
					}
				},
				
				{"it can parse request with headers",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "POST /foobar HTTP/1.1\r\nContent-Length: 0\r\n\r\n";
						
						Request request;
						RequestParser request_parser(request);
						request_parser.parse(std::begin(buffer), std::end(buffer));
						examiner.expect(request_parser.complete()) == true;
						
						examiner.expect(request.method) == "POST";
						examiner.expect(request.target) == "/foobar";
						examiner.expect(request.version) == "HTTP/1.1";
						
						examiner.expect(request.headers["Content-Length"]) == "0";
					}
				},
				
				{"it can parse request with content body",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "POST /foobar HTTP/1.1\r\nContent-Length: 10\r\n\r\n0123456789";
						
						Request request;
						RequestParser request_parser(request);
						request_parser.parse(std::begin(buffer), std::end(buffer));
						examiner.expect(request_parser.complete()) == true;
						
						examiner.expect(request.method) == "POST";
						examiner.expect(request.target) == "/foobar";
						examiner.expect(request.version) == "HTTP/1.1";
						
						examiner.expect(request.headers["Content-Length"]) == "10";
						examiner.expect(request.body) == "0123456789";
					}
				},
				
				{"it can parse request with chunked body",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "POST /foobar HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n0123\r\n6\r\n456789\r\n0\r\n\r\n";
						
						Request request;
						RequestParser request_parser(request);
						request_parser.parse(std::begin(buffer), std::end(buffer));
						examiner.expect(request_parser.complete()) == true;
						
						examiner.expect(request.method) == "POST";
						examiner.expect(request.target) == "/foobar";
						examiner.expect(request.version) == "HTTP/1.1";
						
						examiner.expect(request.body) == "0123456789";
					}
				},
			};
		}
	}
}
