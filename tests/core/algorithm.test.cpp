#include <catch.hpp>
#include <cpp_essentials/core/algorithm.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("accumulate")
{
    REQUIRE(core::accumulate(vec(1, 2, 3, 4), 1, std::multiplies<>{}) == 24);
    REQUIRE((vec(1, 2, 3, 4) | core::accumulate(1, std::multiplies<>{})) == 24);
}

TEST_CASE("adjacent_difference")
{
    REQUIRE(core::adjacent_difference(vec(1, 3, 6, 10), vector_builder<int>{}) == vec(1, 2, 3, 4));
}

TEST_CASE("all_of")
{
    REQUIRE(core::all_of(vec(1, 2, 3), [](auto&& x) { return x < 5; }) == true);
    REQUIRE((vec(1, 2, 3) | core::all_of([](auto&& x) { return x < 5; })) == true);
}

TEST_CASE("any_of")
{
    REQUIRE(core::any_of(vec(1, 2, 3, 7), [](auto&& x) { return x < 5; }) == true);
    REQUIRE((vec(1, 2, 3, 7) | core::any_of([](auto&& x) { return x < 5; })) == true);
}

TEST_CASE("copy")
{
    REQUIRE(core::copy(vec(1, 2, 3, 4), vector_builder<int>{}) == vec(1, 2, 3, 4));
    REQUIRE((vec(1, 2, 3, 4) | core::copy(vector_builder<int>{})) == vec(1, 2, 3, 4));
}

TEST_CASE("copy_if")
{
    REQUIRE(core::copy_if(vec(1, 2, 3, 4), vector_builder<int>{}, [](auto&& x) { return x % 2 == 0; }) == vec(2, 4));
    REQUIRE((vec(1, 2, 3, 4) | core::copy_if(vector_builder<int>{}, [](auto&& x) { return x % 2 == 0; })) == vec(2, 4));
}

TEST_CASE("copy_n")
{
    REQUIRE(core::copy_n(vec(1, 2, 3, 4, 5), 2, vector_builder<int>{}) == vec(1, 2));
    REQUIRE((vec(1, 2, 3, 4, 5) | core::copy_n(2, vector_builder<int>{})) == vec(1, 2));
}

TEST_CASE("count")
{
    REQUIRE(core::count(vec(1, 1, 2, 3, 1, 4), 1) == 3);
    REQUIRE((vec(1, 1, 2, 3, 1, 4) | core::count(1)) == 3);
}

TEST_CASE("count_if")
{
    REQUIRE(core::count_if(vec(1, 1, 2, 3, 1, 4), [](auto&& x) { return x != 1; }) == 3);
    REQUIRE((vec(1, 1, 2, 3, 1, 4) | core::count_if([](auto&& x) { return x != 1; })) == 3);
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
    
    REQUIRE(core::find.as<core::return_optional>(vect, 3) == 3);
    REQUIRE(core::find.as<core::return_optional>(vect, 5) == core::none);
}

TEST_CASE("find_if")
{
    auto vect = vec(1, 2, 3, 3, 4);
    REQUIRE(core::find_if(vect, [](auto x) { return x > 2; }) == vec(3, 3, 4));
    REQUIRE(core::find_if(vect, [](auto) { return false; }) == vector_builder<int>::empty());

    REQUIRE(core::find_if.as<core::return_optional>(vect, [](auto x) { return x > 2; }) == 3);
    REQUIRE(core::find_if.as<core::return_optional>(vect, [](auto) { return false; }) == core::none);
}

TEST_CASE("find_if_not")
{
    auto vect = vec(1, 2, 3, 3, 4);
    REQUIRE(core::find_if_not(vect, [](auto x) { return x < 3; }) == vec(3, 3, 4));
    REQUIRE(core::find_if_not.as<core::return_optional>(vect, [](auto x) { return x < 2; }) == 2);
    REQUIRE(core::find_if_not.as<core::return_optional>(vect, [](auto) { return true; }) == core::none);
}

TEST_CASE("find_end")
{
    auto vect = vec(3, 4, 1, 2, 3, 3, 4);
    auto o = vec(3, 4);
    REQUIRE(core::find_end(vect, o) == vec(3, 4));

}