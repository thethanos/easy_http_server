#pragma once

#include "http_connection.hpp"

class server
{
public:
    server(uint16_t port = 8080, size_t thread_count = std::thread::hardware_concurrency());
    ~server(){}

public:
    // handle the given path (accepts any callable object)
    void handle(http::verb verb, const std::string& path, std::function<void(const req_type&, resp_type&)> handle_func);

    // set a new router
    void set_router(std::shared_ptr<abstract_router>& router_ptr);

    // start accepting connections
    void listen_and_serve();

private:
    net::io_context context;
    tcp::socket socket{context};
    std::shared_ptr<tcp::acceptor> acceptor_ptr;

    size_t thread_count;
    boost::asio::thread_pool threads{thread_count};

    std::shared_ptr<abstract_router> router_ptr;
    
private:
    void accept_next(std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket);
};