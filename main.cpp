#include "HTTPClient.h"
#include "response_handlers/HttpHeadResponse.h"
#include "http_methods/HttpMethod.h"

int main()
{
	try
	{
		HTTPClient client;

		HttpMethod methodGet("http://stackoverflow.com/questions/3021146/poll2-doesnt-empty-the-event-queue", GET);
		//	HttpMethod methodHead("http://www.msn.com/ru-ru/", HEAD);

		auto getResponse = client.execute(methodGet, 3);
		//	auto getResponse2 = client.execute(methodGet);
		//	auto headResponse = client.execute(methodHead);

		//	std::cout << localResponse->getResponseStatus() << std::endl;
		//	std::cout << localResponse->getResponseHeader() << std::endl;
		std::cout << "response: " << getResponse->getResponsePart(BODY) << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}