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

		std::string get = getResponse->getResponsePart(BODY);
		std::string head = headReponse->getResponsePart(HEADER);
		std::string getHead = getResponse->getResponsePart(HEADER);
		std::cout << "response get: " << get << std::endl;
		std::cout << "response head: " << head << std::endl;
		std::cout << "response get2: " << getHead << std::endl;
	}
	catch (const std::runtime_error& err)
	{
		std::cerr << err.what() << std::endl;
	}
}