//
// Created by operator on 3/26/17.
//

#include "HttpPost.h"

//HttpPost::HttpPost(const std::string& url)
//		: IHttpMethod(url, "POST") { }

std::string HttpPost::generateHTTPRequest()
{
	std::ostringstream stream;
	stream << requestMethod << " " << path << " HTTP/1.1\r\n"
		   << "Host: " << host << "\r\n"
		   << "Connection: close" << "\r\n\r\n";
    return stream.str();
}
