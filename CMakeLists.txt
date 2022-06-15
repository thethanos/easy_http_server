cmake_minimum_required(VERSION 3.0.0)

option(BUILD_SHARED_LIBS "Build package with shared libraries." OFF)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(easy_http_server)

include_directories(ehs/include)

add_subdirectory(ehs)
add_subdirectory(tests)
add_subdirectory(deps/googletest)