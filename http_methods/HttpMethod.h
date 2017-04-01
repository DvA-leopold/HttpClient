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
	std::string url_;

	std::string uriHost_;
	std::string uriPath_;
	std::string uriQuery_;
	std::string uriFragment_;

private:
	Methods method_;
	std::vector<std::string> requestParams_;
	static const std::vector<std::string> allowedRequestMethods_;
};


#endif //HTTPCLIENT_HTTPMETHOD_H
