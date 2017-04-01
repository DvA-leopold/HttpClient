#ifndef HTTPCLIENT_HTTPHEADRESPONSE_H
#define HTTPCLIENT_HTTPHEADRESPONSE_H


#include "IHttpResponse.h"
#include <iostream>

class HttpHeadResponse
		: public IHttpResponse
{
public:
	HttpHeadResponse(int secondsWaitForResponse);
	void responseCallback(const std::string&& responseChunk) override;

protected:
	void parseResponse(const std::string&& response) override;

private:
	std::string localResponse;
};


#endif //HTTPCLIENT_HTTPHEADRESPONSE_H
