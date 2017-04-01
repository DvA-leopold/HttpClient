#include "HTTPClient.h"

int main()
{
	try
	{
		HTTPClient client;

		HttpMethod methodGet("http://stackoverflow.com/questions/3021146/poll2-doesnt-empty-the-event-queue", GET);
		HttpMethod methodHead("http://www.msn.com/ru-ru/", HEAD);

		auto getResponse = client.execute(methodGet, 3);
		auto headReponse = client.execute(methodHead, 3);
		//	auto headResponse = client.execute(methodHead);

		//	std::cout << localResponse->getResponseStatus() << std::endl;
		//	std::cout << localResponse->getResponseHeader() << std::endl;
		std::string get = getResponse->getResponsePart(BODY);
		std::string head = headReponse->getResponsePart(HEADER);
		std::cout << "response get: \n" << get << std::endl;
		std::cout << "response head: \n" << head << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}