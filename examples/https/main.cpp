#include <ehs/server.hpp>

class Hanlder
{
public:
    void get(const req_type& request, resp_type& response)
    {
        beast::ostream(response.body()) 
            << "Path: " << http::get_target_path(request.target()) 
            << '\n'
            << "ID: " << util::split_string(http::get_target_path(request.target()), '/')[2]
            << '\n';
        
        auto query = http::parse_query(http::get_target_query(request.target()));

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
    std::stop_source source;

    ssl::context ssl_ctx(ssl::context::tlsv13_server);
    //ssl_ctx.use_certificate_file("certificate.crt");

    server srv(std::move(ssl_ctx), 443);
    srv.handle(http::verb::get, "/test/{id}", std::bind(&Hanlder::get, &test, _1, _2));
    srv.handle(http::verb::post, "/test", std::bind(&Hanlder::post, &test, _1, _2));
    srv.listen_and_serve_secure(source.get_token());

    return 0;
}