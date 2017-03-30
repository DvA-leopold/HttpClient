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
		// all hell broke loose here
		// better to use standard libs for this
		if (m_url.find("//") != std::string::npos)
		{
			m_url = m_url.substr(m_url.find("//") + 2);
		}
		uriHost = m_url.substr(0, m_url.find('/'));
		if (m_url.find_last_of('/') != m_url.find('/'))
		{
			uriPath = m_url.substr(m_url.find('/'), m_url.find_last_of('/'));
		}
		if (m_url.find_last_of('/') != std::string::npos)
		{
			uriParams = m_url.substr(m_url.find_last_of('/'));
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
	std::string uriParams;

private:
	std::vector<std::string> requestParams;
};


#endif //HTTPCLIENT_HTTPMETHOD_H
