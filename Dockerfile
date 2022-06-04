FROM ubuntu:20.04

COPY . /easy_http_server

RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt install -y gcc-11 g++ cmake libboost-all-dev libssl-dev

RUN cd easy_http_server && mkdir build && cd build && cmake ..