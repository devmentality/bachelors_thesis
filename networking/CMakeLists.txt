cmake_minimum_required(VERSION 3.9.4)
project(Networking)


set(NETWORKING_SOURCES TCPClient.cpp TCPServer.cpp)
set(NETWORKING_HEADERS TCPClient.h TCPServer.h)
add_library(networking ${NETWORKING_SOURCES} ${NETWORKING_HEADERS})

target_include_directories(networking PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(networking)
