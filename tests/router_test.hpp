#pragma once

#include "router.hpp"

#include <gtest/gtest.h>

const unsigned int HTTP_VERSION = 11;

class RouterTest : public ::testing::Test 
{
protected:
    void SetUp() override
    {
        router_ptr = std::make_shared<default_router>();
        
        register_get_handlers();
        register_post_handlers();
    }

    void register_get_handlers()
    {
        router_ptr->register_handler(http::verb::get, "/", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << request.target();
        });

        router_ptr->register_handler(http::verb::get, "/api/v1/test/{id}", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << request.target();
        });

        router_ptr->register_handler(http::verb::get, "/api/v1/test/", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << request.target();
        });

        router_ptr->register_handler(http::verb::get, "/api/v1/test/{id}/test", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << request.target();
        });

        router_ptr->register_handler(http::verb::get, "/api/v1/test/test", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << request.target();
        });
    }

    void register_post_handlers()
    {
        router_ptr->register_handler(http::verb::post, "/", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << beast::buffers_to_string(request.body().data());
        });

        router_ptr->register_handler(http::verb::post, "/api/v1/test/{id}", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << beast::buffers_to_string(request.body().data());
        });

        router_ptr->register_handler(http::verb::post, "/api/v1/test/", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << beast::buffers_to_string(request.body().data());
        });

        router_ptr->register_handler(http::verb::post, "/api/v1/test/{id}/test", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << beast::buffers_to_string(request.body().data());
        });

        router_ptr->register_handler(http::verb::post, "/api/v1/test/test", [](const req_type& request, resp_type& response){
            beast::ostream(response.body()) << beast::buffers_to_string(request.body().data());
        });
    }

    std::shared_ptr<abstract_router> router_ptr;
};

TEST_F(RouterTest, process_get_request)
{   
    req_type req1(http::verb::get, "/", HTTP_VERSION);
    req_type req2(http::verb::get, "/api/v1/test/123", HTTP_VERSION);
    req_type req3(http::verb::get, "/api/v1/test/", HTTP_VERSION);
    req_type req4(http::verb::get, "/api/v1/test/123/test", HTTP_VERSION);
    req_type req5(http::verb::get, "/api/v1/test/test?arg1=123&arg2=456", HTTP_VERSION);
    req_type req6(http::verb::get, "/asfasfasfsd", HTTP_VERSION);

    resp_type resp1;
    router_ptr->process_request(req1, resp1);
    ASSERT_EQ(std::string{req1.target()}, beast::buffers_to_string(resp1.body().data()));
    
    resp_type resp2;
    router_ptr->process_request(req2, resp2);
    ASSERT_EQ(std::string{req2.target()}, beast::buffers_to_string(resp2.body().data()));

    resp_type resp3;
    router_ptr->process_request(req3, resp3);
    ASSERT_EQ(std::string{req3.target()}, beast::buffers_to_string(resp3.body().data()));

    resp_type resp4;
    router_ptr->process_request(req4, resp4);
    ASSERT_EQ(std::string{req4.target()}, beast::buffers_to_string(resp4.body().data()));

    resp_type resp5;
    router_ptr->process_request(req5, resp5);
    ASSERT_EQ(std::string{req5.target()}, beast::buffers_to_string(resp5.body().data()));

    resp_type resp6;
    router_ptr->process_request(req6, resp6);
    ASSERT_EQ("Not found\n", beast::buffers_to_string(resp6.body().data()));
}

TEST_F(RouterTest, process_post_request)
{   
    req_type req1(http::verb::post, "/", HTTP_VERSION);
    req_type req2(http::verb::post, "/api/v1/test/123", HTTP_VERSION);
    req_type req3(http::verb::post, "/api/v1/test/", HTTP_VERSION);
    req_type req4(http::verb::post, "/api/v1/test/123/test", HTTP_VERSION);
    req_type req5(http::verb::post, "/api/v1/test/test?arg1=123&arg2=456", HTTP_VERSION);

    resp_type resp1;
    router_ptr->process_request(req1, resp1);
    ASSERT_EQ(beast::buffers_to_string(req1.body().data()), beast::buffers_to_string(resp1.body().data()));
    
    resp_type resp2;
    router_ptr->process_request(req2, resp2);
    ASSERT_EQ(beast::buffers_to_string(req2.body().data()), beast::buffers_to_string(resp2.body().data()));

    resp_type resp3;
    router_ptr->process_request(req3, resp3);
    ASSERT_EQ(beast::buffers_to_string(req3.body().data()), beast::buffers_to_string(resp3.body().data()));

    resp_type resp4;
    router_ptr->process_request(req4, resp4);
    ASSERT_EQ(beast::buffers_to_string(req4.body().data()), beast::buffers_to_string(resp4.body().data()));

    resp_type resp5;
    router_ptr->process_request(req5, resp5);
    ASSERT_EQ(beast::buffers_to_string(req5.body().data()), beast::buffers_to_string(resp5.body().data()));
}
