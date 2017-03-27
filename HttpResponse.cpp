//
// Created by operator on 3/25/17.
//

#include "HttpResponse.h"

HttpResponse::HttpResponse(std::future<std::string>&& futureResponse)
		: futureResponse(std::move(futureResponse))
{ }

HttpResponse::HttpResponse(const std::string& response)
{ parseResponse(response); }

std::string HttpResponse::getResponseStatus()
{
	if (futureResponse.valid()) { parseResponse(futureResponse.get()); }
	return responseStatus;
}

std::string HttpResponse::getResponseHeader()
{
	if (futureResponse.valid()) { parseResponse(futureResponse.get()); }
	return responseHeader;
}

std::string HttpResponse::getResponseBody()
{
	if (futureResponse.valid()) { parseResponse(futureResponse.get()); }
	return responseBody;
}

std::vector<std::string> HttpResponse::getResponseHeaderParam(const std::string& paramKey)
{
	if (responseHeaderParams.empty()) { parseHeaderResponse(responseHeader); }

	std::vector< std::string > returnResponseParams;

	std::pair<multimapIter, multimapIter> retPair(responseHeaderParams.equal_range(paramKey));
	for (auto it=retPair.first; it!=retPair.second; ++it)
	{
		returnResponseParams.push_back(it->second);
	}

    return returnResponseParams;
}

void HttpResponse::parseResponse(const std::string& response)
{
	if (!response.empty())
	{
		size_t first_search_carriage = 0;
		size_t last_search_carriage = response.find("\r\n");
		responseStatus = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

		first_search_carriage = last_search_carriage + 4;
		last_search_carriage = response.find("\r\n\r\n", first_search_carriage);
		responseHeader = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

		if (last_search_carriage != std::string::npos)
		{
			first_search_carriage = last_search_carriage + 4;
			responseBody = response.substr(first_search_carriage);
		}
	}
}

void HttpResponse::parseHeaderResponse(const std::string& responseHeader)
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
