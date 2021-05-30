cmake_minimum_required(VERSION 3.9.4)
PROJECT(MainProj)

add_definitions(-DSQLITE_ENABLE_PREUPDATE_HOOK)
set(MAIN_PROJECT_SRC_LIST main.cpp schema.cpp hooks.cpp column_types.cpp log.cpp log.h)

add_executable(main ${MAIN_PROJECT_SRC_LIST})
add_subdirectory(ron)
add_subdirectory(sqlite)
target_link_libraries(main ron_static sqlite)
