#include "HttpGetResponse.h"


HttpGetResponse::HttpGetResponse() : headerTransmited(false) { }

void HttpGetResponse::responseCallback(std::string&& responseChunk)
{
	localResponse.append(responseChunk);
	if (!headerTransmited)
	{
		long shift = localResponse.size() - responseChunk.size() - 3;
		size_t searchCarriege = shift < 0 ? 0 : shift;
		if (localResponse.find("\r\n\r\n", searchCarriege) != std::string::npos)
		{
			size_t CLCarriage = localResponse.find("Content-Length");
			size_t RNCarriage = localResponse.find("\r\n", CLCarriage);
			std::string bodyContentSize = localResponse.substr(CLCarriage, RNCarriage);
			expectedContentSize = std::stoul(bodyContentSize);
			headerTransmited = true;
		}
	}

	if ((expectedContentSize -= responseChunk.size()) <= 0)
	{
		moveResponseAtomic(localResponse);
	}
}

void HttpGetResponse::parseResponse(const std::string& response)
{
	size_t first_search_carriage = 0;
	size_t last_search_carriage = response.find("\r\n");
	responseStatus = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

	first_search_carriage = last_search_carriage + 4;
	last_search_carriage = response.find("\r\n\r\n", first_search_carriage);
	responseHeader = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

	first_search_carriage = last_search_carriage + 4;
	responseBody = response.substr(first_search_carriage);
}
