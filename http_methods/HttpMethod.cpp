#include "HttpMethod.h"


const std::vector<std::string> HttpMethod::allowedRequestMethods = {"GET", "HEAD"};

HttpMethod::HttpMethod(const std::string &url, Methods method)
		: m_url(url), uriPath("/"), method(method)
{
	size_t doubleSlash = m_url.find("//");
	if (doubleSlash != std::string::npos)
	{
		m_url = m_url.substr(doubleSlash + 2);
	}
	size_t firstSlash = m_url.find('/');
	size_t lastSlash = m_url.find_last_of('/');
	size_t questionMark = m_url.find('?');
	size_t hashTag = m_url.find('#');

	uriHost = m_url.substr(0, firstSlash);

	if (lastSlash != firstSlash)
	{
		uriPath = m_url.substr(firstSlash, questionMark == std::string::npos ? m_url.size() : questionMark);
	}

	if (questionMark != std::string::npos)
	{
		uriQuery = m_url.substr(questionMark, hashTag == std::string::npos ? m_url.size() : hashTag);
	}

	if (hashTag != std::string::npos)
	{
		uriFragment = m_url.substr(hashTag);
	}
}

HttpMethod::~HttpMethod() { }

std::shared_ptr<IHttpResponse> HttpMethod::generateResponseEntity(int secondsWaitForResponse)
{
	switch (method)
	{
		case HEAD:
			return std::make_shared<HttpHeadResponse>(secondsWaitForResponse);
		case GET:
			return std::make_shared<HttpGetResponse>(secondsWaitForResponse);
		default:
			throw std::runtime_error("no such method implemented: " + method);
	}
}

std::string HttpMethod::generateHTTPRequest()
{
	std::ostringstream requestStream;
	requestStream << allowedRequestMethods[method] << " " << uriPath << " HTTP/1.1\r\n"
				  << "Host: " << uriHost << "\r\n";

	for (auto& param: requestParams)
	{
		requestStream << param << "\r\n";
	}
	requestStream << "\r\n";
	//		   << "Connection: close" << "\r\n\r\n";
//	std::cout << "http request: " << requestStream.str() << std::endl;
	return requestStream.str();
}

HttpMethod *HttpMethod::addRequestParam(const std::string &key, const std::string &value)
{
	requestParams.push_back(key + ": " + value);
	return this;
}

std::string HttpMethod::getHost()
{
	return uriHost;
}
