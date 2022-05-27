#include "router.hpp"

#include <regex>

default_router::default_router()
{
	node_ptr temp = std::make_shared<route_node>("/", std::bind(&default_router::default_handler, this, _1, _2)); 
	root.swap(temp); 
}

void default_router::register_handler(http::verb verb, const std::string& path, const route_handler& handle_func) 
{
	if (path == "/")
	{
		root->handler = handle_func;
		return;
	}

	std::string replaced;
	std::regex_replace(std::back_inserter(replaced), path.begin(), path.end(), std::regex("\\{\\w*\\}"), "\\w*");

	std::vector<std::string> final_path = split_string(replaced, '/');
	add_node(root, final_path, 0, handle_func);	
}

void default_router::process_request(const req_type& request, resp_type& response)
{	
	if (request.target() == "/")
	{
		root->handler(request, response);
		return;
	}

	std::string path{request.target()};
	std::string replaced;
	std::regex_replace(std::back_inserter(replaced), path.begin(), path.end(), std::regex("\\{\\w*\\}"), "\\w*");

	std::vector<std::string> final_path = split_string(replaced, '/');

	node_ptr ptr = find_route(root, final_path, 0);
	if (ptr == nullptr)
	{	
		default_handler(request, response);
		return;
	}

	ptr->handler(request, response);
}

void default_router::add_node(node_ptr& parent, const std::vector<std::string>& route, int lvl, const route_handler& handle_func)
{
	for (auto& node : parent->nodes)
	{
		if (node->node_val == route[lvl])
		{
			add_node(node, route, lvl+1, handle_func);
			return;
		}
	}

	auto new_node = std::make_shared<route_node>(route[lvl], handle_func);
	parent->nodes.push_back(new_node);
	if (lvl < route.size()-1)
		add_node(new_node, route, lvl+1, handle_func);
}

default_router::node_ptr default_router::find_route(node_ptr parent, const std::vector<std::string>& route, int lvl)
{
	for (auto& node : parent->nodes)
	{
		if (node->node_val == route[lvl] && lvl == route.size()-1)
			return node;

		if (node->node_val == route[lvl])
			return find_route(node, route, lvl + 1);
	}

	for (auto& node : parent->nodes)
	{	
		std::regex reg(node->node_val);
		if (std::regex_match(route[lvl], reg) && lvl == route.size()-1)
			return node;

		if (std::regex_match(route[lvl], reg))
			return find_route(node, route, lvl + 1);
	}

	return nullptr;
}