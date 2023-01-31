#
# Copyright (c) 2016-2017 Vinnie Falco (vinnie dot falco at gmail dot com)
#
# Distributed under the Boost Software License, Version 1.0. (See accompanying
# file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#
# Official repository: https://github.com/boostorg/beast
#

cmake_minimum_required(VERSION 3.1...3.16)
project(httpserver_ssl VERSION 1.0)


find_package(Boost COMPONENTS thread system REQUIRED)
find_package(OpenSSL)

if (UNIX)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_FLAGS_RELEASE "-lboost_system -lboost_thread -lpthread -pthread")
endif (UNIX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()




if (WIN32)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif (WIN32)

add_subdirectory(library)
add_executable(test example/main.cpp)



option(USE_SSL "Use no TLS" ON) #OFF by default
if(USE_SSL)
add_library(httpserver_ssl STATIC src/httpserver_ssl.cpp)
target_include_directories(httpserver_ssl PUBLIC include/ssl)
target_include_directories(httpserver_ssl PUBLIC include)
target_link_libraries (httpserver_ssl
        OpenSSL::SSL OpenSSL::Crypto sqlframe jsonframe baseframe
   #     BOOST
      #  lib-asio
       # lib-asio-ssl
       # lib-beast
        )

 #target_include_directories(test Public include/ssl)
else()
 add_library(httpserver_ssl INTERFACE)
 target_link_libraries (httpserver_ssl INTERFACE sqlframe jsonframe baseframe OpenSSL::SSL OpenSSL::Crypto)
 target_include_directories(httpserver_ssl INTERFACE include/http)
 #target_include_directories(httpserver_ssl INTERFACE library/dataframe/include/sqlframe)
 #target_include_directories(test INTERFACE include/http)
 target_include_directories(httpserver_ssl INTERFACE include)

endif(USE_SSL)
unset(USE_SSL) # <---- this is the important!!




target_link_libraries (test httpserver_ssl)
