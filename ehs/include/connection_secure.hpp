#pragma once

#include "router.hpp"
#include <boost/asio/ssl.hpp>

#include <chrono>
#include <memory>

namespace net = boost::asio;
namespace ssl = boost::asio::ssl;

using tcp = boost::asio::ip::tcp;

class connection_secure : public std::enable_shared_from_this<connection_secure>
{
public:
    connection_secure(ssl::context& ssl_ctx, tcp::socket&& socket, std::shared_ptr<abstract_router> router_ptr, int64_t deadline);

public:
    // start processing the connection
    void start()
    {
        handshake();
        check_deadline();
    }

private:
    void handshake();
    void read_request();
    void write_response();
    void check_deadline();

private:
    ssl::stream<tcp::socket> socket;
    net::steady_timer timer{socket.get_executor(), std::chrono::seconds(deadline)};

    req_type request;
    resp_type response;
    beast::flat_buffer buffer;

    std::shared_ptr<abstract_router> router_ptr;

    int64_t deadline;
};