#ifndef HTTPCLIENT_HTTPRESPONSE_H
#define HTTPCLIENT_HTTPRESPONSE_H


#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <atomic>
#include <future>

typedef std::multimap<std::string, std::string>::iterator multimapIter;

enum ResponseParts
{
	STATUS = 0,
	HEADER,
	BODY,
};

class IHttpResponse
{
public:
	IHttpResponse() { }
	virtual ~IHttpResponse() { }

	virtual void responseCallback(const std::string&& response) = 0;

	std::string getResponsePart(ResponseParts partNameToGet)
	{
		parseResponse(responsePromise.get_future().get());

		switch (partNameToGet)
		{
			case STATUS:
				return responseStatus;
			case HEADER:
				return responseHeader;
			case BODY:
				return responseBody;
		}
		return "";
	}

	std::vector<std::string> getResponseHeaderParam(const std::string& keyForSearch)
	{
		if (responseHeaderParams.empty()) { parseHeaderParams(); }

		std::vector< std::string > returnResponseParams;

		std::pair<multimapIter, multimapIter> retPair(responseHeaderParams.equal_range(keyForSearch));
		for (auto it=retPair.first; it!=retPair.second; ++it)
		{
			returnResponseParams.push_back(it->second);
		}

		return returnResponseParams;
	}

private:
	void parseHeaderParams()
	{
		if (!responseHeader.empty())
		{
			size_t first_search_carriage_RN = 0, last_search_carriage_RN = responseHeader.find("\r\n");
			do
			{
				std::string headerParam = responseHeader.substr(first_search_carriage_RN,
																last_search_carriage_RN - first_search_carriage_RN);
				size_t delimPosition = headerParam.find(':');
				responseHeaderParams.insert(std::make_pair(headerParam.substr(0, delimPosition),
														   headerParam.substr(delimPosition + 2)));

				first_search_carriage_RN = last_search_carriage_RN + 2;;
				last_search_carriage_RN = responseHeader.find("\r\n", first_search_carriage_RN);
			} while (last_search_carriage_RN != std::string::npos);
		}
	}

protected:
	virtual void parseResponse(const std::string&& response) = 0;

protected:
	std::string responseStatus;
	std::string responseHeader;
	std::string responseBody;
	std::promise<std::string> responsePromise;

private:
	std::multimap<std::string, std::string> responseHeaderParams;
};

#endif //HTTPCLIENT_HTTPRESPONSE_H
