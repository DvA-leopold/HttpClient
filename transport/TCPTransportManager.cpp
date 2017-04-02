#include "TCPTransportManager.h"


TCPTransportManager::TCPTransportManager(int millisecondsTimeout)
		: chunkSize_(512),
		  millisecondsTimeout_(millisecondsTimeout),
		  state_(PROCEED)
{ }

TCPTransportManager::TCPTransportManager(size_t chunkSize, int millisecondsTimeout)
		: chunkSize_(chunkSize),
		  millisecondsTimeout_(millisecondsTimeout),
		  state_(PROCEED)
{ }

TCPTransportManager::~TCPTransportManager()
{
	state_ = STOP;
	packetReceiver_.join();

	for (auto& connections: socketsToHandlersMap_)
	{
		close(connections.first);
	}
}

int TCPTransportManager::Connect(const std::string& hostName, const std::shared_ptr<IHttpResponse>& responseEntity)
{
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

	{
		std::unique_lock<std::mutex> lock(connectionMutex_);
		socketsToHandlersMap_.insert(std::make_pair(socketDescriptor, responseEntity));
	}
	state_ = REFRESH;
	return socketDescriptor;
}

void TCPTransportManager::Send(int socketDescriptor, std::string&& data)
{
	if (socketDescriptor == -1)
	{
		throw std::runtime_error("socket not initialized, u need to connect first");
	}

	while (!data.empty())
	{
		ssize_t bytesSend = send(socketDescriptor, data.c_str(), data.size(), 0);
		if (bytesSend == -1) { throw std::runtime_error("send error: " + std::to_string(errno)); }
		data = data.substr(bytesSend);
	}

	if (!packetReceiver_.joinable())
	{
		packetReceiver_ = std::thread(&TCPTransportManager::Poll, this);
	}
}

std::string TCPTransportManager::Receive(int socketDescriptor)
{
	char serverReply[chunkSize_];
	std::stringstream replyMessageStream;

	while (true)
	{
		ssize_t bytesRead = 0;
		if ((bytesRead = recv(socketDescriptor, serverReply, chunkSize_, 0)) < 0 && errno != EAGAIN) // EAGAIN == EWOULDBLOCK
		{
			std::cerr << "error happens in recv, error: " << errno << std::endl;
			break;
		}
		if( bytesRead == 0 || (bytesRead == -1 && errno == EAGAIN)) { break; }

		replyMessageStream << std::string(serverReply, bytesRead);
	};

	return replyMessageStream.str();
}

void TCPTransportManager::Poll()
{
	while (state_ != STOP)
	{
		std::unordered_map<int, std::shared_ptr<IHttpResponse>> responseHandlersMap;
		std::unique_ptr<pollfd[]> sockDescArray;
		size_t sockDescriptorIter = 0;
		{
			std::unique_lock<std::mutex> lock(connectionMutex_);
			sockDescArray = std::make_unique<pollfd[]>(socketsToHandlersMap_.size());
			for (auto& fd_callback: socketsToHandlersMap_)
			{
				sockDescArray[sockDescriptorIter] = { fd_callback.first, POLLIN };
				sockDescriptorIter++;
			}
			responseHandlersMap = socketsToHandlersMap_;
		}

		TransportStates tmpState = REFRESH;
		state_.compare_exchange_strong(tmpState, PROCEED);

		while (state_ == PROCEED)
		{
			int rc = poll(sockDescArray.get(), responseHandlersMap.size(), millisecondsTimeout_);
			if (rc < 0) { std::cerr << "poll error: " << errno << std::endl; break; }
			if (rc == 0) { std::cout << "break by timeout: " << millisecondsTimeout_ << std::endl; continue; }

			for (size_t i=0; i<sockDescriptorIter; ++i)
			{
				if (sockDescArray[i].revents & POLLIN)
				{
					int socketDescriptor = sockDescArray[i].fd;
					IHttpResponse* httpResponse = responseHandlersMap[socketDescriptor].get();
					if (httpResponse->responseCallback(std::move(Receive(socketDescriptor))))
					{
						std::unique_lock<std::mutex> lock(connectionMutex_);
						socketsToHandlersMap_.erase(sockDescArray[i].fd);
					}
					rc--;
					if (rc == 0) { break; };
				}
				if (sockDescArray[i].revents & POLLHUP) //
				{
					std::unique_lock<std::mutex> lock(connectionMutex_);
					socketsToHandlersMap_.erase(sockDescArray[i].fd);
					rc--;
					if (rc == 0) { break; };
				}

				// POLLERR  /* Error condition.  */
				// POLLNVAL /* Invalid polling request.  */
			}
		}
	}
}
