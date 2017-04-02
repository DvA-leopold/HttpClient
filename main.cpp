#include "HTTPClient.h"

int main()
{
	try
	{
		HTTPClient client;

		HttpMethod methodGet("http://stackoverflow.com/questions/3021146/poll2-doesnt-empty-the-event-queue", GET);
		HttpMethod methodHead("http://www.msn.com/ru-ru/", HEAD);
		HttpMethod methodGet2("http://stackoverflow.com/questions/tagged/c%2B%2B11", GET);

		auto getResponse = client.execute(methodGet, 3);
		auto headReponse = client.execute(methodHead, 3);
		auto getResponse2 = client.execute(methodGet2, 3);
		//	auto headResponse = client.execute(methodHead);

		//	std::cout << localResponse->getResponseStatus() << std::endl;
		//	std::cout << localResponse->getResponseHeader() << std::endl;
		std::string get = getResponse->getResponsePart(BODY);
		std::string head = headReponse->getResponsePart(HEADER);
		std::string get2 = getResponse2->getResponsePart(BODY);
		std::cout << "response get: " << get.empty() << std::endl;
		std::cout << "response head: " << head.empty() << std::endl;
		std::cout << "response get2: " << get2.empty() << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}