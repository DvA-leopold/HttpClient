#include "HTTPClient.h"
#include "response_handlers/HttpHeadResponse.h"
#include "http_methods/HttpMethod.h"

int main()
{
	// If u find some problems here, pls write me a note or report an issue

	// TODO need to handle several response for one domain at a time
	HTTPClient client;

	HttpMethod methodGet("http://stackoverflow.com/questions/3021146/poll2-doesnt-empty-the-event-queue", GET);

//	HttpMethod methodHead("http://www.msn.com/ru-ru/", HEAD);
	auto getResponse = client.execute(methodGet);
//	auto getResponse2 = client.execute(methodGet);
//	auto headResponse = client.execute(methodHead);


//	std::cout << localResponse->getResponseStatus() << std::endl;
//	std::cout << localResponse->getResponseHeader() << std::endl;
	std::cout << getResponse->getResponsePart(BODY) << std::endl;
}