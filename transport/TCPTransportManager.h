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

typedef std::pair<int, std::shared_ptr<IHttpResponse>> sock_response;
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

	void Connect(const std::string& hostName, const std::shared_ptr<IHttpResponse>& responseEntity);
	void Disconnect(const std::string& hostName);
	void DisconnectAll();
	void Send(const std::string& hostName, std::string&& data);

private:
	void Poll();
	std::string Receive(int socketDescriptor);

private:
	std::mutex connectionMutex;
	std::unordered_map<std::string, sock_response> connectionMap;

private:
	std::thread packetReceiver;
	std::atomic<TransportStates> state;

private:
	size_t chunkSize;
	const int millisecondsTimeout;
//	long long totalMSGsize = 0;
};


#endif //HTTPCLIENT_TCPTRANSPORT_H
