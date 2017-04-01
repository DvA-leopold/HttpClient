#ifndef HTTPCLIENT_HTTPREQUEST_H
#define HTTPCLIENT_HTTPREQUEST_H


#include "IHttpResponse.h"
#include <iostream>
#include <fstream>

class HttpGetResponse
		: public IHttpResponse
{
public:
	HttpGetResponse(int secondsWaitForResponse);
	~HttpGetResponse();
	void responseCallback(const std::string&& responseChunk) override;

private:
	void parseResponse(const std::string&& response) override;

private:
	std::string localResponse;
	long long receivedBodySize = 0;
	long contentLength = 0;
	bool headerTransmited;
};


#endif //HTTPCLIENT_HTTPREQUEST_H
