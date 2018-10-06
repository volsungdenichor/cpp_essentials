#include <catch.hpp>
#include <cpp_essentials/core/backwards.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("reverse")
{
    auto vect = vec(2, 4, 5);
    REQUIRE((vect | core::backwards()) == vec(5, 4, 2));
}
