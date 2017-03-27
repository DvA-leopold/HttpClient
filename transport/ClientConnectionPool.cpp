//
// Created by operator on 3/26/17.
//

#include "ClientConnectionPool.h"

ClientConnectionPool::ClientConnectionPool(unsigned poolCapacity)
		: poolCapacity(poolCapacity) { }

std::future<std::string> ClientConnectionPool::send(const std::string& host, const std::string& data)
{
	// TODO need a real connection pooling here

	std::future<std::string> executionFuture = std::async(std::launch::async,
														  [](std::string lhost, std::string ldata)
	{
		try
		{
			TCPClient transportClient;
			if (transportClient.Connect(lhost.c_str()))
			{
				transportClient.Send(ldata);
				return transportClient.Receive();
			};
		}
		catch (std::runtime_error& error)
		{
			std::cerr << error.what() << std::endl;
		}
		return std::string();
	}, host, data);

    return executionFuture;
}

