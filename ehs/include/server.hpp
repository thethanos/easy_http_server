#pragma once

#include "connection.hpp"
#include "connection_secure.hpp"

class server
{
public:
    server(uint16_t port = 8080, size_t thread_count = std::thread::hardware_concurrency());
    server(ssl::context&& ssl_ctx, uint16_t port = 8080, size_t thread_count = std::thread::hardware_concurrency());
    ~server(){}

public:
    // handle the given path (accepts any callable objects)
    void handle(http::verb verb, const std::string& path, const route_handler& handle_func);

    // set a new router
    void set_router(std::shared_ptr<abstract_router> router_ptr);

    // set deadline (relatively to now())
    void set_deadline(const int64_t& deadline) { this->deadline = deadline; }

    // start accepting http connections
    void listen_and_serve(const std::stop_token& token);

    // start accepting https connections
    void listen_and_serve_secure(const std::stop_token& token);

private:
    net::io_context io_ctx;
    tcp::socket socket{io_ctx};
    std::shared_ptr<tcp::acceptor> acceptor_ptr;

    size_t thread_count;
    boost::asio::thread_pool threads{thread_count};

    std::shared_ptr<abstract_router> router_ptr;
    
    int64_t deadline = 60;

    ssl::context ssl_ctx{ssl::context::sslv23};

private:
    void wait_for_stop_token(const std::stop_token& token);
    void accept_next(const std::stop_token& token, std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket);
    void accept_next_secure(const std::stop_token& token, std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket);
};