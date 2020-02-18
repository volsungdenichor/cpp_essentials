#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>
#include <random>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

struct Test
{
    bool active;
};

bool operator ==(const Test& lhs, const Test& rhs)
{
    return lhs.active == rhs.active;
}

TEST_CASE("accumulate")
{
    REQUIRE(core::accumulate(vec(1, 2, 3, 4), 1, std::multiplies<>{}) == 24);
    REQUIRE((vec(1, 2, 3, 4) | sq::accumulate(1, std::multiplies<>{})) == 24);
    REQUIRE((vec(1, 2, 3, 4) | sq::accumulate(1, std::multiplies<>{})) == 24);
}

TEST_CASE("adjacent_difference")
{
    REQUIRE(core::adjacent_difference(vec(1, 3, 6, 10), vector_builder<int>{}) == vec(1, 2, 3, 4));
}

TEST_CASE("all_of")
{
    REQUIRE(core::all_of(vec(1, 2, 3), [](auto&& x) { return x < 5; }) == true);
    REQUIRE((vec(1, 2, 3) | sq::all_of([](auto&& x) { return x < 5; })) == true);
    REQUIRE((vec(Test{ true }, Test{ true }, Test{ true }) | sq::all_of(&Test::active)) == true);
}

TEST_CASE("any_of")
{
    REQUIRE(core::any_of(vec(1, 2, 3, 7), [](auto&& x) { return x < 5; }) == true);
    REQUIRE((vec(1, 2, 3, 7) | sq::any_of([](auto&& x) { return x < 5; })) == true);
    REQUIRE((vec(Test{ false }, Test{ true }, Test{ false }) | sq::any_of(&Test::active)) == true);
}

TEST_CASE("copy")
{
    REQUIRE(core::copy(vec(1, 2, 3, 4), vector_builder<int>{}) == vec(1, 2, 3, 4));
    REQUIRE((vec(1, 2, 3, 4) | sq::copy(vector_builder<int>{})) == vec(1, 2, 3, 4));
}

TEST_CASE("copy_if")
{
    REQUIRE(core::copy_if(vec(1, 2, 3, 4), vector_builder<int>{}, [](auto&& x) { return x % 2 == 0; }) == vec(2, 4));
    REQUIRE((vec(1, 2, 3, 4) | sq::copy_if(vector_builder<int>{}, [](auto&& x) { return x % 2 == 0; })) == vec(2, 4));
    REQUIRE((vec(Test{ false }, Test{ true }, Test{ false }) | sq::copy_if(vector_builder<Test>{}, &Test::active)) == vec(Test{ true }));
}

TEST_CASE("copy_n")
{
    REQUIRE(core::copy_n(vec(1, 2, 3, 4, 5), 2, vector_builder<int>{}) == vec(1, 2));
    REQUIRE((vec(1, 2, 3, 4, 5) | sq::copy_n(2, vector_builder<int>{})) == vec(1, 2));
}

TEST_CASE("count")
{
    REQUIRE(core::count(vec(1, 1, 2, 3, 1, 4), 1) == 3);
    REQUIRE((vec(1, 1, 2, 3, 1, 4) | sq::count(1)) == 3);
}

TEST_CASE("count_if")
{
    REQUIRE(core::count_if(vec(1, 1, 2, 3, 1, 4), [](auto&& x) { return x != 1; }) == 3);
    REQUIRE((vec(1, 1, 2, 3, 1, 4) | sq::count_if([](auto&& x) { return x != 1; })) == 3);
    REQUIRE((vec(Test{ false }, Test{ true }, Test{ false }) | sq::count_if(&Test::active)) == 1);
}

TEST_CASE("equal")
{
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3, 4)) == true);
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3)) == false);
    REQUIRE(core::equal(vec(1, 2, 3, 4), vec(1, 2, 3, 4, 5)) == false);
}

TEST_CASE("equal_range")
{
    auto vect = vec(1, 2, 3, 4, 5, 6, 7);
    REQUIRE(core::equal_range(vect, 4) == vec(4));
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

TEST_CASE("find_if")
{
    auto vect = vec(1, 2, 3, 3, 4);
    REQUIRE(core::find_if(vect, [](auto x) { return x > 2; }) == vec(3, 3, 4));
    REQUIRE(core::find_if(vect, [](auto) { return false; }) == vector_builder<int>::empty());

    REQUIRE(core::find_if(core::return_optional, vect, [](auto x) { return x > 2; }) == 3);
    REQUIRE(core::find_if(core::return_optional, vect, [](auto) { return false; }) == core::none);
}

TEST_CASE("find_if_not")
{
    auto vect = vec(1, 2, 3, 3, 4);
    REQUIRE(core::find_if_not(vect, [](auto x) { return x < 3; }) == vec(3, 3, 4));
    REQUIRE(core::find_if_not(core::return_optional, vect, [](auto x) { return x < 2; }) == 2);
    REQUIRE(core::find_if_not(core::return_optional, vect, [](auto) { return true; }) == core::none);
}

TEST_CASE("find_end")
{
    auto vect = vec(3, 4, 1, 2, 3, 3, 4);
    auto o = vec(3, 4);
    REQUIRE(core::find_end(vect, o) == vec(3, 4));
}

TEST_CASE("inner_product")
{
    auto u = vec(3, 4, 1);
    auto v = vec(2.5, 2.5, 2.5);
    REQUIRE(core::inner_product(u, v, 0.0) == 20.0);
}

TEST_CASE("replace")
{
    auto vect = vec(0, 2, 3, 0, 5, 0);
    core::replace(vect, 0, -1);
    REQUIRE(vect == vec(-1, 2, 3, -1, 5, -1));
}

TEST_CASE("replace_if")
{
    auto vect = vec(1, 2, 3, 4, 5, 2);
    core::replace_if(vect, [](auto&& x) { return x < 3; }, 0);
    REQUIRE(vect == vec(0, 0, 3, 4, 5, 0));
}

TEST_CASE("search_n")
{
    auto vect = vec(1, 2, 3, 2, 4, 3, 3, 2, 3, 3, 3, 0);
    auto result = core::search_n(core::return_found, vect, 3, 3);
    REQUIRE(result - vect.begin() == 8);
}

TEST_CASE("shuffle")
{
    auto vect = vec(0, 1, 2, 3, 4);
    core::shuffle(vect, std::default_random_engine(0));
}

TEST_CASE("sort")
{
    auto vect = vec(3, 5, 1, 2, 4, 0);
    core::sort(vect);
    REQUIRE(vect == vec(0, 1, 2, 3, 4, 5));
}

TEST_CASE("lower_bound")
{
    auto vect = vec(1, 2, 3, 4, 5, 6, 7);
    REQUIRE(core::lower_bound(vect, 4) == vec(4, 5, 6, 7));
}

TEST_CASE("upper_bound")
{
    auto vect = vec(1, 2, 3, 4, 5, 6, 7);
    REQUIRE(core::upper_bound(vect, 4) == vec(5, 6, 7));
}
