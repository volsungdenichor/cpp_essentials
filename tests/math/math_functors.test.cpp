#include <catch.hpp>
#include <cpp_essentials/math/constants.hpp>
#include <cpp_essentials/math/functors.hpp>

using namespace cpp_essentials;

TEST_CASE("constants")
{
    REQUIRE(math::zero.as<double>() == 0.0);
    REQUIRE(math::one.as<double>() == 1.0);
    REQUIRE(math::pi.as<double>() == Approx(3.14).epsilon(0.001));
    REQUIRE(math::two_pi.as<double>() == Approx(6.2831).epsilon(0.001));
    REQUIRE(math::half_pi.as<double>() == Approx(1.5708).epsilon(0.001));
    REQUIRE(math::e.as<double>() == Approx(2.7182).epsilon(0.001));
}

TEST_CASE("pow")
{
    REQUIRE(math::pow(2, 3) == 8);
    REQUIRE(math::pow(3)(2) == 8);
    REQUIRE(math::pow(4, 0.5) == 2);
}

TEST_CASE("sqr")
{
    REQUIRE(math::sqr(4) == 16);
}

TEST_CASE("sqrt")
{
    REQUIRE(math::sqrt(4) == 2);
}

TEST_CASE("exp")
{
    REQUIRE(math::exp(1) == Approx(2.7182).epsilon(0.001));
}