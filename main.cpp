#include "HTTPClient.h"

int main()
{
	// If u find some problems here, pls write me a note or report an issue, just interesting

	HTTPClient client;
	IHttpResponse* response = new HttpGetResponse(); // TODO control lifetime

	client.execute(new IHttpMethod("http://www.msn.com/ru-ru/", GET), response);

//	std::cout << localResponse->getResponseStatus() << std::endl;
//	std::cout << localResponse->getResponseHeader() << std::endl;
//	std::cout << localResponse->getResponseBody() << std::endl;
}