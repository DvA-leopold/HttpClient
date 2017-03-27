#ifndef HTTPCLIENT_HTTPHEAD_H
#define HTTPCLIENT_HTTPHEAD_H


#include "IHttpMethod.h"

class HttpHead : public IHttpMethod
{
public:
    HttpHead(const std::string &url);

    std::string generateHTTPRequest() override;

private:
};


#endif //HTTPCLIENT_HTTPHEAD_H
