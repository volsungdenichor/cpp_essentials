#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::replace_if")
{
    auto vect = core::range(5) | sq::replace_if([](auto x) { return x % 3 == 0; }, -1);
    REQUIRE(vect == vec(-1, 1, 2, -1, 4));
}

TEST_CASE("views::replace")
{
    auto vect = core::range(5) | sq::replace(3, -1);
    REQUIRE(vect == vec(0, 1, 2, -1, 4));
}
