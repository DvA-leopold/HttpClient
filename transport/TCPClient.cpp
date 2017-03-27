//
// Created by operator on 3/26/17.
//

#include "TCPClient.h"


TCPClient::TCPClient(size_t chunkSize)
		: socketDescriptor(-1), chunkSize(chunkSize)
{ }

TCPClient::~TCPClient()
{
	if (socketDescriptor != -1)
	{
//		shutdown(socketDescriptor, 2); // TODO do i need this? 0 - stop receive, 1 - stop transmit, 2 - stop both
		close(socketDescriptor);
	}
}

bool TCPClient::Connect(const char* hostName)
{
	struct addrinfo *servinfo, *next_addrinfo;

	int rc = getaddrinfo(hostName, "http", NULL, &servinfo);
	if (rc != 0)
	{
		std::cerr << "getaddrinfo error: " << gai_strerror(rc) << std::endl;
		return false;
	}

	for (next_addrinfo = servinfo; next_addrinfo != NULL; next_addrinfo = next_addrinfo->ai_next)
	{
		if ((socketDescriptor = socket(next_addrinfo->ai_family, next_addrinfo->ai_socktype, next_addrinfo->ai_protocol)) == -1)
		{
			std::cerr << "socket error: " << errno << std::endl;
			continue;
		}
		if (connect(socketDescriptor, next_addrinfo->ai_addr, next_addrinfo->ai_addrlen) == -1)
		{
			std::cerr << "connect error: " << errno << std::endl;
			close(socketDescriptor);
			continue;
		}

		std::cout << "connect successfully, host: " << hostName << std::endl;
		break;
	}

	if (next_addrinfo == NULL)
	{
		std::cerr << "failed to connect anywhere" << std::endl;
		return false;
	}

	freeaddrinfo(servinfo);
	return true;
}

void TCPClient::Send(std::string data)
{
	if (socketDescriptor == -1) { throw std::runtime_error("socket not initialized, u need to connect first"); }

	while (!data.empty())
	{
		ssize_t bytesSend = send(socketDescriptor, data.c_str(), data.size(), 0);
		if (bytesSend == -1) { throw std::runtime_error("send error: " + std::to_string(errno)); }
		data = data.substr(bytesSend);
		std::cout << "bytes send: " << bytesSend << std::endl;
	}
}

std::string TCPClient::Receive()
{
	if (socketDescriptor == -1) { throw std::runtime_error("socket not initialized, u need to connect first"); }
	char serverReply[chunkSize];
	std::stringstream replyMessageStream;

	while (true)
	{
		memset(serverReply, 0, chunkSize);
		ssize_t bytesRead = 0;

		if ((bytesRead = recv(socketDescriptor, serverReply, chunkSize, 0)) < 0)
		{
			throw std::runtime_error(std::string("error happens in recv, error: ").append(std::to_string(errno)));
		}
		std::cout << "bytes received: " << bytesRead << std::endl;
		if (bytesRead == 0)
		{
			break;
		} // TODO if connection will not break, this wont finish ever, nowait + timeout, chunks, Content-Length?

		replyMessageStream << serverReply;
	}

	std::cout << "total received: " << replyMessageStream.str().size() << std::endl;
	return replyMessageStream.str();
}