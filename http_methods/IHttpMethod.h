//
// Created by operator on 3/26/17.
//

#ifndef HTTPCLIENT_HTTPMETHOD_H
#define HTTPCLIENT_HTTPMETHOD_H

#include <string>
#include <sstream>
#include <vector>
#include <iostream>

class IHttpMethod
{
public:
    IHttpMethod(const std::string& url, const std::string& requestMethod)
            : m_url(url), requestMethod(requestMethod), path("/")
	{
		// all hell broke loose here
		// TODO better to use standard libs for this
		if (m_url.find("//") != std::string::npos)
		{
			m_url = m_url.substr(m_url.find("//") + 2);
		}
		host = m_url.substr(0, m_url.find('/'));
		if (m_url.find_last_of('/') != m_url.find('/'))
		{
			path = m_url.substr(m_url.find('/'), m_url.find_last_of('/'));
		}
		if (m_url.find_last_of('/') != std::string::npos)
		{
			params = m_url.substr(m_url.find_last_of('/'));
		}
	};

    virtual ~IHttpMethod() {};

    virtual std::string generateHTTPRequest() = 0;

	std::string getHost() { return host; }

protected:
	std::string m_url;

	const std::string requestMethod;
	std::string host;
	std::string path;
	std::string params;
};


#endif //HTTPCLIENT_HTTPMETHOD_H
