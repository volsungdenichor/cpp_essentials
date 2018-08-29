#include <catch.hpp>
#include <cpp_essentials/sq/front.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("front")
{
    std::vector<int> vect{ };
    REQUIRE_THROWS(vect | sq::front());
}

TEST_CASE("front_or")
{
    std::vector<int> vect{};
    REQUIRE((vect | sq::front_or(-1)) == -1);
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
