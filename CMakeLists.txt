cmake_minimum_required(VERSION 3.6)
project(HttpClient)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-lpthread")

set(SOURCE_FILES main.cpp
        HTTPClient.cpp HTTPClient.h
        response_handlers/HttpGetResponse.cpp response_handlers/HttpGetResponse.h
        response_handlers/HttpHeadResponse.cpp response_handlers/HttpHeadResponse.h
        transport/TCPTransportManager.cpp transport/TCPTransportManager.h
        http_methods/IHttpMethod.h
        response_handlers/IHttpResponse.h)
add_executable(HttpClient ${SOURCE_FILES})