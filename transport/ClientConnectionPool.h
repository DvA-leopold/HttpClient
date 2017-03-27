#ifndef HTTPCLIENT_CLIENTCONNECTIONPOOL_H
#define HTTPCLIENT_CLIENTCONNECTIONPOOL_H


#include <future>
#include <sstream>
#include <unordered_map>
#include "TCPClient.h"

class ClientConnectionPool
{
public:
    ClientConnectionPool(unsigned poolCapacity = 16);

    std::future<std::string> send(const std::string& host, const std::string& data);

private:
	const unsigned poolCapacity; // TODO const thread safe?
	std::atomic<unsigned> poolLoad;
	std::unordered_map<std::string, TCPClient> connectionPool;
};


#endif //HTTPCLIENT_CLIENTCONNECTIONPOOL_H
