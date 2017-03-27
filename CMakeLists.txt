cmake_minimum_required(VERSION 3.6)
project(HttpClient)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lpthread")

set(SOURCE_FILES main.cpp
        HTTPClient.cpp HTTPClient.h
        transport/ClientConnectionPool.cpp transport/ClientConnectionPool.h
        http_methods/HttpGet.cpp http_methods/HttpGet.h
        http_methods/HttpPost.cpp http_methods/HttpPost.h
        HttpResponse.cpp HttpResponse.h
        http_methods/IHttpMethod.h
        http_methods/HttpHead.cpp http_methods/HttpHead.h
        transport/TCPClient.cpp transport/TCPClient.h)
add_executable(HttpClient ${SOURCE_FILES})