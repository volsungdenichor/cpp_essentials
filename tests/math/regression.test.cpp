#include <catch.hpp>
#include <cpp_essentials/math/regression.hpp>

using namespace cpp_essentials;

TEST_CASE("linear regression")
{
    std::vector<double> a = { 1, 2, 3, 4, 6 };
    std::vector<double> b = { 3, 5, 7, 9, 13 };
    auto regression = math::linear_regression(a, b);
    REQUIRE(*regression == math::linear_polynomial<double>{ 1, 2 });
}

TEST_CASE("quadratic regression")
{
    std::vector<double> a = { 1, 2, 3, 4, 6 };
    std::vector<double> b = { 2, 5, 10, 17, 37 };
    auto regression = *math::quadratic_regression(a, b);
    REQUIRE(regression[0] == Approx(1));
    REQUIRE(regression[1] == Approx(0));
    REQUIRE(regression[2] == Approx(1));
}