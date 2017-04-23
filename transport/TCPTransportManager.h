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
#include <set>
#include "../response_handlers/IHttpResponse.h"

typedef std::unique_ptr<addrinfo, std::function<void(addrinfo*)>> uniq_ptr_del;

enum TransportStates
{
	STOP,
	PROCEED,
	REFRESH
};

class TCPTransportManager
{
public:
	TCPTransportManager(int millisecondsTimeout = 5 * 1000);
	TCPTransportManager(size_t chunkSize = 512, int millisecondsTimeout = 5 * 1000);
	~TCPTransportManager();

	int Connect(const std::string& hostName, const std::shared_ptr<IHttpResponse>& responseEntity);
	void Send(int socketDescriptor, std::string&& data);

private:
	void Poll();
	std::string Receive(int socketDescriptor);

private:
	std::mutex connectionMutex_;
	std::unordered_map<int, std::shared_ptr<IHttpResponse>> socketsToHandlersMap_;

private:
	std::thread packetReceiver_;
	std::atomic<TransportStates> state_;

private:
	size_t chunkSize_;
	const int millisecondsTimeout_;
};


#endif //HTTPCLIENT_TCPTRANSPORT_H
