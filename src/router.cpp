#include "router.hpp"

void default_router::register_handler(http::verb verb, const std::string& path, std::function<void(const req_type&, resp_type&)> handle_func) 
{
	auto& routes = verbs[verb];
	routes[path] = handle_func;
}

void default_router::process_request(const req_type& request, resp_type& response)
{	
	auto routes = verbs[request.method()];
	if (routes.empty())
	{
		default_handler(request, response);
		return;
	}

	auto handler = routes[std::string{request.target()}];
	if (!handler)
	{
		default_handler(request, response);
		return;
	}  
    
	handler(request, response);
}