#ifndef HTTPCLIENT_HTTPMETHOD_H
#define HTTPCLIENT_HTTPMETHOD_H


#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <memory>
#include "../response_handlers/IHttpResponse.h"
#include "../response_handlers/HttpGetResponse.h"
#include "../response_handlers/HttpHeadResponse.h"

enum Methods
{
	GET = 0,
	HEAD
};

class HttpMethod
{
public:
	HttpMethod(const std::string& url, Methods method);

	virtual ~HttpMethod();

	std::shared_ptr<IHttpResponse> generateResponseEntity(int secondsWaitForResponse);

	virtual std::string generateHTTPRequest();

	HttpMethod* addRequestParam(const std::string& key, const std::string& value);

	std::string getHost();

protected:
	std::string m_url;

	std::string uriHost;
	std::string uriPath;
	std::string uriQuery;
	std::string uriFragment;

private:
	Methods method;
	std::vector<std::string> requestParams;
	static const std::vector<std::string> allowedRequestMethods;
};


#endif //HTTPCLIENT_HTTPMETHOD_H
