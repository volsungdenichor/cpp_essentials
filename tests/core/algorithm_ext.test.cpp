#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("front")
{
    std::vector<int> vect{};
    REQUIRE_THROWS(vect | sq::front());
}

TEST_CASE("front_or")
{
    std::vector<int> vect{};
    REQUIRE((vect | sq::front_or(-1)) == -1);
}

TEST_CASE("front_or_default")
{
    std::vector<int> vect{};
    REQUIRE((vect | sq::front_or_default()) == 0);
}

TEST_CASE("front_or_eval")
{
    std::vector<int> vect{};
    REQUIRE((vect | sq::front_or_eval([]() { return -1; })) == -1);
}

TEST_CASE("front_or_none")
{
    std::vector<int> vect{};
    REQUIRE((vect | sq::front_or_none()) == core::none);
}

TEST_CASE("single")
{
    REQUIRE_THROWS(std::vector<int>{} | sq::single());
    REQUIRE_THROWS(vec(1, 2) | sq::single());
    REQUIRE((vec(4) | sq::single()) == 4);
}

TEST_CASE("single_or")
{
    REQUIRE((std::vector<int>{} | sq::single_or(-1)) == -1);
    REQUIRE((vec(1, 2) | sq::single_or(-1)) == -1);
    REQUIRE((vec(4) | sq::single_or(-1)) == 4);
}

TEST_CASE("single_or_default")
{
    REQUIRE((std::vector<int>{} | sq::single_or_default()) == 0);
    REQUIRE((vec(1, 2) | sq::single_or_default()) == 0);
    REQUIRE((vec(4) | sq::single_or_default()) == 4);
}

TEST_CASE("single_or_eval")
{
    REQUIRE((std::vector<int>{} | sq::single_or_eval([]() { return -1; })) == -1);
    REQUIRE((vec(1, 2) | sq::single_or_eval([]() { return -1; })) == -1);
    REQUIRE((vec(4) | sq::single_or_eval([]() { return -1; })) == 4);
}

TEST_CASE("single_or_none")
{
    REQUIRE((std::vector<int>{} | sq::single_or_none()) == core::none);
    REQUIRE((vec(1, 2) | sq::single_or_none()) == core::none);
    REQUIRE((vec(4) | sq::single_or_none()) == 4);
}