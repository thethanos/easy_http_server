FROM ubuntu:20.04

RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt install -y gcc-11 g++ cmake libboost-all-dev libssl-dev git

RUN git clone --recurse-submodules https://github.com/thethanos/easy_http_server.git && \
    cd easy_http_server && mkdir build && cd build && cmake ..