#ifndef HTTPCLIENT_HTTPMETHOD_H
#define HTTPCLIENT_HTTPMETHOD_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

enum Methods
{
	GET,
	HEAD
};

class IHttpMethod
{
public:
    IHttpMethod(const std::string& url,
				Methods method)
            : m_url(url), uriPath("/")
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

		switch (method)
		{
			case GET:
				requestMethod = "GET";
				break;
			case HEAD:
				requestMethod = "HEAD";
				break;
		}
	};

    virtual ~IHttpMethod() {};

	//		   << "Connection: close" << "\r\n\r\n";
	virtual std::string generateHTTPRequest()
	{
		std::ostringstream requestStream;
		requestStream << requestMethod << " " << uriPath << " HTTP/1.1\r\n"
			          << "Host: " << uriHost << "\r\n";
		for (auto& param: requestParams)
		{
			requestStream << param << "\r\n";
		}
		requestStream << "\r\n";

		std::cout << "http request: " << requestStream.str() << std::endl;
		return requestStream.str();
	}

	IHttpMethod* addRequestParam(const std::string& key, const std::string& value)
	{
		requestParams.push_back(key + ": " + value);
		return this;
	}

	std::string getHost() { return uriHost; }

protected:
	std::string m_url;

	std::string requestMethod;
	std::string uriHost;
	std::string uriPath;
	std::string uriQuery;
	std::string uriFragment;

private:
	std::vector<std::string> requestParams;
};


#endif //HTTPCLIENT_HTTPMETHOD_H
