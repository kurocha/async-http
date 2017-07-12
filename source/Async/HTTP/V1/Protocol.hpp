//
//  Protocol.hpp
//  File file is part of the "Async Http" project and released under the MIT License.
//
//  Created by Samuel Williams on 11/7/2017.
//  Copyright, 2017, by Samuel Williams. All rights reserved.
//

#pragma once

#include <Async/Network/Socket.hpp>
#include <Async/Protocol/Stream.hpp>
#include <Async/Protocol/Buffer.hpp>

#include "../Request.hpp"
#include "../Response.hpp"

namespace Async
{
	namespace HTTP
	{
		namespace V1
		{
			using namespace Async::Protocol;
			
			class Protocol : protected Stream
			{
			public:
				Protocol(Network::Socket & socket, Reactor & reactor);
				virtual ~Protocol();
				
				bool read_request(Request & request);
				bool read_response(Response & response);
				
				void write_response(const Response & response);
				void write_request(const Request & request);
				
				void write_request(const std::string & method, const std::string & target, const Version & version, const Headers & headers, const std::string & body);
				void write_response(const std::uint32_t & status, const Version & version, const Headers & headers, const std::string & body);
				
			protected:
				Buffer _buffer;
				
				template <typename ParserT>
				bool read_into_parser(ParserT & parser);
			};
		}
	}
}
