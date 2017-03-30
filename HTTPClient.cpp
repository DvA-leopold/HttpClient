#include "HTTPClient.h"

HTTPClient::HTTPClient()
        : connectionManager(true)
{ }

void HTTPClient::execute(IHttpMethod* executeMethod, IHttpResponse* const httpResponseEntity)
{
	try
	{
		connectionManager.Connect(executeMethod->getHost(), httpResponseEntity);
		connectionManager.Send(executeMethod->getHost(), executeMethod->generateHTTPRequest());
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}