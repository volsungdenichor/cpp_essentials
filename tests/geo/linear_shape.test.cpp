#include <catch.hpp>
#include <cpp_essentials/geo/linear_shape.hpp>

using namespace cpp_essentials;

TEST_CASE("make segment")
{
    auto s = geo::make_segment(geo::make_vector(5, 10), geo::make_vector(20, 20));
    REQUIRE(s == geo::segment_2d<int>{ { 5, 10 }, { 20, 20 } });
}

TEST_CASE("make line")
{
    auto s = geo::make_line(geo::make_vector(5, 10), geo::make_vector(20, 20));
    REQUIRE(s == geo::line_2d<int>{ { 5, 10 }, { 20, 20 } });
}

TEST_CASE("make ray")
{
    auto s = geo::make_ray(geo::make_vector(5, 10), geo::make_vector(20, 20));
    REQUIRE(s == geo::ray_2d<int>{ { 5, 10 }, { 20, 20 } });
}

TEST_CASE("segment addition assignment")
{
    geo::segment_2d<int> s = { { 0, 0 }, { 10, 20 } };
    s += geo::make_vector(5, 5);
    REQUIRE(s == geo::segment_2d<int>{ { 5, 5 }, { 15, 25 } });
}

TEST_CASE("segment addition")
{
    geo::segment_2d<int> s = { { 0, 0 }, { 10, 20 } };
    REQUIRE((s + geo::make_vector(5, 5)) == geo::segment_2d<int>{ { 5, 5 }, { 15, 25 } });
}

TEST_CASE("segment subtraction assignment")
{
    geo::segment_2d<int> s = { { 0, 0 }, { 10, 20 } };
    s -= geo::make_vector(5, 5);
    REQUIRE(s == geo::segment_2d<int>{ { -5, -5 }, { 5, 15 } });
}

TEST_CASE("segment subtraction")
{
    geo::segment_2d<int> s = { { 0, 0 }, { 10, 20 } };
    REQUIRE((s - geo::make_vector(5, 5)) == geo::segment_2d<int>{ { -5, -5 }, { 5, 15 } });
}

TEST_CASE("segment-matrix multiplication")
{
    geo::segment_2d<int> s = { { 0, 0 }, { 10, 20 } };
    REQUIRE((s * geo::identity.make<3>()) == geo::segment_2d<double>{ { 0, 0 }, { 10, 20 } });
}
