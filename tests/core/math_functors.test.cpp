#include <catch.hpp>
#include <cpp_essentials/core/math_constants.hpp>
#include <cpp_essentials/core/math_functors.hpp>

using namespace cpp_essentials;

TEST_CASE("constants")
{
    REQUIRE(core::zero.as<double>() == 0.0);
    REQUIRE(core::one.as<double>() == 1.0);
    REQUIRE(core::pi.as<double>() == Approx(3.14).epsilon(0.001));
    REQUIRE(core::two_pi.as<double>() == Approx(6.2831).epsilon(0.001));
    REQUIRE(core::half_pi.as<double>() == Approx(1.5708).epsilon(0.001));
    REQUIRE(core::e.as<double>() == Approx(2.7182).epsilon(0.001));
}

TEST_CASE("pow")
{
    REQUIRE(core::pow(2, 3) == 8);
    REQUIRE(core::pow(3)(2) == 8);
    REQUIRE(core::pow(4, 0.5) == 2);
}

TEST_CASE("sqr")
{
    REQUIRE(core::sqr(4) == 16);
}

TEST_CASE("sqrt")
{
    REQUIRE(core::sqrt(4) == 2);
}

TEST_CASE("exp")
{
    REQUIRE(core::exp(1) == Approx(2.7182).epsilon(0.001));
}