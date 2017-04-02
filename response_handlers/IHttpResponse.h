#ifndef HTTPCLIENT_HTTPRESPONSE_H
#define HTTPCLIENT_HTTPRESPONSE_H


#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <atomic>
#include <future>
#include <iostream>

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
	IHttpResponse(int secondsWaitForResponse)
			: secondsWaitForResponse_(secondsWaitForResponse)
	{ }
	virtual ~IHttpResponse() { }

	virtual bool responseCallback(const std::string&& response) = 0;

	std::string getResponsePart(ResponseParts partNameToGet)
	{
		responseFuture_ = responsePromise_.get_future();
		std::future_status ftrStatus = responseFuture_.wait_for(std::chrono::seconds(secondsWaitForResponse_));

		if (ftrStatus != std::future_status::ready)
		{
			std::cerr << "timeout exceeded: " << secondsWaitForResponse_ << " sec." << std::endl;
			return "";
		}

		parseResponse(responseFuture_.get());

		switch (partNameToGet)
		{
			case STATUS:
				return responseStatus_;
			case HEADER:
				return responseHeader_;
			case BODY:
				return responseBody_;
		}
		return "";
	}

	std::vector<std::string> getResponseHeaderParam(const std::string& keyForSearch)
	{
		if (responseHeaderParams_.empty()) { parseHeaderParams(); }

		std::vector< std::string > returnResponseParams;

		std::pair<multimapIter, multimapIter> retPair(responseHeaderParams_.equal_range(keyForSearch));
		for (auto it=retPair.first; it!=retPair.second; ++it)
		{
			returnResponseParams.push_back(it->second);
		}

		return returnResponseParams;
	}

private:
	void parseHeaderParams()
	{
		if (!responseHeader_.empty())
		{
			size_t first_search_carriage_RN = 0, last_search_carriage_RN = responseHeader_.find("\r\n");
			do
			{
				std::string headerParam = responseHeader_.substr(first_search_carriage_RN,
																last_search_carriage_RN - first_search_carriage_RN);
				size_t delimPosition = headerParam.find(':');
				responseHeaderParams_.insert(std::make_pair(headerParam.substr(0, delimPosition),
														   headerParam.substr(delimPosition + 2)));

				first_search_carriage_RN = last_search_carriage_RN + 2;;
				last_search_carriage_RN = responseHeader_.find("\r\n", first_search_carriage_RN);
			} while (last_search_carriage_RN != std::string::npos);
		}
	}

protected:
	virtual void parseResponse(const std::string&& response) = 0;

protected:
	std::string responseStatus_;
	std::string responseHeader_;
	std::string responseBody_;
	std::promise<std::string> responsePromise_;
	std::future<std::string> responseFuture_;

private:
	int secondsWaitForResponse_;
	std::multimap<std::string, std::string> responseHeaderParams_;
};

#endif //HTTPCLIENT_HTTPRESPONSE_H
