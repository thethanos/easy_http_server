cmake_minimum_required(VERSION 3.0.0)

project(https_example)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(ehs REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE ehs OpenSSL::SSL Threads::Threads)