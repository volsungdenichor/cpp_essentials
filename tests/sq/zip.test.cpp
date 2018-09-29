#include <catch.hpp>
#include <cpp_essentials/sq/zip.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("zip")
{
    auto vect = vec(2, 4, 5);
    auto other = vec(2, 5, 1, 3);
    REQUIRE((vect | sq::zip(other, std::plus<>{})) == vec(4, 9, 6));
}
