#include <catch.hpp>
#include <cpp_essentials/sq/slice.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("slice")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::slice(3, 5)) == vec(7, 3));
}

TEST_CASE("take")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::take(3)) == vec(2, 4, 5));
}

TEST_CASE("drop")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::drop(3)) == vec(7, 3, 1, 2));
}
