## Dependencies (See Dockerfile for more information):
> Boost, OpenSSL

## How to install
```
mkdir build
cd build
cmake ..
make
make install
```

## How to use in your project:
```
cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(your_project)

add_executable(${PROJECT_NAME} main.cpp)

find_package(OpenSSL REQUIRED)
find_package(Threads REQUIRED)
find_package(ehs REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE ehs OpenSSL::SSL Threads::Threads)
```

## See examples folder
