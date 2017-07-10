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
#include <Async/Protocol.hpp>

namespace Async
{
	namespace HTTP
	{
		UnitTest::Suite ServerTestSuite {
			"Dream::HTTP::Server",
			
			{"it should have some real tests",
				[](UnitTest::Examiner & examiner) {
					Reactor reactor;
					
					// Addresses for the server.
					auto endpoints = Network::Endpoint::service_endpoints(8080, SOCK_STREAM);
					
					//server.resume();
				}
			},
		};
	}
}
