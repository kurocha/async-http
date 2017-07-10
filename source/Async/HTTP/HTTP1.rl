
%%{
	# An HTTP request grammar. Can parse both http_request and http_response.
	# Based on https://tools.ietf.org/html/rfc7230
	
	machine HTTP1;
	
	alphtype unsigned char;
	
	# Visible, printable characters.
	http_vchar = print - ' ';
	
	http_tchar = ("!" | "#" | "$" | "%" | "&" | "'" | "*" | "+" | "-" | "." | "^" | "_" | "`" | "|" | "~" | [0-9] | [a-zA-Z]);
	http_token = http_tchar+;
	
	http_crlf = "\r\n";
	
	http_method = http_token >mark %http_method;
	http_version = ("HTTP/1." [01]) >mark %http_version;
	http_status = ([0-9]{3}) >mark %http_status;
	http_target = http_vchar+ >mark %http_target;
	http_reason = http_vchar+ >mark %http_reason;
	
	http_header_name = http_token >mark %http_header_name;
	http_header_value = http_token >mark %http_header_value;
	http_header = http_header_name ':' space* http_header_value http_crlf;
	
	# The first line of the request.
	http_request_line = http_method space http_target space http_version http_crlf;
	
	# The first line of the response.
	http_status_line = http_version space http_status (space http_reason)? http_crlf;
	
	# The two main entry points.
	http_request = http_request_line (http_header)* http_crlf @http_body;
	http_response = http_status_line (http_header)* http_crlf @http_body;
	
	# Reads a fixed number of bytes of content.
	http_content_body := any >http_read_content;
	
	# Reads a chunked transfer encoding body.
	http_chunked_body :=
		(
			([0-9a-fA-F]+) >mark http_crlf %http_chunk_size
			any >http_read_content http_crlf
		)+;
}%%
