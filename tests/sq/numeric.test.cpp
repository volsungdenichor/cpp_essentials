#include <catch.hpp>
#include <cpp_essentials/sq/numeric.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("range")
{
    REQUIRE(sq::range(2, 5) == vec(2, 3, 4));
    REQUIRE(sq::range(5) == vec(0, 1, 2, 3, 4));
    REQUIRE(sq::range(5, 2) == vec(5, 4, 3));
    REQUIRE(sq::range(-2) == vec(0, -1));
}

TEST_CASE("inclusive_range")
{
    REQUIRE(sq::inclusive_range(2, 5) == vec(2, 3, 4, 5));
    REQUIRE(sq::inclusive_range(5) == vec(0, 1, 2, 3, 4, 5));
    REQUIRE(sq::inclusive_range(5, 2) == vec(5, 4, 3, 2));
    REQUIRE(sq::inclusive_range(-2) == vec(0, -1, -2));
}
