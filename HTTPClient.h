#ifndef HTTPCLIENT_HTTPCLIENT_H
#define HTTPCLIENT_HTTPCLIENT_H

#include "response_handlers/HttpGetResponse.h"
#include "transport/TCPTransportManager.h"
#include "http_methods/HttpMethod.h"

class HTTPClient
{
public:
    HTTPClient();
	std::shared_ptr<IHttpResponse> execute(HttpMethod& executeMethod, int secondsWaitForResponse);

private:
    TCPTransportManager connectionManager_;
};


#endif //HTTPCLIENT_HTTPCLIENT_H
