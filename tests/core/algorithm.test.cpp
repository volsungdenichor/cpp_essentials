#include <catch.hpp>
#include <cpp_essentials/core/algorithm.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("accumulate")
{
    REQUIRE(core::accumulate(vec(1, 2, 3, 4), 1, std::multiplies<>{}) == 24);
}

TEST_CASE("adjacent_difference")
{
    REQUIRE(core::adjacent_difference(vec(1, 3, 6, 10), vector_builder<int>{}) == vec(1, 2, 3, 4));
}

TEST_CASE("all_of")
{
    REQUIRE(core::all_of(vec(1, 2, 3), [](auto&& x) { return x < 5; }) == true);
}

TEST_CASE("any_of")
{
    REQUIRE(core::any_of(vec(1, 2, 3, 7), [](auto&& x) { return x < 5; }) == true);
}

TEST_CASE("contains")
{
    REQUIRE(core::contains(vec(1, 2, 3, 4, 5, 5, 6), vec(3, 4, 5)) == true);
}

TEST_CASE("copy")
{
    REQUIRE(core::copy(vec(1, 2, 3, 4), vector_builder<int>{}) == vec(1, 2, 3, 4));
}

TEST_CASE("copy_if")
{
    REQUIRE(core::copy_if(vec(1, 2, 3, 4), vector_builder<int>{}, [](auto&& x) { return x % 2 == 0; }) == vec(2, 4));
}

TEST_CASE("copy_n")
{
    REQUIRE(core::copy_n(vec(1, 2, 3, 4, 5), 2, vector_builder<int>{}) == vec(1, 2));
}

TEST_CASE("copy_while")
{
    REQUIRE(core::copy_while(vec(1, 2, 3, 4, 5), vector_builder<int>{}, [](auto&& x) { return x < 3; }) == vec(1, 2));
}

TEST_CASE("copy_until")
{
    REQUIRE(core::copy_until(vec(1, 2, 3, 4, 5), vector_builder<int>{}, [](auto&& x) { return x == 4; }) == vec(1, 2, 3));
}

TEST_CASE("count")
{
    REQUIRE(core::count(vec(1, 1, 2, 3, 1, 4), 1) == 3);
}

TEST_CASE("count_if")
{
    REQUIRE(core::count_if(vec(1, 1, 2, 3, 1, 4), [](auto&& x) { return x != 1; }) == 3);
}

TEST_CASE("ends_with")
{
    REQUIRE(core::ends_with(vec(1, 2, 3, 4, 5, 5, 6), vec(5, 6)) == true);
}

TEST_CASE("equal")
{
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3, 4)) == true);
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3)) == false);
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3, 4, 5)) == false);
}

TEST_CASE("equal_range")
{
    // TODO
}

TEST_CASE("fill")
{
    std::vector<int> result(3);
    core::fill(result, 42);
    REQUIRE(result == vec(42, 42, 42));
}

TEST_CASE("find")
{
    auto vect = vec(1, 2, 3, 3, 4);
    REQUIRE(core::find(vect, 3) == vec(3, 3, 4));
    REQUIRE(core::find(vect, 5) == vector_builder<int>::empty());
    
    REQUIRE(core::find(core::return_optional, vect, 3) == 3);
    REQUIRE(core::find(core::return_optional, vect, 5) == core::none);
}