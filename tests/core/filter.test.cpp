#include <catch.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/trim.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("take_if")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::take_if([](auto&& x) { return x % 3 == 0; })) == vec(3, 6));
}

TEST_CASE("drop_if")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::drop_if([](auto&& x) { return x % 3 == 0; })) == vec(1, 2, 4, 5));
}

TEST_CASE("take_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::take_while([](auto&& x) { return x < 4; })) == vec(1, 2, 3));
}


TEST_CASE("drop_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::drop_while([](auto&& x) { return x < 4; })) == vec(4, 5, 6));
}

TEST_CASE("take_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::take_until([](auto&& x) { return x == 4; })) == vec(1, 2, 3));
}


TEST_CASE("drop_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::drop_until([](auto&& x) { return x == 4; })) == vec(4, 5, 6));
}

TEST_CASE("take_back_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::take_back_while([](auto&& x) { return x >= 4; })) == vec(4, 5, 6));
}

TEST_CASE("drop_back_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::drop_back_while([](auto&& x) { return x >= 4; })) == vec(1, 2, 3));
}

TEST_CASE("take_back_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::take_back_until([](auto&& x) { return x == 4; })) == vec(5, 6));
}

TEST_CASE("drop_back_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | core::drop_back_until([](auto&& x) { return x == 4; })) == vec(1, 2, 3, 4));
}
