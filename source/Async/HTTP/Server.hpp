//
//  Server.hpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 21/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Async/Reactor.hpp>
#include <Async/Network/Endpoint.hpp>

#include "Request.hpp"
#include "Response.hpp"

#include <Concurrent/Fiber.hpp>
#include <Memory/Shared.hpp>

namespace Async
{
	namespace HTTP
	{
		class Server
		{
		public:
			Server();
			virtual ~Server();
			
			Memory::Shared<Concurrent::Fiber> run(const Network::Endpoints & endpoints, Reactor & reactor);
			
			virtual Response process(const Network::Socket & peer, const Request & request, Reactor & reactor);
		};
	}
}
