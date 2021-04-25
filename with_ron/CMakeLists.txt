cmake_minimum_required(VERSION 3.9.4)
PROJECT(MainProj)

set(MAIN_PROJECT_SRC_LIST main.cpp)

add_executable(main ${MAIN_PROJECT_SRC_LIST})
add_subdirectory(ron)
target_link_libraries(main ron_static)
