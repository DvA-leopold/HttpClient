//
// Created by operator on 3/26/17.
//

#ifndef HTTPCLIENT_TCPTRANSPORT_H
#define HTTPCLIENT_TCPTRANSPORT_H

#include <stdexcept>
#include <netdb.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>

class TCPClient
{
public:
	TCPClient(size_t chunkSize = 512);
	~TCPClient();

	bool Connect(const char* hostName);
	void Send(std::string data);
	std::string Receive();

private:
	int socketDescriptor;
	size_t chunkSize;
};


#endif //HTTPCLIENT_TCPTRANSPORT_H
