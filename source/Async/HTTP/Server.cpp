//
//  Server.cpp
//  File file is part of the "Async" project and released under the MIT License.
//
//  Created by Samuel Williams on 21/6/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Server.hpp"

#include <Logger/Console.hpp>

#include "V1/Protocol.hpp"

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
						
						Console::info("Server", socket, "listening to", endpoint.address());
						
						while (true) {
							connections.resume([&, client = socket.accept(reactor)]() mutable {
								Console::info("Client", client, "connected from", client.remote_address());
								V1::Protocol protocol(client, reactor);
								
								Request request;
								while (protocol.read_request(request)) {
									// Generate a response:
									Console::debug("Client", client, "requested", request.target);
									auto response = this->process(request);
									
									// Send the response back:
									Console::debug("Sending response", response.status, "to client", client);
									protocol.write_response(response);
									
									client.shutdown();
								}
								
								Console::debug("Client", client, "finished");
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
					{"Content-Type", "text/plain"},
				},
				""
			};
		}
	}
}
