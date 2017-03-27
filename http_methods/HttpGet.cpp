//
// Created by operator on 3/26/17.
//

#include "HttpGet.h"

HttpGet::HttpGet(const std::string& url)
		: IHttpMethod(url, "GET") {}

std::string HttpGet::generateHTTPRequest()
{
	std::ostringstream stream;
	stream << requestMethod << " " << path << " HTTP/1.1\r\n"
		   << "Host: " << host << "\r\n"
//		   << "Transfer-Encoding: chunked" << "\r\n\r\n";
		   << "Connection: close" << "\r\n\r\n";
	return stream.str();
}
