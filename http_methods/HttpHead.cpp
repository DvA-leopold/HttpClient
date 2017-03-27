#include "HttpHead.h"

HttpHead::HttpHead(const std::string& url)
        : IHttpMethod(url, "HEAD") { }

std::string HttpHead::generateHTTPRequest()
{
    std::ostringstream stream;
    stream << requestMethod << " " << path << " HTTP/1.1\r\n"
           << "Host: " << host << "\r\n"
           << "Connection: close" << "\r\n\r\n";

    return stream.str();
}
