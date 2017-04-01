#include "TCPTransportManager.h"


TCPTransportManager::TCPTransportManager(int millisecondsTimeout)
		: chunkSize(512),
		  millisecondsTimeout(millisecondsTimeout),
		  state(PROCEED)
{ }

TCPTransportManager::TCPTransportManager(size_t chunkSize, int millisecondsTimeout)
		: chunkSize(chunkSize),
		  millisecondsTimeout(millisecondsTimeout),
		  state(PROCEED)
{ }

TCPTransportManager::~TCPTransportManager()
{
	state = STOP;
	packetReceiver.join();

	for (auto& connections: connectionMap)
	{
		close(connections.second.first);
	}
}

void TCPTransportManager::Connect(const std::string& hostName, const std::shared_ptr<IHttpResponse>& responseEntity)
{
	{
		std::unique_lock<std::mutex> lock(connectionMutex);
		if (connectionMap.find(hostName) != connectionMap.end())
		{ return; }
	}

	uniq_ptr_del servinfo(NULL, [](addrinfo* srv) { if (srv != NULL) { freeaddrinfo(srv); } });
	addrinfo *tmp, *next_addrinfo;

	int rc = getaddrinfo(hostName.c_str(), "http", NULL, &tmp);
	if (rc != 0) { throw std::runtime_error("getaddrinfo error: " + std::string(gai_strerror(rc))); }
	servinfo.reset(tmp);

	int socketDescriptor;
	for (next_addrinfo = servinfo.get(); next_addrinfo != NULL; next_addrinfo = next_addrinfo->ai_next)
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

		int on = 1;
		if (ioctl(socketDescriptor, FIONBIO, (char* )&on) < 0)
		{
			close(socketDescriptor);
			throw std::runtime_error("ioctl error: " + std::to_string(errno));
		}
		std::cout << "uriHost: " << hostName << "successfully connected" << std::endl;
		break;
	}

	if (next_addrinfo == NULL) { throw std::runtime_error("failed to connect anywhere"); }

	auto connectionPair = std::make_pair(hostName, std::make_pair(socketDescriptor, responseEntity));
	std::unique_lock<std::mutex> lock(connectionMutex); // TODO second mutex capture, try to optimize
	connectionMap.insert(connectionPair);
	state = REFRESH;
}

void TCPTransportManager::Disconnect(const std::string& hostName)
{
	std::unique_lock<std::mutex> lock(connectionMutex);
	auto connectionIter = connectionMap.find(hostName);
	if (connectionIter != connectionMap.end())
	{
		connectionMap.erase(connectionIter);
		close(connectionIter->second.first);
	}
	state = REFRESH;
}

void TCPTransportManager::DisconnectAll()
{
	std::unique_lock<std::mutex> lock(connectionMutex);
	for (auto& connPair: connectionMap)
	{
		shutdown(connPair.second.first, SHUT_RDWR);
		close(connPair.second.first);
	}
	state = STOP;
}

void TCPTransportManager::Send(const std::string& hostName, std::string&& data)
{
	int socketDescriptor;
	{
		std::unique_lock<std::mutex> lock(connectionMutex);
		if (connectionMap.find(hostName) == connectionMap.end())
		{
			throw std::runtime_error("socket not initialized, u need to connect first");
		}
		socketDescriptor = connectionMap[hostName].first;
	}

	while (!data.empty())
	{
		ssize_t bytesSend = send(socketDescriptor, data.c_str(), data.size(), 0);
		if (bytesSend == -1) { throw std::runtime_error("send error: " + std::to_string(errno)); }
		data = data.substr(bytesSend);
//		std::cout << "bytes send: " << bytesSend << std::endl;
	}

	if (!packetReceiver.joinable())
	{
		packetReceiver = std::thread(&TCPTransportManager::Poll, this);
	}
}

std::string TCPTransportManager::Receive(int socketDescriptor)
{
	char serverReply[chunkSize];
	std::stringstream replyMessageStream;

	while (true)
	{
		ssize_t bytesRead = 0;
		if ((bytesRead = recv(socketDescriptor, serverReply, chunkSize, 0)) < 0 && errno != EAGAIN) // EAGAIN == EWOULDBLOCK
		{
			std::cerr << "error happens in recv, error: " << errno << std::endl;
			break;
		}
//		std::cout << "bytes received: " << bytesRead << std::endl;
		if( bytesRead == 0 || (bytesRead == -1 && errno == EAGAIN)) { break; }

		replyMessageStream << std::string(serverReply, bytesRead);
	};

	return replyMessageStream.str();
}

void TCPTransportManager::Poll()
{
	while (state != STOP)
	{
		std::unordered_map<int, std::shared_ptr<IHttpResponse>> fdCallbackMap;
		size_t sockDescriptorIter = 0;
		std::unique_ptr<pollfd[]> sockDescArray;
		{
			std::unique_lock<std::mutex> lock(connectionMutex);
			sockDescArray = std::make_unique<pollfd[]>(connectionMap.size());
			for (auto& host_fd_callback: connectionMap)
			{
				sockDescArray[sockDescriptorIter] = { host_fd_callback.second.first, POLLIN };
				fdCallbackMap.insert(host_fd_callback.second);
				sockDescriptorIter++;
			}
		}
		TransportStates tmpState = REFRESH;
		state.compare_exchange_strong(tmpState, PROCEED);

		while (state == PROCEED)
		{
			int rc = poll(sockDescArray.get(), fdCallbackMap.size(), millisecondsTimeout);
			if (rc < 0) { std::cerr << "poll error: " << errno << std::endl; break; }
			if (rc == 0) { std::cout << "break by timeout: " << millisecondsTimeout << std::endl; continue; }

			for (size_t i=0; i<fdCallbackMap.size(); ++i)
			{
				if (sockDescArray[i].revents & POLLIN)
				{
					int socketDescriptor = sockDescArray[i].fd;
					IHttpResponse* httpResponse = fdCallbackMap[socketDescriptor].get();
					httpResponse->responseCallback(std::move(Receive(socketDescriptor)));
					rc--;
					if (rc == 0) { break; };
				}
				if (sockDescArray[i].revents & POLLHUP)
				{
					std::cout << "closed" << std::endl;
					rc--;
					if (rc == 0) { break; };
				}
				// POLLERR  /* Error condition.  */
				// POLLHUP  /* Hung up.  */
				// POLLNVAL /* Invalid polling request.  */
			}
		}
	}
}