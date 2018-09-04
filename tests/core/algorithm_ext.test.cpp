#include <catch.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("front")
{
    std::vector<int> vect{};
    REQUIRE_THROWS(vect | core::front());
}

TEST_CASE("front_or")
{
    std::vector<int> vect{};
    REQUIRE((vect | core::front_or(-1)) == -1);
}

TEST_CASE("front_or_default")
{
    std::vector<int> vect{};
    REQUIRE((vect | core::front_or_default()) == 0);
}

TEST_CASE("front_or_eval")
{
    std::vector<int> vect{};
    REQUIRE((vect | core::front_or_eval([]() { return -1; })) == -1);
}

TEST_CASE("front_or_none")
{
    std::vector<int> vect{};
    REQUIRE((vect | core::front_or_none()) == core::none);
}
