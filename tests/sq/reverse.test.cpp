#include <catch.hpp>
#include <cpp_essentials/sq/reverse.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("reverse")
{
    std::vector<int> vect{ 2, 4, 5 };
    REQUIRE((vect | sq::reverse()) == vec(5, 4, 2));
}
