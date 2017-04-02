#include "HttpMethod.h"


const std::vector<std::string> HttpMethod::allowedRequestMethods_ = {"GET", "HEAD"};

HttpMethod::HttpMethod(const std::string &url, Methods method)
		: url_(url), uriPath_("/"), method_(method)
{
	size_t doubleSlash = url_.find("//");
	if (doubleSlash != std::string::npos)
	{
		url_ = url_.substr(doubleSlash + 2);
	}
	size_t firstSlash = url_.find('/');
	size_t lastSlash = url_.find_last_of('/');
	size_t questionMark = url_.find('?');
	size_t hashTag = url_.find('#');

	uriHost_ = url_.substr(0, firstSlash);

	if (lastSlash != firstSlash)
	{
		uriPath_ = url_.substr(firstSlash, questionMark == std::string::npos ? url_.size() : questionMark);
	}

	if (questionMark != std::string::npos)
	{
		uriQuery_ = url_.substr(questionMark, hashTag == std::string::npos ? url_.size() : hashTag);
	}

	if (hashTag != std::string::npos)
	{
		uriFragment_ = url_.substr(hashTag);
	}
}

HttpMethod::~HttpMethod() { }

std::shared_ptr<IHttpResponse> HttpMethod::generateResponseEntity(int secondsWaitForResponse)
{
	switch (method_)
	{
		case HEAD:
			return std::make_shared<HttpHeadResponse>(secondsWaitForResponse);
		case GET:
			return std::make_shared<HttpGetResponse>(secondsWaitForResponse);
		default:
			throw std::runtime_error("no such method implemented: " + method_);
	}
}

std::string HttpMethod::generateHTTPRequest()
{
	std::ostringstream requestStream;
	requestStream << allowedRequestMethods_[method_] << " " << uriPath_ << " HTTP/1.1\r\n"
				  << "Host: " << uriHost_ << "\r\n";
	for (auto& param: requestParams_)
	{
		requestStream << param << "\r\n";
	}
	requestStream << "Connection: close" << "\r\n\r\n";
	return requestStream.str();
}

HttpMethod *HttpMethod::addRequestParam(const std::string &key, const std::string &value)
{
	requestParams_.push_back(key + ": " + value);
	return this;
}

std::string HttpMethod::getHost()
{
	return uriHost_;
}
