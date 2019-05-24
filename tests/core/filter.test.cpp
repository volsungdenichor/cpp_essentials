#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::take_if")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::take_if([](auto&& x) { return x % 3 == 0; })) == vec(3, 6));
}

TEST_CASE("views::drop_if")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::drop_if([](auto&& x) { return x % 3 == 0; })) == vec(1, 2, 4, 5));
}

TEST_CASE("views::take_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::take_while([](auto&& x) { return x < 4; })) == vec(1, 2, 3));
}


TEST_CASE("views::drop_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::drop_while([](auto&& x) { return x < 4; })) == vec(4, 5, 6));
}

TEST_CASE("views::take_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::take_until([](auto&& x) { return x == 4; })) == vec(1, 2, 3));
}


TEST_CASE("views::drop_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::drop_until([](auto&& x) { return x == 4; })) == vec(4, 5, 6));
}

TEST_CASE("views::take_back_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::take_back_while([](auto&& x) { return x >= 4; })) == vec(4, 5, 6));
}

TEST_CASE("views::drop_back_while")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::drop_back_while([](auto&& x) { return x >= 4; })) == vec(1, 2, 3));
}

TEST_CASE("views::take_back_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::take_back_until([](auto&& x) { return x == 4; })) == vec(5, 6));
}

TEST_CASE("views::drop_back_until")
{
    auto vect = vec(1, 2, 3, 4, 5, 6);
    REQUIRE((vect | sq::drop_back_until([](auto&& x) { return x == 4; })) == vec(1, 2, 3, 4));
}

TEST_CASE("views::trim_while")
{
    auto vect = vec(1, 1, 1, 2, 3, 4, 5, 1, 1);
    REQUIRE((vect | sq::trim_while([](auto&& x) { return x < 2; })) == vec(2, 3, 4, 5));
}

TEST_CASE("views::trim_until")
{
    auto vect = vec(1, 1, 1, 2, 3, 4, 5, 2, 1, 1);
    REQUIRE((vect | sq::trim_until([](auto&& x) { return x == 2; })) == vec(2, 3, 4, 5, 2));
}

