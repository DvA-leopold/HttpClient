#ifndef HTTPCLIENT_HTTPCLIENT_H
#define HTTPCLIENT_HTTPCLIENT_H

#include "http_methods/IHttpMethod.h"
#include "response_handlers/HttpGetResponse.h"
#include "transport/TCPTransportManager.h"

class HTTPClient
{
public:
    HTTPClient();
    void execute(IHttpMethod* executeMethod, IHttpResponse* const httpResponseEntity);

private:
    TCPTransportManager connectionManager;
};


#endif //HTTPCLIENT_HTTPCLIENT_H
