#include <catch.hpp>
#include <cpp_essentials/core/container_helpers.hpp>

#include <../tests/test_helpers.hpp>
#include <vector>

using namespace cpp_essentials;

TEST_CASE("insert")
{    
    std::vector<int> vect = { 2, 4, 5 };
    std::vector<int> o = { 9, 8 };
    core::insert(vect, vect.end(), o);
    REQUIRE(vect == vec(2, 4, 5, 9, 8));
}

TEST_CASE("push_front")
{
    std::vector<int> vect = { 2, 4, 5 };
    std::vector<int> o = { 9, 8 };
    core::push_front(vect, o);
    REQUIRE(vect == vec(9, 8, 2, 4, 5));
}

TEST_CASE("push_back")
{
    std::vector<int> vect = { 2, 4, 5 };
    std::vector<int> o = { 9, 8 };
    core::push_back(vect, o);
    REQUIRE(vect == vec(2, 4, 5, 9, 8));
}

TEST_CASE("erase")
{
    std::vector<int> vect = { 2, 4, 5, 3, 6 };
    core::erase(vect, core::make_range(vect.begin() + 1, vect.begin() + 4));
    REQUIRE(vect == vec(2, 6));
}

TEST_CASE("erase_if")
{
    std::vector<int> vect = { 2, 4, 5, 3, 6 };
    core::erase_if(vect, [](auto&& x) { return x % 2 != 0; });
    REQUIRE(vect == vec(2, 4, 6));
}
