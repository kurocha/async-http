//
//  Client.hpp
//  File file is part of the "Async HTTP" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Async/Reactor.hpp>
#include <Async/Network/Endpoint.hpp>

#include "Request.hpp"
#include "Response.hpp"

#include <Concurrent/Fiber.hpp>
#include <Memory/Shared.hpp>

#include "V1/Protocol.hpp"

namespace Async
{
	namespace HTTP
	{
		class Client
		{
		public:
			Client(const Network::Endpoints & endpoints, Reactor & reactor);
			virtual ~Client();
			
			bool send(const Request & request, Response & response);
			
		private:
			Network::Socket _socket;
			
			// Connect on demand, prefer keep-alive if possible.
			Memory::Shared<V1::Protocol> _protocol;
		};
	}
}
