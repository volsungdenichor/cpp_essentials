#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::zip")
{
    auto vect = vec(2, 4, 5);
    auto other = vec(2, 5, 1, 3);
    REQUIRE((sq::zip(vect, other, std::plus<>{})) == vec(4, 9, 6));
}
