cmake_minimum_required(VERSION 3.0.0)

project(tests)

add_executable(tests tests.cpp)

add_test(NAME tests COMMAND tests)

target_link_libraries(tests PRIVATE ehs gtest)