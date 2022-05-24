#pragma once

#include "router.hpp"

#include <chrono>
#include <memory>

namespace net = boost::asio;

using tcp = boost::asio::ip::tcp;

class http_connection : public std::enable_shared_from_this<http_connection>
{
public:
  http_connection(tcp::socket socket, std::shared_ptr<abstract_router> router_ptr):socket(std::move(socket)), router_ptr(router_ptr) {}

public:
  // start processing the connection
  void start()
  {
    read_request();
    check_deadline();
  }

private:
  void read_request();
  void write_response();
  void check_deadline();

private:
  tcp::socket socket;
  net::steady_timer deadline{socket.get_executor(), std::chrono::seconds(60)};

  req_type request;
  resp_type response;
  beast::flat_buffer buffer;

  std::shared_ptr<abstract_router> router_ptr;
};