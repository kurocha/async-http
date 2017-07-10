//
//  Test.RequestParser.cpp
//  This file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 22/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>
#include <Buffers/StaticBuffer.hpp>

#include <Async/HTTP/RequestParser.hpp>

namespace Async
{
	namespace HTTP
	{
		UnitTest::Suite RequestParserTestSuite {
			"Dream::HTTP::RequestParser",
			
			{"it can parse request line",
				[](UnitTest::Examiner & examiner) {
					Buffers::StaticBuffer buffer("GET /foobar HTTP/1.1\r\n");
					
					Request request;
					RequestParser request_parser(request);
					request_parser.parse(buffer.begin(), buffer.end());
					
					examiner.expect(request.method) == "GET";
					examiner.expect(request.target) == "/foobar";
					examiner.expect(request.version) == "HTTP/1.1";
				}
			},
			
			{"it can parse request line in partial chunks",
				[](UnitTest::Examiner & examiner) {
					Buffers::StaticBuffer buffer("GET /foobar HTTP/1.1\r\n");
					
					Request request;
					RequestParser request_parser(request);
					
					for (auto current = buffer.begin(); (current+1) < buffer.end(); current += 1) {
						request_parser.parse(current, current+1);
					}
					
					examiner.expect(request.method) == "GET";
					examiner.expect(request.target) == "/foobar";
					examiner.expect(request.version) == "HTTP/1.1";
				}
			},
			
			{"it can parse request with headers",
				[](UnitTest::Examiner & examiner) {
					Buffers::StaticBuffer buffer("POST /foobar HTTP/1.1\r\nContent-Length: 0\r\n\r\n");
					
					Request request;
					RequestParser request_parser(request);
					request_parser.parse(buffer.begin(), buffer.end());
					
					examiner.expect(request.method) == "POST";
					examiner.expect(request.target) == "/foobar";
					examiner.expect(request.version) == "HTTP/1.1";
					
					examiner.expect(request.headers["Content-Length"]) == "0";
				}
			},
			
			{"it can parse request with content body",
				[](UnitTest::Examiner & examiner) {
					Buffers::StaticBuffer buffer("POST /foobar HTTP/1.1\r\nContent-Length: 10\r\n\r\n0123456789");
					
					Request request;
					RequestParser request_parser(request);
					request_parser.parse(buffer.begin(), buffer.end());
					
					examiner.expect(request.method) == "POST";
					examiner.expect(request.target) == "/foobar";
					examiner.expect(request.version) == "HTTP/1.1";
					
					examiner.expect(request.headers["Content-Length"]) == "10";
					examiner.expect(request.body) == "0123456789";
				}
			},
			
			{"it can parse request with chunked body",
				[](UnitTest::Examiner & examiner) {
					Buffers::StaticBuffer buffer("POST /foobar HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n0123\r\n6\r\n456789\r\n0\r\n\r\n");
					
					Request request;
					RequestParser request_parser(request);
					request_parser.parse(buffer.begin(), buffer.end());
					
					examiner.expect(request.method) == "POST";
					examiner.expect(request.target) == "/foobar";
					examiner.expect(request.version) == "HTTP/1.1";
					
					examiner.expect(request.body) == "0123456789";
				}
			}
		};
	}
}