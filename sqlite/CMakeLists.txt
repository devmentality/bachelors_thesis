cmake_minimum_required(VERSION 3.9.4)
project(SqliteLibrary)

set(SQLITE_SOURCES sqlite3.c)
set(SQLITE_HEADERS sqlite3.h)
add_compile_options(-ldl -pthread)
add_library(sqlite ${SQLITE_SOURCES} ${SQLITE_HEADERS})
target_include_directories(sqlite PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})