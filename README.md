## Dependencies (See Dockerfile for more information):
> Boost, OpenSSL

## Example (no TLS):
```
#include "server.hpp"

class Hanlder
{
public:
    void get(const req_type& request, resp_type& response) { beast::ostream(response.body()) << data << '\n'; }
    void post(const req_type& request, resp_type& response) { data = beast::buffers_to_string(request.body().data()); }
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

## Example (TLS):
```
#include "server.hpp"

class Hanlder
{
public:
    void get(const req_type& request, resp_type& response) { beast::ostream(response.body()) << data << '\n'; }
    void post(const req_type& request, resp_type& response) { data = beast::buffers_to_string(request.body().data()); }
private:
    std::string data;
};

int main(int argc, char* argv[])
{
    Hanlder test;

    ssl::context ssl_ctx(ssl::context::tlsv13_server);
    ssl_ctx.set_options(ssl::context::default_workarounds | ssl::context::single_dh_use);
    ssl_ctx.use_certificate_chain_file("/root/tls/intermediate/certs/ca-chain-bundle.pem");
    ssl_ctx.use_private_key_file("/root/tls/intermediate/private/intermediate.cakey.pem", ssl::context::pem);
    ssl_ctx.use_tmp_dh_file("/root/tls/dhparams.pem");

    server srv(std::move(ssl_ctx), 443);
    srv.handle(http::verb::get, "/test", std::bind(&Hanlder::get, &test, std::placeholders::_1, std::placeholders::_2));
    srv.handle(http::verb::post, "/test", std::bind(&Hanlder::post, &test, std::placeholders::_1, std::placeholders::_2));
    srv.listen_and_serve_secure();

    return 0;
}
```
