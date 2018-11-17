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
			template <typename BlockT>
			void incremental_parse(Byte * begin, Byte * end, BlockT yield)
			{
				for (auto * middle = begin; middle < end-1; ++middle) {
					yield(begin, middle, end);
				}
			}
			
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
						examiner.expect(request.version) == Version::HTTP_11;
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
						examiner.expect(request.version) == Version::HTTP_11;
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
						examiner.expect(request.version) == Version::HTTP_11;
						
						examiner.expect(request.headers["Content-Length"]) == "0";
					}
				},
				
				{"it can parse request with content body",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "POST /foobar HTTP/1.1\r\nContent-Length: 10\r\n\r\n0123456789";
						
						incremental_parse(std::begin(buffer), std::end(buffer)-1, [&](Byte * begin, Byte * middle, Byte * end){
							Request request;
							RequestParser request_parser(request);
							
							request_parser.parse(begin, middle);
							examiner.expect(request_parser.complete()) == false;
							request_parser.parse(middle, end);
							examiner.expect(request_parser.complete()) == true;
							
							examiner.expect(request.method) == "POST";
							examiner.expect(request.target) == "/foobar";
							examiner.expect(request.version) == Version::HTTP_11;
							
							examiner.expect(request.headers["Content-Length"]) == "10";
							examiner.expect(request.body) == "0123456789";
						});
					}
				},
				
				{"it can parse request with chunked body",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "POST /foobar HTTP/1.1\r\nTransfer-Encoding: chunked\r\n\r\n4\r\n0123\r\n6\r\n456789\r\n0\r\n\r\n";
						
						incremental_parse(std::begin(buffer), std::end(buffer)-1, [&](Byte * begin, Byte * middle, Byte * end){
							Request request;
							RequestParser request_parser(request);
							
							request_parser.parse(begin, middle);
							examiner.expect(request_parser.complete()) == false;
							request_parser.parse(middle, end);
							examiner.expect(request_parser.complete()) == true;
							
							examiner << "Transfer encoding is chunked." << std::endl;
							examiner.check(request_parser.is_chunked_body());
							examiner.expect(request.method) == "POST";
							examiner.expect(request.target) == "/foobar";
							examiner.expect(request.version) == Version::HTTP_11;
							
							examiner << "Split at " << (void*)middle << std::endl;
							examiner.expect(request.body) == "0123456789";
						});
					}
				},
				
				{"it ignores trailing space on headers",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] = "GET /test HTTP/1.1\r\nAccept: */*   \r\n\r\n";
						
						incremental_parse(std::begin(buffer), std::end(buffer)-1, [&](Byte * begin, Byte * middle, Byte * end){
							Request request;
							RequestParser request_parser(request);
							
							request_parser.parse(begin, middle);
							examiner.expect(request_parser.complete()) == false;
							request_parser.parse(middle, end);
							examiner.expect(request_parser.complete()) == true;
							
							examiner.expect(request.method) == "GET";
							examiner.expect(request.target) == "/test";
							examiner.expect(request.version) == Version::HTTP_11;
							
							examiner.expect(request.headers["Accept"]) == "*/*";
						});
					}
				},
				
				{"it can parse sample request from web browser",
					[](UnitTest::Examiner & examiner) {
						unsigned char buffer[] =
							"GET /render HTTP/1.1\r\n"
							"Host: hana.local:8080\r\n"
							"Connection: keep-alive\r\n"
							"Cache-Control: max-age=0\r\n"
							"Upgrade-Insecure-Requests: 1\r\n"
							"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_6) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/60.0.3112.113 Safari/537.36\r\n"
							"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,image/apng,*/*;q=0.8\r\n"
							"Accept-Encoding: gzip, deflate\r\n"
							"Accept-Language: en-GB,en-US;q=0.8,en;q=0.6\r\n\r\n";

						Request request;
						RequestParser request_parser(request);

						request_parser.parse(std::begin(buffer), std::end(buffer)-1);
						examiner.expect(request_parser.complete()) == true;

						examiner.expect(request.method) == "GET";
						examiner.expect(request.target) == "/render";
						examiner.expect(request.version) == Version::HTTP_11;
					}
				},
			};
		}
	}
}
