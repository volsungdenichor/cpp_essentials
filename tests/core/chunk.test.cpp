#include <catch.hpp>
#include <cpp_essentials/core/chunk.hpp>
#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/containers.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("chunk")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5, 6 };
    auto result = vect | core::chunk(3, 2) | core::map(core::to_vector()) | core::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2, 3 }, { 3, 4, 5 }, { 5, 6 } });
}

TEST_CASE("slide")
{
    std::vector<int> vect{ 1, 2, 3, 4 };
    auto result = vect | core::slide(3) | core::map(core::to_vector()) | core::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ {1, 2, 3 }, { 2, 3, 4 }, { 3, 4 }, { 4 } });
}

TEST_CASE("group")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5 };
    auto result = vect | core::group(2) | core::map(core::to_vector()) | core::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2 }, { 3, 4 }, { 5 } });
}

TEST_CASE("pairwise")
{
    std::vector<int> vect{ 1, 2, 3, 4, 5 };
    auto result = vect | core::pairwise() | core::map(core::to_vector()) | core::to_vector();
    REQUIRE(result == std::vector<std::vector<int>>{ { 1, 2 }, { 3, 4 }, { 5 } });
}
