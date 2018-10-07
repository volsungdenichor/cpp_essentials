#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("chunk")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    auto result = vect | sq::chunk(3, 2) | sq::map(sq::to_vector()) | sq::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2, 3 }, { 3, 4, 5 }, { 5, 6 } });
}

TEST_CASE("slide")
{
    std::vector<int> vect{ 1, 2, 3, 4 };
    auto result = vect | sq::slide(3) | sq::map(sq::to_vector()) | sq::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ {1, 2, 3 }, { 2, 3, 4 }, { 3, 4 }, { 4 } });
}

TEST_CASE("group")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5 };
    auto result = vect | sq::group(2) | sq::map(sq::to_vector()) | sq::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2 }, { 3, 4 }, { 5 } });
}

TEST_CASE("pairwise")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5 };
    auto result = vect | sq::pairwise() | sq::map(sq::to_vector()) | sq::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2 }, { 3, 4 }, { 5 } });
}
