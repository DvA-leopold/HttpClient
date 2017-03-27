#include <iostream>
#include <sstream>
#include <map>
#include <unordered_map>
#include "transport/TCPClient.h"
#include "HttpResponse.h"
#include "HTTPClient.h"
#include "http_methods/HttpGet.h"
#include "http_methods/HttpHead.h"

void testTCPclient()
{
	TCPClient transport;
	try
	{
		if(transport.Connect("www.msn.com"))
		{
			std::string path("/ru-ru/"), host("www.msn.com");
//            std::string data("GET /?st=1 HTTP/1.1\r\nHost: www.msn.com\r\n\r\n");
			std::ostringstream os;
			os << "GET " << path << " HTTP/1.1\r\n" //�#w'� ??? wtf is this??
			   << "Host: " << host << "\r\n"
			   << "Connection: close" << "\r\n\r\n";

			transport.Send(os.str());
			HttpResponse response(transport.Receive());
			std::cout << "---------------------------------------------------------------------------" << std::endl;
			std::cout << "status: " << response.getResponseStatus() << std::endl;
			std::cout << "---------------------------------------------------------------------------" << std::endl;
			std::cout << "header: " << response.getResponseHeader() << std::endl;
			std::cout << "---------------------------------------------------------------------------" << std::endl;
			std::cout << "body: " << response.getResponseBody() << std::endl;
			std::cout << "---------------------------------------------------------------------------" << std::endl;

			std::vector< std::string > data = response.getResponseHeaderParam("Pragma");
			for(std::string i: data) {
				std::cout << i << std::endl;
			}
		}
	}
	catch (std::runtime_error& err)
	{
		std::cout << err.what();
	}
}

int main()
{
	HTTPClient client;
	HttpResponse response = client.execute(new HttpGet("http://www.msn.com/ru-ru/"));
	std::cout << response.getResponseStatus() << std::endl;
	std::cout << response.getResponseHeader() << std::endl;
	std::cout << response.getResponseBody() << std::endl;
}