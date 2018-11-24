#include <catch.hpp>
#include <cpp_essentials/geo/intersects.hpp>

using namespace cpp_essentials;

TEST_CASE("interval intersects interval")
{
    REQUIRE(geo::intersects(geo::make_interval(0, 1), geo::make_interval(2, 3)) == false);
    REQUIRE(geo::intersects(geo::make_interval(0, 2), geo::make_interval(1, 3)) == true);
    REQUIRE(geo::intersects(geo::make_interval(0, 2), geo::make_interval(2, 4)) == true);
    REQUIRE(geo::intersects(geo::make_interval(1, 4), geo::make_interval(1, 4)) == true);
    REQUIRE(geo::intersects(geo::make_interval(2, 2), geo::make_interval(1, 4)) == true);
}

TEST_CASE("rect intersects rect")
{
    auto rect = geo::rect_2d<int>{ { 0, 0 }, { 5, 5 } };
    REQUIRE(geo::intersects(rect, geo::rect_2d<int>{ { 0, 0 }, { 5, 5 } }) == true);
    REQUIRE(geo::intersects(rect, geo::rect_2d<int>{ { 1, 1 }, { 3, 3 } }) == true);
    REQUIRE(geo::intersects(rect, geo::rect_2d<int>{ { 4, 4 }, { 5, 5 } }) == true);

    REQUIRE(geo::intersects(rect, geo::rect_2d<int>{ { 4, 4 }, { 6, 6 } }) == true);
    REQUIRE(geo::intersects(rect, geo::rect_2d<int>{ { 6, 6 }, { 8, 8 } }) == false);
}

TEST_CASE("segment intersects segment")
{
    REQUIRE(geo::intersects(geo::segment_2d<float>{ { 0, 0 }, { 2, 2 } }, geo::segment_2d<float>{ { 0, 2 }, { 2, 0 } }) == true);
}

TEST_CASE("line intersects line")
{
    REQUIRE(geo::intersects(geo::line_2d<float>{ { 0, 0 }, { 10, 1 } }, geo::line_2d<float>{ { 0, 1 }, { 11, 2 } }) == true);
    REQUIRE(geo::intersects(geo::line_2d<float>{ { 0, 0 }, { 10, 1 } }, geo::line_2d<float>{ { 0, 1 }, { 10, 2 } }) == false);
}