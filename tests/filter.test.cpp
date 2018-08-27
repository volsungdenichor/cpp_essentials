#include <catch.hpp>
#include <cpp_essentials/sq/filter.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("take_if-function")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    std::vector<int> result = sq::take_if(vect, [](auto&& x) { return x % 3 == 0; });
    REQUIRE(result == vec(3, 6));
}

TEST_CASE("take_if-adaptor")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    std::vector<int> result = vect | sq::take_if([](auto&& x) { return x % 3 == 0; });
    REQUIRE(result == vec(3, 6));
}

TEST_CASE("drop_if-function")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    std::vector<int> result = sq::drop_if(vect, [](auto&& x) { return x % 3 == 0; });
    REQUIRE(result == vec(1, 2, 4, 5));
}

TEST_CASE("drop_if-adaptor")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    std::vector<int> result = vect | sq::drop_if([](auto&& x) { return x % 3 == 0; });
    REQUIRE(result == vec(1, 2, 4, 5));
}
