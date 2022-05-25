#include "connection.hpp"

connection::connection(tcp::socket&& socket, std::shared_ptr<abstract_router> router_ptr, const int64_t& deadline)
    :socket(std::move(socket)), timer(socket.get_executor(), std::chrono::seconds(deadline))
{ 
    this->router_ptr = router_ptr; 
    this->deadline   = deadline;
}

void connection::read_request()
{
    auto self = shared_from_this();

    http::async_read(socket, buffer, request, [self](beast::error_code error, size_t bytes){
        boost::ignore_unused(bytes);
        if (!error)
            self->router_ptr->process_request(self->request, self->response);
        self->write_response();
    });
}

void connection::write_response() 
{
    auto self = shared_from_this();

    response.content_length(response.body().size());

    http::async_write(socket, response, [self](beast::error_code error, size_t size){
        self->socket.shutdown(tcp::socket::shutdown_send, error);
        self->timer.cancel();
    });
}

void connection::check_deadline()
{
    auto self = shared_from_this();

    timer.async_wait([self](beast::error_code error){
        if (!error)
            self->socket.close(error);
    });
}