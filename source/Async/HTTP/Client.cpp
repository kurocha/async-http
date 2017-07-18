//
//  Client.cpp
//  File file is part of the "Async HTTP" project and released under the MIT License.
//
//  Created by Samuel Williams on 18/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#include "Client.hpp"

namespace Async
{
	namespace HTTP
	{
		Client::Client(const Network::Endpoints & endpoints, Reactor & reactor)
		{
			for (auto & endpoint : endpoints) {
				_socket = endpoint.connect(reactor);
				
				// TODO the protocol version should probably be explicitly selected by the nework endpoint.
				_protocol = Memory::shared<V1::Protocol>(_socket, reactor);
				
				break;
			}
		}
		
		Client::~Client()
		{
		}
		
		bool Client::send(const Request & request, Response & response)
		{
			_protocol->write_request(request);
			
			return _protocol->read_response(response);
		}
	}
}
