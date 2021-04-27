cmake_minimum_required(VERSION 3.9.4)
PROJECT(MainProj)

include(ExternalProject)

set(SQLITE_PREFIX ${DEPS_PREFIX}/build/sqlite)
set(SQLITE_LDD_FLAGS "-ldl -lutil")
set(ENV{CC} ${CMAKE_C_COMPILER})
set(ENV{CFLAGS} ${CMAKE_C_FLAGS})
set(ENV{LDFLAGS} ${SQLITE_LDD_FLAGS})

ExternalProject_Add(
    sqlite_pro
    PREFIX ${SQLITE_PREFIX}
    INSTALL_DIR ${DEPS_PREFIX}
    URL http://sqlite.org/2019/sqlite-autoconf-3300100.tar.gz
    DOWNLOAD_DIR ${FILECACHE}
    DOWNLOAD_NAME sqlite-autoconf-3300100.tar.gz
    URL_HASH SHA1=8383f29d53fa1d4383e4c8eb3e087f2ed940a9e0
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND CC=${CMAKE_C_COMPILER} ./configure --prefix=${DEPS_PREFIX}
    BUILD_COMMAND make ${MAKE_FLAGS}
    INSTALL_COMMAND make install
    BUILD_IN_SOURCE 1
    BUILD_BYPRODUCTS ${DEPSLIBDIR}/libsqlite3.a
)
add_library(sqlite_static STATIC IMPORTED)
set_property(TARGET sqlite_static PROPERTY IMPORTED_LOCATION ${DEPSLIBDIR}/libsqlite3.a)
add_dependencies(sqlite_static sqlite_pro)

set(MAIN_PROJECT_SRC_LIST main.cpp schema.cpp)

add_executable(main ${MAIN_PROJECT_SRC_LIST})
add_subdirectory(ron)
target_link_libraries(main ron_static sqlite_static)
