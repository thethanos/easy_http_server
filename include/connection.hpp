#pragma once

#include "router.hpp"

#include <chrono>
#include <memory>

namespace net = boost::asio;

using tcp = boost::asio::ip::tcp;

class connection : public std::enable_shared_from_this<connection>
{
public:
	connection(tcp::socket&& socket, std::shared_ptr<abstract_router> router_ptr, const int64_t deadline);

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
	net::steady_timer timer;

	req_type request;
	resp_type response;
	beast::flat_buffer buffer;

	std::shared_ptr<abstract_router> router_ptr;

	int64_t deadline;
};