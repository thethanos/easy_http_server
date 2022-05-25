#include "server.hpp"

server::server(uint16_t port, size_t thread_count):thread_count(thread_count)
{ 
    acceptor_ptr = std::make_shared<tcp::acceptor>(io_ctx);

    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_ptr->open(endpoint.protocol());
    acceptor_ptr->bind(endpoint);
}

server::server(ssl::context&& ssl_ctx, uint16_t port, size_t thread_count):thread_count(thread_count), ssl_ctx(std::move(ssl_ctx))
{
    acceptor_ptr = std::make_shared<tcp::acceptor>(io_ctx);

    tcp::endpoint endpoint(tcp::v4(), port);
    acceptor_ptr->open(endpoint.protocol());
    acceptor_ptr->bind(endpoint);
}

void server::handle(http::verb verb, const std::string& path, std::function<void(const req_type&, resp_type&)> handle_func)
{
    if (!router_ptr)
        router_ptr = std::make_shared<default_router>();

    router_ptr->register_handler(verb, path, handle_func);
}

void server::set_router(std::shared_ptr<abstract_router>& router_ptr)
{
    this->router_ptr = router_ptr;
}

void server::listen_and_serve()
{
    acceptor_ptr->listen();
    
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {
        if (!error)
            std::make_shared<connection>(std::move(socket), router_ptr, deadline)->start();
        accept_next(acceptor_ptr, socket);
    });

    for (size_t i(0); i < thread_count; ++i)
        boost::asio::post(threads, [this](){ this->io_ctx.run();});

    threads.join();
}

void server::listen_and_serve_secure()
{
    acceptor_ptr->listen();
    
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {
        if (!error)
            std::make_shared<connection_secure>(ssl_ctx, std::move(socket), router_ptr, deadline)->start();
        accept_next_secure(acceptor_ptr, socket);
    });

    for (size_t i(0); i < thread_count; ++i)
        boost::asio::post(threads, [this](){ this->io_ctx.run(); });
	
    threads.join();
}

void server::accept_next(std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket)
{
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {
        if (!error)
            std::make_shared<connection>(std::move(socket), router_ptr, deadline)->start();
        accept_next(acceptor_ptr, socket);
    });
}

void server::accept_next_secure(std::shared_ptr<tcp::acceptor>& acceptor_ptr, tcp::socket& socket)
{
    acceptor_ptr->async_accept(socket, [&](beast::error_code error) {
        if (!error)
            std::make_shared<connection_secure>(ssl_ctx, std::move(socket), router_ptr, deadline)->start();
        accept_next_secure(acceptor_ptr, socket);
    });
}