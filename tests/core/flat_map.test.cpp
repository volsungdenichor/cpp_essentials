#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::flat_map")
{
    std::vector<std::string> vect = { "1", "2", "x", "4", "y" };

    REQUIRE((vect | sq::flat_map(core::try_parse<int>)) == vec(1, 2, 4));
}
