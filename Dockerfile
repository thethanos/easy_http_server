FROM ubuntu:20.04

RUN apt update && \
    apt install -y software-properties-common && \
    add-apt-repository -y ppa:ubuntu-toolchain-r/test && \
    apt install -y g++-11 cmake libboost-all-dev libssl-dev git && \
    update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 100

RUN git clone --recurse-submodules https://github.com/thethanos/easy_http_server.git && \
    cd easy_http_server && mkdir build && cd build && cmake ..