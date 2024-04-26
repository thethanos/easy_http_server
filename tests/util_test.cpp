#include <ehs/util.hpp>

#include <gtest/gtest.h>

using namespace ehs;

TEST(utility, split_string)
{
    std::vector<std::string> result1{{""}, {"api"}, {"v1"}, {"products"}, {"{id}"}};
    std::vector<std::string> result2{{"param1=123"}, {"param2=456"}};
    std::vector<std::string> result3;

    ASSERT_TRUE(std::equal(result1.begin(), result1.end(), util::split_string("/api/v1/products/{id}", '/').begin()));
    ASSERT_TRUE(std::equal(result2.begin(), result2.end(), util::split_string("param1=123&param2=456", '&').begin()));
    ASSERT_TRUE(std::equal(result3.begin(), result3.end(), util::split_string("", '/').begin()));
}

TEST(utility, replace)
{
    std::vector<std::string> input{{""}, {"api"}, {"v1"}, {"products"}, {"{id}"}};
    std::vector<std::string> result{{""}, {"api"}, {"v1"}, {"products"}, {"\\w*"}};
    
    std::vector<std::string> func_result1(5);
    std::copy(input.begin(), input.end(), func_result1.begin());

    util::replace(func_result1);
    ASSERT_TRUE(std::equal(result.begin(), result.end(), func_result1.begin()));
}

TEST(utility, get_target_path)
{
    ASSERT_EQ(http::get_target_path("/api/v1/products/{id}"), "/api/v1/products/{id}");
    ASSERT_EQ(http::get_target_path("/api/v1/products"), "/api/v1/products");
    ASSERT_EQ(http::get_target_path("/"), "/");
    ASSERT_EQ(http::get_target_path("/api/v1/products/{id}?param1=123&param2=456"), "/api/v1/products/{id}");
    ASSERT_EQ(http::get_target_path("/api/v1/products/{id}?param1=123&param2="), "/api/v1/products/{id}");
    ASSERT_EQ(http::get_target_path("/api/v1/products/{id}?param1=123"), "/api/v1/products/{id}");
}

TEST(utility, get_target_query)
{
    ASSERT_EQ(http::get_target_query("/api/v1/products/{id}"), "");
    ASSERT_EQ(http::get_target_query("/api/v1/products/{id}?param1=123&param2=456"), "param1=123&param2=456");
    ASSERT_EQ(http::get_target_query("/api/v1/products/{id}?param1=123&param2="), "param1=123&param2=");
    ASSERT_EQ(http::get_target_query("/api/v1/products/{id}?param1=123"), "param1=123");
}

TEST(utility, parse_query)
{
    std::map<std::string, std::string> result1{{"param1", "123"},{"param2","456"},{"param3", "789"}};
    std::map<std::string, std::string> result2{{"param1", "123"},{"param2","456"}};
    std::map<std::string, std::string> result3{{"param1", "123"}};
    std::map<std::string, std::string> result4{{"param1", ""}};
    std::map<std::string, std::string> result5;

    ASSERT_EQ(http::parse_query("param1=123&param2=456&param3=789"), result1);
    ASSERT_EQ(http::parse_query("param1=123&param2=456"), result2);
    ASSERT_EQ(http::parse_query("param1=123"), result3);
    ASSERT_EQ(http::parse_query("param1="), result4);
    ASSERT_EQ(http::parse_query("param1"), result4);
    ASSERT_EQ(http::parse_query(""), result5);
}