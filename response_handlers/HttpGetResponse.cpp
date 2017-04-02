#include "HttpGetResponse.h"


HttpGetResponse::HttpGetResponse(int secondsWaitForResponse)
		: headerTransmited_(false)
		, IHttpResponse(secondsWaitForResponse)
{ }

HttpGetResponse::~HttpGetResponse()
{ }

bool HttpGetResponse::responseCallback(const std::string&& responseChunk)
{
	localResponse_.append(responseChunk);
	receivedBodySize_ += responseChunk.size();
	if (!headerTransmited_)
	{
		long shift = localResponse_.size() - responseChunk.size() - 3;
		size_t searchCarriage = shift < 0 ? 0 : shift;
		size_t foundDoubleNewLine = localResponse_.find("\r\n\r\n", searchCarriage);
		if (foundDoubleNewLine != std::string::npos)
		{
			receivedBodySize_ -= (foundDoubleNewLine + 4);
			size_t CLCarriage = localResponse_.find("Content-Length");
			size_t RNCarriage = localResponse_.find("\r\n", CLCarriage);
			std::string bodyContentSize = localResponse_.substr(CLCarriage + 15, RNCarriage);
			contentLength_ = std::stol(bodyContentSize);
			headerTransmited_ = true;
		}
	}

	if (headerTransmited_)
	{
		if (receivedBodySize_ - contentLength_ == 0)
		{
			if (!localResponse_.empty())
			{
				responsePromise_.set_value(std::move(localResponse_));
			}
			receivedBodySize_ = 0;
			contentLength_ = 0;
			return true;
		}
	}
	return false;
}

void HttpGetResponse::parseResponse(const std::string&& response)
{
	size_t first_search_carriage = 0;
	size_t last_search_carriage = response.find("\r\n");
	responseStatus_ = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

	first_search_carriage = last_search_carriage + 4;
	last_search_carriage = response.find("\r\n\r\n", first_search_carriage);
	responseHeader_ = response.substr(first_search_carriage, last_search_carriage - first_search_carriage);

	first_search_carriage = last_search_carriage + 4;
	responseBody_ = response.substr(first_search_carriage);
}
