# easy_http_server
A small library that simplifies creation of http servers with C++ 

## Dependencies:
> Boost (See Dockerfile for more information)

## Example:
```
#include "server.hpp"

class Hanlder
{
public:
  	Hanlder(){}

public:
  	void get(const req_type& request, resp_type& response)
  	{
		beast::ostream(response.body()) << data << '\n';
  	}

  	void post(const req_type& request, resp_type& response)
  	{
		data = beast::buffers_to_string(request.body().data());
	}

private:
	std::string data;
};

int main(int argc, char* argv[])
{
    Hanlder test;

    server srv(8080);
    srv.handle(http::verb::get, "/test", std::bind(&Hanlder::get, &test, std::placeholders::_1, std::placeholders::_2));
    srv.handle(http::verb::post, "/test", std::bind(&Hanlder::post, &test, std::placeholders::_1, std::placeholders::_2));
    srv.listen_and_serve();

    return 0;
}
```