#include <catch.hpp>
#include <cpp_essentials/geo/bounding_box.hpp>

using namespace cpp_essentials;

TEST_CASE("rect properties")
{
    geo::rect_2d<int> rect = { { 0, 0 }, { 50, 30 } };
    REQUIRE(rect.lower() == geo::make_vector(0, 0));
    REQUIRE(rect.upper() == geo::make_vector(50, 30));
    REQUIRE(rect.min() == geo::make_vector(0, 0));
    REQUIRE(rect.max() == geo::make_vector(49, 29));
    REQUIRE(rect.size() == geo::make_vector(50, 30));
    REQUIRE(rect.empty() == false);
}

TEST_CASE("rect equal")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    geo::rect_2d<int> b = { { 0, 0 }, { 50, 30 } };
    REQUIRE(a == b);
}

TEST_CASE("rect addition assignment")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    a += geo::make_vector(5, 5);
    REQUIRE(a == geo::rect_2d<int>{ { 5, 5 }, { 55, 35 } });
}

TEST_CASE("rect addition")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    REQUIRE(a + geo::make_vector(10, 5) == geo::rect_2d<int>{ { 10, 5 }, { 60, 35 } });
}

TEST_CASE("rect subtraction assignment")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    a -= geo::make_vector(5, 5);
    REQUIRE(a == geo::rect_2d<int>{ { -5, -5 }, { 45, 25 } });
}

TEST_CASE("rect subtraction")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    REQUIRE(a - geo::make_vector(10, 5) == geo::rect_2d<int>{ { -10, -5 }, { 40, 25 } });
}

TEST_CASE("rect as quad")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    geo::quad_2d<int> quad = a;
    REQUIRE(quad == geo::quad_2d<int>{ { 0, 0 }, { 49, 0 }, { 49, 29 }, { 0, 29 } });
}

TEST_CASE("rect - matrix multiplication")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    auto actual = a * geo::identity.make<3>();
    REQUIRE(actual == geo::quad_2d<double>{ { 0, 0 }, { 49, 0 }, { 49, 29 }, { 0, 29 } });
}

TEST_CASE("rect vertices")
{
    geo::rect_2d<int> a = { { 0, 0 }, { 50, 30 } };
    REQUIRE(geo::top_left(a) == geo::make_vector(0, 0));
    REQUIRE(geo::top_right(a) == geo::make_vector(49, 0));
    REQUIRE(geo::bottom_left(a) == geo::make_vector(0, 29));
    REQUIRE(geo::bottom_right(a) == geo::make_vector(49, 29));

    REQUIRE(geo::left(a) == 0);
    REQUIRE(geo::right(a) == 49);
    REQUIRE(geo::top(a) == 0);
    REQUIRE(geo::bottom(a) == 29);
}