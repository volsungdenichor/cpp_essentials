#include <catch.hpp>
#include <cpp_essentials/geo/vertex_array.hpp>

using namespace cpp_essentials;

TEST_CASE("polygon addition assignment")
{
    geo::polygon_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    t += geo::make_vector(5, 5);
    REQUIRE(t == geo::polygon_2d<int>{ { 5, 5 }, { 15, 25 }, { 25, 5 } });
}

TEST_CASE("polygon addition")
{
    geo::polygon_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE((t + geo::make_vector(5, 5)) == geo::polygon_2d<int>{ { 5, 5 }, { 15, 25 }, { 25, 5 } });
}

TEST_CASE("polygon subtraction assignment")
{
    geo::polygon_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    t -= geo::make_vector(5, 5);
    REQUIRE(t == geo::polygon_2d<int>{ { -5, -5 }, { 5, 15 }, { 15, -5 } });
}

TEST_CASE("polygon subtraction")
{
    geo::polygon_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE((t - geo::make_vector(5, 5)) == geo::polygon_2d<int>{ { -5, -5 }, { 5, 15 }, { 15, -5 } });
}