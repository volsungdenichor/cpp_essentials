#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("map")
{
    auto vect = vec(2, 4, 5);
    REQUIRE((vect | sq::map([](auto&& x) { return 10 * x + 1; })) == vec(21, 41, 51));
}
