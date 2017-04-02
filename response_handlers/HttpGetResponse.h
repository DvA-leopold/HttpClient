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
	bool responseCallback(const std::string&& responseChunk) override;

private:
	void parseResponse(const std::string&& response) override;

private:
	std::string localResponse_;
	long long receivedBodySize_ = 0;
	long contentLength_ = 0;
	bool headerTransmited_;
};


#endif //HTTPCLIENT_HTTPREQUEST_H
