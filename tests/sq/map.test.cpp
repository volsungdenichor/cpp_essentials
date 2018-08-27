#include <catch.hpp>
#include <cpp_essentials/sq/map.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("map-function")
{
    std::vector<int> vect{ 2, 4, 5 };
    std::vector<int> result = sq::map(vect, [](auto&& x) { return 10 * x + 1; });
    REQUIRE(result == vec(21, 41, 51));
}

TEST_CASE("map-adaptor")
{
    std::vector<int> vect{ 2, 4, 5 };
    std::vector<int> result = vect | sq::map([](auto&& x) { return 10 * x + 1; });
    REQUIRE(result == vec(21, 41, 51));
}
