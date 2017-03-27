//
// Created by operator on 3/25/17.
//

#ifndef HTTPCLIENT_HTTPCLIENT_H
#define HTTPCLIENT_HTTPCLIENT_H

#include <string>
#include "transport/ClientConnectionPool.h"
#include "http_methods/IHttpMethod.h"
#include "HttpResponse.h"

class HTTPClient
{
public:
    HTTPClient(bool async = false);

    HttpResponse execute(IHttpMethod* executeMethod);

private:
    const bool async;
    ClientConnectionPool connectionPool;
};


#endif //HTTPCLIENT_HTTPCLIENT_H
