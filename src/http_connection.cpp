#include "http_connection.hpp"

void http_connection::read_request()
{
    auto self = shared_from_this();

    http::async_read(socket, buffer, request, [self](beast::error_code ec, size_t bytes){
      boost::ignore_unused(bytes);
      if (!ec)
        self->router_ptr->process_request(self->request, self->response);
        self->write_response();
    });
}

void http_connection::write_response() 
{
    auto self = shared_from_this();

    response.content_length(response.body().size());

    http::async_write(socket, response, [self](beast::error_code ec, size_t size){
      self->socket.shutdown(tcp::socket::shutdown_send, ec);
      self->deadline.cancel();
    });
}

void http_connection::check_deadline()
{
    auto self = shared_from_this();

    deadline.async_wait([self](beast::error_code ec){
      if (!ec)
        self->socket.close(ec);
    });
}