#include <catch.hpp>
#include <cpp_essentials/sq/zip.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("zip")
{
    std::vector<int> vect{ 2, 4, 5 };
    std::vector<int> other = { 2, 5, 1, 3 };
    std::vector<int> result = vect | sq::zip(other, std::plus<>{});
    REQUIRE(result == vec(4, 9, 6));
}
