#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("map")
{
    auto vect = vec(2, 4, 5);
    REQUIRE((vect | sq::map([](auto&& x) { return 10 * x + 1; })) == vec(21, 41, 51));
}

TEST_CASE("map field")
{
    struct Test
    {
        int a;
        int b;
    };

    std::vector<Test> vect = {
        { 2, 5 },
        { 3, 1 },
        { 5, 9 },
    };
    
    REQUIRE((vect | sq::map(&Test::a)) == vec(2, 3, 5));
    REQUIRE((vect | sq::map(&Test::b)) == vec(5, 1, 9));
}
