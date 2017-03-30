#ifndef HTTPCLIENT_TCPTRANSPORT_H
#define HTTPCLIENT_TCPTRANSPORT_H

#include <stdexcept>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <sys/socket.h>
#include <thread>
#include <unordered_map>
#include <sys/ioctl.h>
#include <poll.h>
#include <netdb.h>
#include <atomic>
#include <mutex>
#include "../response_handlers/IHttpResponse.h"

typedef std::pair<int, IHttpResponse*> sock_response;
typedef std::unique_ptr<addrinfo, std::function<void(addrinfo*)>> uniq_ptr_del;

class TCPTransportManager
{
public:
	TCPTransportManager(int millisecondsTimeout = 5 * 1000);
	TCPTransportManager(bool startAsync, size_t chunkSize = 512, int millisecondsTimeout = 5 * 1000);
	~TCPTransportManager();

	void Connect(const std::string& hostName, IHttpResponse* const responseEntity);
	void Send(const std::string& hostName, std::string&& data);

private:
	void Poll();
	std::string Receive(int socketDescriptor);

private:
	std::mutex connectionMutex;
	std::unordered_map<std::string, sock_response> connectionMap;

private:
	std::thread packetReceiver;
	std::atomic<bool> stop;

private:
	bool startAsync;
	size_t chunkSize;
	const int millisecondsTimeout;
};


#endif //HTTPCLIENT_TCPTRANSPORT_H
