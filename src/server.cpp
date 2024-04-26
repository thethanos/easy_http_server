#include "server.hpp"

namespace ehs {

server::server(uint16_t port, size_t thread_count):thread_count(thread_count)
{ 
    acceptor_ptr = std::make_shared<tcp::acceptor>(io_ctx);

    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_ptr->open(endpoint.protocol());
    acceptor_ptr->bind(endpoint);
    acceptor_ptr->set_option(boost::asio::socket_base::reuse_address(true));

    router_ptr = std::make_shared<default_router>();
}

server::server(ssl::context&& ssl_ctx, uint16_t port, size_t thread_count):thread_count(thread_count)
{   
    this->ssl_ctx.emplace(std::move(ssl_ctx));
    acceptor_ptr = std::make_shared<tcp::acceptor>(io_ctx);

    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_ptr->open(endpoint.protocol());
    acceptor_ptr->bind(endpoint);
    acceptor_ptr->set_option(boost::asio::socket_base::reuse_address(true));

    router_ptr = std::make_shared<default_router>();
}

void server::handle(http::verb verb, const std::string& path, const route_handler& handle_func)
{
    router_ptr->register_handler(verb, path, handle_func);
}

void server::set_router(std::shared_ptr<abstract_router> router_ptr)
{
    this->router_ptr = router_ptr;
}

void server::listen_and_serve(const std::stop_token& token)
{
    acceptor_ptr->listen();
    accept_next(token, acceptor_ptr, socket);

    for (size_t i(0); i < thread_count; ++i)
        boost::asio::post(threads, [this](){ this->io_ctx.run();});

    boost::asio::post(std::bind(&server::wait_for_stop_token, this, token));
    threads.join();
}

void server::listen_and_serve_secure(const std::stop_token& token)
{
    acceptor_ptr->listen();
    accept_next_secure(token, acceptor_ptr, socket);

    for (size_t i(0); i < thread_count; ++i)
        boost::asio::post(threads, [this](){ this->io_ctx.run(); });
	
    boost::asio::post(std::bind(&server::wait_for_stop_token, this, token));
    threads.join();
}

void server::wait_for_stop_token(const std::stop_token& token)
{
    if (token.stop_requested())
    {
        io_ctx.stop();
        return;
    }
    
    boost::asio::post(std::bind(&server::wait_for_stop_token, this, token));
}

void server::accept_next(const std::stop_token& token, std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket)
{
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {

        if (token.stop_requested())
            return;

        if (!error)
            std::make_shared<connection>(std::move(socket), router_ptr, deadline)->start();
        accept_next(token, acceptor_ptr, socket);
    });
}

void server::accept_next_secure(const std::stop_token& token, std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket)
{
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {

        if (token.stop_requested())
            return;

        if (!error)
            std::make_shared<secure::connection_secure>(ssl_ctx.value(), std::move(socket), router_ptr, deadline)->start();
        accept_next_secure(token, acceptor_ptr, socket);
    });
}

}