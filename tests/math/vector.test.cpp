#include <catch.hpp>
#include <cpp_essentials/math/matrix.hpp>
#include <cpp_essentials/math/functors.hpp>

using namespace cpp_essentials;

TEST_CASE("vector dot product")
{
    auto a = math::make_vector(2, 3);
    auto b = math::make_vector(4, 5);
    REQUIRE(math::dot(a, b) == 23);
}

TEST_CASE("vector norm")
{
    auto a = math::make_vector(2, 3);
    REQUIRE(math::norm(a) == 13);
}

TEST_CASE("vector length")
{
    auto a = math::make_vector(3, 4);
    REQUIRE(math::length(a) == 5);
}

TEST_CASE("vector normalize")
{
    auto a = math::make_vector(3.0, 4.0);
    math::normalize(a);
    REQUIRE(a == math::make_vector(0.6, 0.8));
}

TEST_CASE("vector unit")
{
    auto a = math::make_vector(3.0, 4.0);
    REQUIRE(math::unit(a) == math::make_vector(0.6, 0.8));
}

TEST_CASE("vector distance")
{
    auto a = math::make_vector(3.0, 4.0);
    auto b = math::make_vector(0.0, 0.0);
    REQUIRE(math::distance(a, b) == 5);
}

TEST_CASE("vector cross product")
{
    auto a = math::make_vector(2.0, 1.0);
    auto b = math::make_vector(4.0, 3.0);
    REQUIRE(math::cross(a, b) == 2.0);
}

TEST_CASE("vector projection")
{
    auto a = math::make_vector(2.0, 1.0);
    auto b = math::make_vector(1.0, 2.0);
    REQUIRE(math::projection(a, b) == math::make_vector(0.8, 1.6));
}

TEST_CASE("vector rejection")
{
    auto a = math::make_vector(2.0, 1.0);
    auto b = math::make_vector(1.0, 3.0);
    REQUIRE(math::rejection(a, b) == math::make_vector(1.5, -0.5));
}

TEST_CASE("vector perpendicular")
{
    auto a = math::make_vector(2.0, 1.0);
    REQUIRE(math::perpendicular(a) == math::make_vector(-1.0, 2.0));
}

TEST_CASE("vector angle")
{
    auto a = math::make_vector(3.0, 0.0);
    auto b = math::make_vector(3.0, 3.0);
    REQUIRE(math::angle(a, b) == math::rad(45.0));
}

TEST_CASE("vector bisector")
{
    auto a = math::make_vector(3.0, 0.0);
    auto b = math::make_vector(0.0, 1.0);
    REQUIRE(math::bisector(a, b) == math::unit(math::make_vector(1.0, 1.0)));
}
