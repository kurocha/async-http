//
//  Test.Client.cpp
//  This file is part of the "Async HTTP" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Concurrent/Fiber.hpp>
#include <Async/HTTP/Client.hpp>
#include <Async/HTTP/Version.hpp>
#include <URI/Generic.hpp>

#include <Logger/Console.hpp>

namespace Async
{
	namespace HTTP
	{
		UnitTest::Suite ClientTestSuite {
			"Async::HTTP::Client",
			
			{"it can request remote resource",
				[](UnitTest::Examiner & examiner) {
					using namespace Concurrent;
					using namespace Logger;
					
					tzset();
					
					Reactor reactor;
					
					Fiber task([&]{
						auto remote_uri = URI::Generic::parse("http://www.oriontransfer.co.nz/_static/logo.svg");
						
						auto endpoints = Network::Endpoint::named_endpoints(remote_uri.host, remote_uri.port.empty() ? "80" : remote_uri.port, SOCK_STREAM);
						
						Console::info("Connecting to", remote_uri.host);
						auto client = Client(endpoints, reactor);
						
						HTTP::Request request = {"GET", remote_uri.path.value, HTTP::Version::HTTP_11, {
							{"Host", remote_uri.host},
							{"Accept", "image/*"},
							{"Connection", "close"},
						}};
						
						Response response;
						
						Console::info("Sending request", request.target);
						if (client.send(request, response)) {
							Console::info("Got response", response.status, response.body.size());
							examiner.expect(response.status) == 200;
							examiner.expect(response.headers["Content-Type"]) == "image/svg+xml";
							examiner.check(!response.body.empty());
							
							for (const auto & header : response.headers) {
								std::cerr << header.first << ": " << header.second << std::endl;
							}
						}
					});
					
					task.resume();
					
					reactor.wait(5.0);
				}
			},
		};
	}
}
