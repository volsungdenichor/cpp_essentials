#include <catch.hpp>
#include <cpp_essentials/geo/interval_operations.hpp>

using namespace cpp_essentials;

TEST_CASE("interval union")
{
    auto a = geo::make_interval(0, 10);
    auto b = geo::make_interval(8, 15);
    REQUIRE(geo::make_union(a, b) == geo::make_interval(0, 15));
}

TEST_CASE("interval intersecion")
{
    auto a = geo::make_interval(0, 10);
    auto b = geo::make_interval(8, 15);
    REQUIRE(geo::make_intersection(a, b) == geo::make_interval(8, 10));
}

TEST_CASE("interval - make_union")
{
    REQUIRE(geo::make_union(geo::make_interval(0, 1), geo::make_interval(4, 5)) == geo::make_interval(0, 5));
    REQUIRE(geo::make_union(geo::make_interval(0, 3), geo::make_interval(1, 5)) == geo::make_interval(0, 5));
}

TEST_CASE("rect - make_union")
{

    REQUIRE(geo::make_union(geo::rect_2d<int>{ { 0, 0 }, { 3, 3 } }, geo::rect_2d<int>{ { 4, 4 }, { 5, 5 } }) == geo::rect_2d<int>{ { 0, 0 }, { 5, 5 } });
}