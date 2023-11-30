cmake_minimum_required(VERSION 3.22)
project(TWMailerBasic)

set(CMAKE_CXX_STANDARD 20)

add_executable(TWMailerClient twmailer-client.cpp
        test.cpp)

add_executable(TWMailerServer twmailer-server.cpp)


