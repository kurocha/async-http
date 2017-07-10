//
//  Server.cpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 21/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Server.hpp"

#include <Logger/Console.hpp>

#include "Protocol/HTTP1.hpp"

namespace Async
{
	namespace HTTP
	{
		using namespace Memory;
		using namespace Concurrent;
		using namespace Logger;
		
		Server::Server()
		{
		}
		
		Server::~Server()
		{
		}
		
		Shared<Fiber> Server::run(const Network::Endpoints & endpoints, Reactor & reactor)
		{
			auto server = shared<Fiber>([&]{
				Fiber::Pool bindings;
				
				for (auto & endpoint : endpoints) {
					bindings.resume([&]{
						Fiber::Pool connections;
						
						auto socket = endpoint.bind();
						
						socket.listen();
						
						Console::info("Server", (Descriptor)socket, "listening to", endpoint.address());
						
						while (true) {
							connections.resume([&, client = socket.accept(reactor)]() mutable {
								Protocol::HTTP1 protocol(client, reactor);
								
								while (true) {
									auto request = protocol.read_request();
									
									// Generate a response:
									auto response = this->process(request);
									
									// Send the response back:
									protocol.write_response(response);
								}
							});
						}
					});
				}
				
				Fiber::current->yield();
			});
			
			return server;
		}
		
		Response Server::process(const Request & request)
		{
			return {
				request.version, 200, "",
				{
					{"Content-Type", "text/plain"}
				},
				""
			};
		}
	}
}
