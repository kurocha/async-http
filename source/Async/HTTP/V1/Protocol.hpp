//
//  Protocol.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Async/Protocol.hpp>
#include <Async/Network/Socket.hpp>

#include "../Request.hpp"
#include "../Response.hpp"

#include <Buffers/DynamicBuffer.hpp>

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			class Protocol : protected Async::StreamProtocol
			{
			public:
				Protocol(Network::Socket & socket, Reactor & reactor);
				virtual ~Protocol();
				
				bool read_request(Request & request);
				bool read_response(Response & response);
				
				void write_response(const Response & response);
				void write_request(const Request & request);
				
				void write_request(const std::string & method, const std::string & target, const std::string & version, const std::map<std::string, std::string> & headers, const std::string & body);
				void write_response(const std::uint32_t & status, const std::map<std::string, std::string> & headers, const std::string & body);
				
			protected:
				Buffers::DynamicBuffer _buffer;
				
				bool fill_buffer();
			};
		}
	}
}
