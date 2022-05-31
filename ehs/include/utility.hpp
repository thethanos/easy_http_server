#pragma once

#include <regex>
#include <string>
#include <vector>
#include <sstream>
#include <functional>

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

namespace beast = boost::beast;

using std::placeholders::_1;
using std::placeholders::_2;

namespace util
{
	inline std::string parse_target(const std::string& path, bool flag)
	{
		std::cmatch res;
		std::regex expr("(/?[^ #?]*)\\x3f?([^ #]*)#?([^ ]*)");

		std::string result_path, result_query;
		if(std::regex_match(path.c_str(), res, expr)) 
		{
			result_path = std::string(res[1].first, res[1].second);
			result_query = std::string(res[2].first, res[2].second);
		}

		if (flag)
			return result_path;
		else
			return result_query;
	}

	//split string into a vector
	inline std::vector<std::string> split_string(const std::string& str, char delim)
	{
		std::istringstream istream{str};

		std::vector<std::string> res;
		while (!istream.eof()) 
		{	
			std::string temp;
			std::getline(istream, temp, delim);
			res.push_back(temp);
		}

		return res;
	}

	// replace strings matching the given regexp with the given replacement
	inline void replace(std::vector<std::string>& path, const std::string& expr = "\\{\\w*\\}", const std::string& replacement = "\\w*")
	{
		std::regex reg(expr);
		for (auto& segment : path)
		{
			if (std::regex_match(segment, reg))
				segment = replacement;
		}
	}
}

namespace http
{	
	using namespace beast::http;

	// get path from target
	inline auto get_path = std::bind(util::parse_target, _1, true);

	//get query from target
	inline auto get_query = std::bind(util::parse_target, _1, false);

	inline std::map<std::string, std::string> parse_query(const std::string& query)
	{
		auto pairs = util::split_string(query, '&');

		std::map<std::string, std::string> res;
		for (auto& pair : pairs)
		{
			if (pair.empty())
				return res;

			auto temp = util::split_string(pair, '=');
			if (temp.empty())
				return res;

			if (temp.size() < 2)
				res[temp[0]] = "";
			else
				res[temp[0]] = temp[1];
		}

		return res;
	}
}

using req_type  = http::request<http::dynamic_body>;
using resp_type = http::response<http::dynamic_body>;
using route_handler = std::function<void(const req_type&, resp_type&)>;