#include <catch.hpp>
#include <cpp_essentials/core/serialization.hpp>

using namespace cpp_essentials;
using namespace std::string_literals;

TEST_CASE("try_parse")
{
    REQUIRE(core::try_parse<int>("42"s) == 42);
    REQUIRE(core::try_parse<int>("42  "s) == 42);
    REQUIRE(core::try_parse<int>("  42  "s) == 42);
    REQUIRE(core::try_parse<int>("   42"s) == 42);

    REQUIRE(core::try_parse<int>("x42"s) == core::none);
    REQUIRE(core::try_parse<int>("42x"s) == 42);
}

TEST_CASE("parse")
{
    REQUIRE(core::parse<int>("42"s) == 42);
    REQUIRE(core::parse<int>("42 "s) == 42);
    REQUIRE(core::parse<int>(" 42 "s) == 42);
    REQUIRE(core::parse<int>(" 42"s) == 42);
    REQUIRE_THROWS(core::parse<int>("x42"s));
}

TEST_CASE("to_string")
{
    REQUIRE(core::to_string(42) == "42");
    REQUIRE(core::to_string(4.2) == "4.2");
}