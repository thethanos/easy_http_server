cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(easy_http_server)

add_library(http_server ${SOURCES})

set_target_properties(http_server PROPERTIES PREFIX "")

include_directories(include)

aux_source_directory(src SOURCES)
target_sources(http_server PRIVATE ${SOURCES})

find_package(Boost REQUIRED)
include_directories(Boost_INCLUDE_DIRS)
target_link_libraries(http_server PRIVATE ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(OPENSSL_INCLUDE_DIR)
target_link_libraries(http_server PRIVATE OpenSSL::SSL)

find_package(Threads REQUIRED)
target_link_libraries(http_server PRIVATE Threads::Threads)