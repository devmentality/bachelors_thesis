cmake_minimum_required(VERSION 3.19)
project(networking)

set(CMAKE_CXX_STANDARD 14)

add_executable(client client_main.cpp tcp_client.cpp)
add_executable(server server_main.cpp tcp_server.cpp)