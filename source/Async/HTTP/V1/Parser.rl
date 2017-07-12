
%%{
	# An HTTP request grammar. Can parse both http_request and http_response.
	# Based on https://tools.ietf.org/html/rfc7230
	
	machine Parser;
	alphtype unsigned char;
	
	# Generic actions:
	
	action mark {
		mark = p;
		_marked = true;
	}

	action http_method {
		_object.method = read_marked(mark, p);
	}

	action http_status {
		_object.status = std::stoul(read_marked(mark, p));
	}

	action http_reason {
		// ignore
		mark = nullptr;
	}

	action http_target {
		_object.target = read_marked(mark, p);
	}

	action http_version {
		_object.version = read_marked(mark, p);
	}

	action http_header_name {
		_header_name = read_marked(mark, p);
	}

	action http_header_value {
		_object.headers[_header_name] = read_marked(mark, p);
	}

	action http_body {
		if (is_chunked_body()) {
			fnext http_chunked_body;
		} else {
			_remaining_body_size = content_length();
			
			if (_remaining_body_size > 0) {
				fnext http_content_body;
			} else {
				_complete = true;
				fbreak;
			}
		}
	}

	action http_read_content {
		// We may need to read data.
		if (_remaining_body_size > 0) {
			// TODO verify no chance of overflow.
			p += append_body(p, pe) - 1;
			// p will be incremented, so we point it at the last consumed character.
		}
		
		// We may need more data:
		if (_remaining_body_size > 0) {
			// Stay in current state.
			fnext *fcurs;
		} else {
			// Advance to next state.
		}
	}

	action http_chunk_size {
		auto hex_size = read_marked(mark, p);
		_remaining_body_size = std::stoul(hex_size, nullptr, 16);
		
		if (_remaining_body_size == 0) {
		}
	}

	action http_complete {
		_complete = true;
	}
	
	include RFC7230 "RFC7230.rl";
}%%