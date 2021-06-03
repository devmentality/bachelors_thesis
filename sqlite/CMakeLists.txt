cmake_minimum_required(VERSION 3.9.4)
project(SqliteLibrary)

include(FindThreads)

set(SQLITE_SOURCES sqlite3.c)
set(SQLITE_HEADERS sqlite3.h)
add_library(sqlite ${SQLITE_SOURCES} ${SQLITE_HEADERS})

target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(sqlite ${CMAKE_DL_LIBS} Threads::Threads)
