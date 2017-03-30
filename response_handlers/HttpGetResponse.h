#ifndef HTTPCLIENT_HTTPREQUEST_H
#define HTTPCLIENT_HTTPREQUEST_H


#include "IHttpResponse.h"

class HttpGetResponse
		: public IHttpResponse
{
public:
	HttpGetResponse();
	void responseCallback(std::string&& responseChunk) override;

private:
	void parseResponse(const std::string& response) override;

private:
	std::string localResponse;
	unsigned long expectedContentSize;
	bool headerTransmited;
};


#endif //HTTPCLIENT_HTTPREQUEST_H
