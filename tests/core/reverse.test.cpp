#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("backwards")
{
    auto vect = vec(2, 4, 5);
    REQUIRE((vect | sq::backwards()) == vec(5, 4, 2));
}