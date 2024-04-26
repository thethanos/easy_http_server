FROM ubuntu:23.04

RUN apt update && \
    apt install -y cmake g++-11 gcc-11 && \
    libboost-all-dev libssl-dev libgtest-dev && \
    cppcheck gdb

ENV CC=/usr/bin/gcc-11
ENV CXX=/usr/bin/g++-11
