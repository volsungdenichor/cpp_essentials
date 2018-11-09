#include <catch.hpp>
#include <cpp_essentials/geo/contains.hpp>

using namespace cpp_essentials;

TEST_CASE("interval contains value")
{
    REQUIRE(geo::contains(geo::make_interval(1, 5), 2) == true);
    REQUIRE(geo::contains(geo::make_interval(1, 5), 1) == true);
    REQUIRE(geo::contains(geo::make_interval(1, 5), 0) == false);
    REQUIRE(geo::contains(geo::make_interval(1, 5), 6) == false);
    REQUIRE(geo::contains(geo::make_interval(1, 5), 5) == false);
}

TEST_CASE("interval contains other interval")
{
    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(1, 5)) == true);
    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(2, 4)) == true);
    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(2, 5)) == true);
    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(1, 2)) == true);

    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(4, 6)) == false);
    REQUIRE(geo::contains(geo::make_interval(1, 5), geo::make_interval(0, 1)) == false);
}

TEST_CASE("rect contains vector")
{
    auto rect = geo::rect_2d<int>{ { 0, 0 }, { 5, 3 } };
    REQUIRE(geo::contains(rect, geo::make_vector(0, 0)) == true);
    REQUIRE(geo::contains(rect, geo::make_vector(4, 0)) == true);
    REQUIRE(geo::contains(rect, geo::make_vector(4, 2)) == true);
    REQUIRE(geo::contains(rect, geo::make_vector(0, 2)) == true);

    REQUIRE(geo::contains(rect, geo::make_vector(5, 3)) == false);
    REQUIRE(geo::contains(rect, geo::make_vector(5, 0)) == false);
    REQUIRE(geo::contains(rect, geo::make_vector(0, 3)) == false);

    REQUIRE(geo::contains(rect, geo::make_vector(2, 1)) == true);
    REQUIRE(geo::contains(rect, geo::make_vector(-1, -1)) == false);
    REQUIRE(geo::contains(rect, geo::make_vector(6, 4)) == false);
}

TEST_CASE("rect contains rect")
{
    auto rect = geo::rect_2d<int>{ { 0, 0 }, { 5, 5 } };
    REQUIRE(geo::contains(rect, geo::rect_2d<int>{ { 0, 0 }, { 5, 5 } }) == true);
    REQUIRE(geo::contains(rect, geo::rect_2d<int>{ { 1, 1 }, { 3, 3 } }) == true);
    REQUIRE(geo::contains(rect, geo::rect_2d<int>{ { 4, 4 }, { 5, 5 } }) == true);

    REQUIRE(geo::contains(rect, geo::rect_2d<int>{ { 4, 4 }, { 6, 6 } }) == false);
    REQUIRE(geo::contains(rect, geo::rect_2d<int>{ { 6, 6 }, { 8, 8 } }) == false);
}