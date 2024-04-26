#pragma once

#include "util.hpp"

#include <map>

namespace ehs {

class abstract_router
{
public:
    virtual void register_handler(http::verb verb, const std::string& path, const route_handler& handle_func) = 0;
    virtual void process_request(const req_type& request, resp_type& response) = 0;

protected:
    void default_handler(const req_type& request, resp_type& response) 
    { 
        response.result(http::status::not_found);
        beast::ostream(response.body()) << "Not found\n";
    }
};

class default_router : public abstract_router
{
    struct route_node;
    using node_ptr = std::shared_ptr<route_node>;

    struct route_node
    {	
        route_node(){}
        route_node(const std::string& node_val, route_handler handler, bool complete = false)
            :node_val(node_val), handler(handler), complete(complete){}

        std::string   node_val = "/";
        route_handler handler;
        std::vector<node_ptr> nodes;
        bool complete;
    };

public:
    default_router(){}

public:
    void register_handler(http::verb verb, const std::string& path, const route_handler& handle_func) override;
    void process_request(const req_type& request, resp_type& response) override;

private:
    void     add_node(std::vector<node_ptr>& nodes, const std::vector<std::string>& route, int lvl, const route_handler& handle_func);
    node_ptr find_route(node_ptr parent, const std::vector<std::string>& route, int lvl);

private:
    std::map<http::verb, node_ptr> routes;
};

}