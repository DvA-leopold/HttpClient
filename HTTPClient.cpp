#include "HTTPClient.h"

HTTPClient::HTTPClient(bool async)
        : async(async) { }

HttpResponse HTTPClient::execute(IHttpMethod* executeMethod)
{
	std::future<std::string> futureResponse = connectionPool.send(executeMethod->getHost(),
																  executeMethod->generateHTTPRequest());
	HttpResponse response = async ? HttpResponse(std::move(futureResponse)) : HttpResponse(futureResponse.get());
	return response;
}