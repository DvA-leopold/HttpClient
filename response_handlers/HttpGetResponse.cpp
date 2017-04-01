#include "HttpGetResponse.h"


HttpGetResponse::HttpGetResponse(int secondsWaitForResponse)
		: headerTransmited(false)
		, IHttpResponse(secondsWaitForResponse)
{
	file.open("../test1.txt");
}

HttpGetResponse::~HttpGetResponse()
{
	file.close();
}

void HttpGetResponse::responseCallback(const std::string&& responseChunk)
{
	localResponse.append(responseChunk);
	receivedBodySize += responseChunk.size();
	if (!headerTransmited)
	{
		long shift = localResponse.size() - responseChunk.size() - 3;
		size_t searchCarriage = shift < 0 ? 0 : shift;
		size_t foundDoubleNewLine = localResponse.find("\r\n\r\n", searchCarriage);
		if (foundDoubleNewLine != std::string::npos)
		{
			receivedBodySize -= (foundDoubleNewLine + 4);
			size_t CLCarriage = localResponse.find("Content-Length");
			size_t RNCarriage = localResponse.find("\r\n", CLCarriage);
			std::string bodyContentSize = localResponse.substr(CLCarriage + 15, RNCarriage);
			contentLength = std::stol(bodyContentSize);
			headerTransmited = true;
		}
	}

	if (headerTransmited)
	{
		std::cout << "GET: " << contentLength << " " << receivedBodySize << std::endl;
//		file << localResponse;

		if ((contentLength - receivedBodySize) == 0)
		{
//			std::cout << "GET" << std::endl;
			responsePromise.set_value(std::move(localResponse));
			headerTransmited = false;
			contentLength = 0;
			receivedBodySize = 0;
		}
	}
}

void HttpGetResponse::parseResponse(const std::string&& response)
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
