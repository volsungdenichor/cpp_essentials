#include <catch.hpp>
#include <cpp_essentials/sq/filter.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("take_if")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::take_if([](auto&& x) { return x % 3 == 0; })) == vec(3, 6));
}

TEST_CASE("drop_if")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::drop_if([](auto&& x) { return x % 3 == 0; })) == vec(1, 2, 4, 5));
}

TEST_CASE("take_while")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::take_while([](auto&& x) { return x < 4; })) == vec(1, 2, 3));
}


TEST_CASE("drop_while")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::drop_while([](auto&& x) { return x < 4; })) == vec(4, 5, 6));
}

TEST_CASE("take_until")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::take_until([](auto&& x) { return x == 4; })) == vec(1, 2, 3));
}


TEST_CASE("drop_until")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    REQUIRE((vect | sq::drop_until([](auto&& x) { return x == 4; })) == vec(4, 5, 6));
}
