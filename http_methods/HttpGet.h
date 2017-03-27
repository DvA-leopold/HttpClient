#ifndef HTTPCLIENT_HTTPGET_H
#define HTTPCLIENT_HTTPGET_H


#include "IHttpMethod.h"

class HttpGet : public IHttpMethod
{
public:
    HttpGet(const std::string &url);

    std::string generateHTTPRequest() override;
private:
};


#endif //HTTPCLIENT_HTTPGET_H
