#ifndef HTTPCLIENT_HTTPREQUEST_H
#define HTTPCLIENT_HTTPREQUEST_H


#include <future>
#include <map>
#include <vector>

typedef std::multimap<std::string, std::string>::iterator multimapIter;

class HttpResponse
{
public:
	HttpResponse(std::future<std::string>&& responsePromise);
	HttpResponse(const std::string& response);

public:
	std::string getResponseStatus();
	std::string getResponseHeader();
	std::string getResponseBody();
	std::vector<std::string> getResponseHeaderParam(const std::string &paramKey);

private:
	void parseHeaderResponse(const std::string &responseHeader);
	void parseResponse(const std::string& response);

private:
	std::future<std::string> futureResponse;

private:
	std::string responseStatus;
	std::string responseHeader;
	std::string responseBody;
	std::multimap<std::string, std::string> responseHeaderParams;
};


#endif //HTTPCLIENT_HTTPREQUEST_H
