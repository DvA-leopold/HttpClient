#include "HTTPClient.h"

HTTPClient::HTTPClient()
        : connectionManager(5*1000)
{ }

std::shared_ptr<IHttpResponse> HTTPClient::execute(HttpMethod& executeMethod, int secondsWaitForResponse)
{
	std::shared_ptr<IHttpResponse> response = executeMethod.generateResponseEntity(secondsWaitForResponse);
	connectionManager.Connect(executeMethod.getHost(), response);
	connectionManager.Send(executeMethod.getHost(), executeMethod.generateHTTPRequest());
	return response;
}

void HTTPClient::CloseClient()
{
	connectionManager.DisconnectAll();
}
