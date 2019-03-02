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

TEST_CASE("triangle negation")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE(-t == geo::triangle_2d<int>{ { 0, 0 }, { -10, -20 }, { -20, 0 } });
}

TEST_CASE("triangle addition assignment")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    t += geo::make_vector(5, 5);
    REQUIRE(t == geo::triangle_2d<int>{ { 5, 5 }, { 15, 25 }, { 25, 5 } });
}

TEST_CASE("triangle addition")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE((t + geo::make_vector(5, 5)) == geo::triangle_2d<int>{ { 5, 5 }, { 15, 25 }, { 25, 5 } });
}

TEST_CASE("triangle subtraction assignment")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    t -= geo::make_vector(5, 5);
    REQUIRE(t == geo::triangle_2d<int>{ { -5, -5 }, { 5, 15 }, { 15, -5 } });
}

TEST_CASE("triangle subtraction")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE((t - geo::make_vector(5, 5)) == geo::triangle_2d<int>{ { -5, -5 }, { 5, 15 }, { 15, -5 } });
}

TEST_CASE("triangle-matrix multiplication")
{
    geo::triangle_2d<int> t = { { 0, 0 }, { 10, 20 }, { 20, 0 } };
    REQUIRE((t * geo::identity.make<3>()) == geo::triangle_2d<double>{ { 0, 0 }, { 10, 20 }, { 20, 0 } });
}