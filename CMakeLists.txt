cmake_minimum_required(VERSION 3.9.4)
PROJECT(MainProj)

add_definitions(-DSQLITE_ENABLE_PREUPDATE_HOOK)
set(PROJECT_SOURCES_LIST
        schema.cpp
        hooks.cpp
        column_types.cpp
        log.cpp
        lww.cpp
        sqlite_statements.cpp
        version_vector.cpp
        merge.cpp)

add_subdirectory(ron)
add_subdirectory(sqlite)

add_executable(test test.cpp ${PROJECT_SOURCES_LIST})
target_link_libraries(test ron_static sqlite)

add_executable(server server_main.cpp tcp_server.cpp ${PROJECT_SOURCES_LIST})
target_link_libraries(server ron_static sqlite)

add_executable(client client_main.cpp tcp_client.cpp ${PROJECT_SOURCES_LIST})
target_link_libraries(client ron_static sqlite)
