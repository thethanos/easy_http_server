cmake_minimum_required(VERSION 3.0.0)

project(tests)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SOURCES)
add_executable(${PROJECT_NAME} ${SOURCES})

add_test(NAME ${PROJECT_NAME} COMMAND tests)

find_package(GTest REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(ehs REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest ehs OpenSSL::SSL Threads::Threads)