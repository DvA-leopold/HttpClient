//
// Created by operator on 3/26/17.
//

#ifndef HTTPCLIENT_HTTPPOST_H
#define HTTPCLIENT_HTTPPOST_H


#include "IHttpMethod.h"

class HttpPost : public IHttpMethod // TODO not implemented yet
{
public:
	HttpPost() = delete;
    HttpPost(const std::string& url) = delete;

private:
    std::string generateHTTPRequest() override;

private:

};


#endif //HTTPCLIENT_HTTPPOST_H
