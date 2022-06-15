cmake_minimum_required(VERSION 3.0.0)

set(OPENSSL_USE_STATIC_LIBS TRUE)

add_library(ehs ${SOURCES})
set_target_properties(ehs PROPERTIES PREFIX "")
target_link_options(ehs PRIVATE -static-libgcc -static-libstdc++)

aux_source_directory(src SOURCES)
target_sources(ehs PRIVATE ${SOURCES})

find_package(Boost REQUIRED)
include_directories(Boost_INCLUDE_DIRS)
target_link_libraries(ehs PRIVATE ${Boost_LIBRARIES})

find_package(OpenSSL REQUIRED)
include_directories(OPENSSL_INCLUDE_DIR)
target_link_libraries(ehs PRIVATE OpenSSL::SSL)

find_package(Threads REQUIRED)
target_link_libraries(ehs PRIVATE Threads::Threads)