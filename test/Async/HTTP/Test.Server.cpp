//
//  Test.Server.cpp
//  This file is part of the "Dream" project and released under the MIT License.
//
//  Created by Samuel Williams on 27/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include <UnitTest/UnitTest.hpp>

#include <Concurrent/Fiber.hpp>
#include <Async/Network/Endpoint.hpp>
#include <Async/Network/Socket.hpp>
#include <Async/Reactor.hpp>

#include <Async/HTTP/Server.hpp>
#include <Async/HTTP/V1/Protocol.hpp>

namespace Async
{
	namespace HTTP
	{
		UnitTest::Suite ServerTestSuite {
			"Async::HTTP::Server",
			
			{"it can request resource",
				[](UnitTest::Examiner & examiner) {
					Reactor reactor;
					
					// Addresses for the server.
					auto endpoints = Network::Endpoint::service_endpoints(8080, SOCK_STREAM);
					
					auto server = Server();
					
					auto task = server.run(endpoints, reactor);
					
					task->resume();
					
					Concurrent::Fiber client([&]{
						Request request = {
							"GET", "/hello-world", Version::HTTP_11,
							{{"Accept", "*/*"}},
							""
						};
						
						for (auto & endpoint : endpoints) {
							auto socket = endpoint.connect(reactor);
							
							V1::Protocol protocol(socket, reactor);
							
							protocol.write_request(request);
							
							Response response;
							
							examiner.expect(protocol.read_response(response)) == true;
							examiner.expect(response.status) == 404;
						}
					});
					
					client.resume();
					
					reactor.wait(1.0);
				}
			},
		};
	}
}
