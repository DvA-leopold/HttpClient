#include "HttpHeadResponse.h"


HttpHeadResponse::HttpHeadResponse() { }

void HttpHeadResponse::responseCallback(const std::string&& responseChunk)
{
	localResponse.append(responseChunk);
	long shift = localResponse.size() - responseChunk.size() - 3;
	size_t searchCarriege = shift < 0 ? 0 : shift;
	if (localResponse.find("\r\n\r\n", searchCarriege) != std::string::npos)
	{
		std::cout << "HEAD" << std::endl;
		responsePromise.set_value(std::move(localResponse));
	}
}

void HttpHeadResponse::parseResponse(const std::string&& response)
{
	size_t first_search_carriage = 0;
	size_t last_search_carriage = response.find("\r\n");
	responseStatus = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

	first_search_carriage = last_search_carriage + 4;
	last_search_carriage = response.find("\r\n\r\n", first_search_carriage);
	responseHeader = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);
}
