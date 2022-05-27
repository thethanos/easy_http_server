#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;
namespace http  = beast::http;

using req_type  = http::request<http::dynamic_body>;
using resp_type = http::response<http::dynamic_body>;

using std::placeholders::_1;
using std::placeholders::_2;

using route_handler = std::function<void(const req_type&, resp_type&)>;


inline std::vector<std::string> split_string(const std::string& str, char delim)
{
	std::istringstream istream{str};

	std::vector<std::string> res;
	while (!istream.eof()) 
	{	
		std::string temp;
		std::getline(istream, temp, delim);
		if (!temp.empty())
			res.push_back(temp);
	}

	return res;
}