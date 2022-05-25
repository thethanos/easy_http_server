#pragma once

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>

#include <map>
#include <string>
#include <functional>

namespace beast = boost::beast;
namespace http = beast::http;

using req_type = http::request<http::dynamic_body>;
using resp_type = http::response<http::dynamic_body>;

class abstract_router
{
public:
    virtual void register_handler(http::verb verb, const std::string& path, std::function<void(const req_type&, resp_type&)> handle_func) = 0;
    virtual void process_request(const req_type& request, resp_type& response) = 0;

protected:
    void default_handler(const req_type& request, resp_type& response) 
    { 
        response.result(http::status::not_found);
        beast::ostream(response.body()) << "Not found\n";
    }

protected:
    using routes = std::map<std::string, std::function<void(const req_type&, resp_type&)>>;

    std::map<http::verb, routes> verbs;
};

class default_router : public abstract_router
{	
public:
    void register_handler(http::verb verb, const std::string& path, std::function<void(const req_type&, resp_type&)> handle_func) override;
    void process_request(const req_type& request, resp_type& response) override;
};