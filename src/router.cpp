#include "router.hpp"

namespace ehs {

void default_router::register_handler(http::verb verb, const std::string& path, const route_handler& handle_func) 
{
    if (!routes[verb])
        routes[verb] = std::make_shared<route_node>("/", std::bind(&default_router::default_handler, this, _1, _2));

    if (path == "/")
    {
        routes[verb]->handler = handle_func;
        routes[verb]->complete = true;
        return;
    }

    auto segments = util::split_string(path, '/');
    util::replace(segments);

    add_node(routes[verb]->nodes, {segments.begin()+1, segments.end()}, 0, handle_func);	
}

void default_router::process_request(const req_type& request, resp_type& response)
{	
    if (!routes[request.method()])
    {
        default_handler(request, response);
        return;
    }

    if (http::get_target_path(request.target()) == "/")
    {   
        routes[request.method()]->handler(request, response);
        return;
    }

    std::string path = http::get_target_path(request.target());

    auto segments = util::split_string(path, '/');

    node_ptr ptr = find_route(routes[request.method()], {segments.begin()+1, segments.end()}, 0);
    if (ptr == nullptr)
    {	
        default_handler(request, response);
        return;
    }

    ptr->handler(request, response);
}

void default_router::add_node(std::vector<node_ptr>& nodes, const std::vector<std::string>& route, int lvl, const route_handler& handle_func)
{
    for (auto& node : nodes)
    {   
        if (node->node_val == route[lvl] && lvl == route.size()-1)
        {
            node->handler = handle_func;
            node->complete = true;
            return;
        }

        if (node->node_val == route[lvl])
        {
            add_node(node->nodes, route, lvl+1, handle_func);
            return;
        }
    }

    if (lvl == route.size()-1)
    {
        nodes.push_back(std::make_shared<route_node>(route[lvl], handle_func, true));
        return;
    }

    auto new_node = std::make_shared<route_node>(route[lvl], std::bind(&default_router::default_handler, this, _1, _2));
    nodes.push_back(new_node);
    
    add_node(new_node->nodes, route, lvl+1, handle_func);
}

default_router::node_ptr default_router::find_route(node_ptr parent, const std::vector<std::string>& route, int lvl)
{
    for (auto& node : parent->nodes)
    {
        if (node->node_val == route[lvl] && lvl == route.size()-1 && node->complete)
            return node;

        if (node->node_val == route[lvl] && lvl != route.size()-1)
            return find_route(node, route, lvl + 1);
    }

    for (auto& node : parent->nodes)
    {	
        if (route[lvl].empty())
            continue;

        std::regex reg(node->node_val);
        if (std::regex_match(route[lvl], reg) && lvl == route.size()-1 && node->complete)
            return node;

        if (std::regex_match(route[lvl], reg) && lvl != route.size()-1)
            return find_route(node, route, lvl + 1);
    }

    return nullptr;
}

}