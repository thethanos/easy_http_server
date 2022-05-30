## Dependencies (See Dockerfile for more information):
> Boost, OpenSSL

## Example (no TLS):
```
#include "server.hpp"

class Hanlder
{
public:
    void get(const req_type& request, resp_type& response)
    {
        beast::ostream(response.body()) 
            << "Path: " << http::get_path(request.target().to_string()) 
            << '\n'
            << "ID: " << util::split_string(http::get_path(request.target().to_string()), '/')[2]
            << '\n';
        
        auto query = http::parse_query(http::get_query(request.target().to_string()));

        for (auto& pair : query)
            beast::ostream(response.body()) << "Key: " << pair.first << " value: " << pair.second << '\n';
    }
    void post(const req_type& request, resp_type& response) { data = beast::buffers_to_string(request.body().data()); }

private:
    std::string data;
};

int main(int argc, char* argv[])
{
    Hanlder test;

    server srv(8080);
    srv.handle(http::verb::get, "/test/{id}", std::bind(&Hanlder::get, &test, _1, _2));
    srv.handle(http::verb::post, "/test", std::bind(&Hanlder::post, &test, _1, _2));
    srv.listen_and_serve();

    return 0;
}
```

## Example (TLS):
```
    ssl::context ssl_ctx(ssl::context::tlsv13_server);
    ssl_ctx.set_options(ssl::context::default_workarounds | ssl::context::single_dh_use);
    ssl_ctx.use_certificate_chain_file("/root/tls/intermediate/certs/ca-chain-bundle.pem");
    ssl_ctx.use_private_key_file("/root/tls/intermediate/private/intermediate.cakey.pem", ssl::context::pem);
    ssl_ctx.use_tmp_dh_file("/root/tls/dhparams.pem");

    server srv(std::move(ssl_ctx), 443);
    srv.handle(http::verb::get, "/test/{id}", std::bind(&Hanlder::get, &test, _1, _2));
    srv.handle(http::verb::post, "/test", std::bind(&Hanlder::post, &test, _1, _2));
    srv.listen_and_serve_secure();
```
