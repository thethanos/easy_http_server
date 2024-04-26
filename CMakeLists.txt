cmake_minimum_required(VERSION 3.0.0)

project(ehs LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)
aux_source_directory(src SOURCES)

add_library(${PROJECT_NAME} ${SOURCES})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "")

find_package(Boost REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE
    Boost_INCLUDE_DIRS
    OPENSSL_INCLUDE_DIR
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${Boost_LIBRARIES}
    OpenSSL::SSL
    Threads::Threads
)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ 
    DESTINATION ${CMAKE_INSTALL_PREFIX}/include/ehs 
    FILES_MATCHING PATTERN *.hpp
)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})
install(EXPORT ${PROJECT_NAME} FILE ehs-config.cmake DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/${PROJECT_NAME})