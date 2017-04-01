#include "HTTPClient.h"

HTTPClient::HTTPClient()
        : connectionManager(5*1000)
{ }

std::shared_ptr<IHttpResponse> HTTPClient::execute(HttpMethod& executeMethod, int secondsWaitForResponse)
{
	std::shared_ptr<IHttpResponse> response = executeMethod.generateResponseEntity(secondsWaitForResponse);
	int socketDescriptor = connectionManager.Connect(executeMethod.getHost(), response);
	connectionManager.Send(socketDescriptor, executeMethod.generateHTTPRequest());
	return response;
}
