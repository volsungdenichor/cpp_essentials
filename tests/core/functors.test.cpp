#include <catch.hpp>
#include <cpp_essentials/core/functors.hpp>

#include <vector>
#include <algorithm>

using namespace cpp_essentials;

TEST_CASE("identity")
{
    REQUIRE(core::identity(2.5) == 2.5);
}

TEST_CASE("logical_negation")
{
    auto func = core::logical_negation([](auto x) { return x % 2 == 0; });
    REQUIRE(func(2) == false);
}

TEST_CASE("approx_equal_to")
{
    REQUIRE(core::approx_equal_to(2.50, 2.51, 0.1) == true);
    auto approx_equal_to = core::approx_equal_to(0.1);
    REQUIRE(approx_equal_to(2.50, 2.51) == true);
    REQUIRE(approx_equal_to(2.50, 2.61) == false);
}

TEST_CASE("clamp")
{
    REQUIRE(core::clamp(4.0, 3.0, 6.5) == 4.0);
    auto clamp = core::clamp(2.0, 3.5);
    REQUIRE(clamp(2.0) == 2.0);
    REQUIRE(clamp(3.5) == 3.5);
    REQUIRE(clamp(1.5) == 2.0);
    REQUIRE(clamp(4.5) == 3.5);
}

TEST_CASE("between")
{
    REQUIRE(core::between(4.0, 3.0, 6.5) == true);
    auto between = core::between(2.0, 3.5);
    REQUIRE(between(2.0) == true);
    REQUIRE(between(3.0) == true);
    REQUIRE(between(3.5) == false);
    REQUIRE(between(1.5) == false);
    REQUIRE(between(4.5) == false);
}

TEST_CASE("min")
{
    REQUIRE(core::min(4.0, 3.0) == 3.0);
    REQUIRE(core::min(3.0, 4.0) == 3.0);
    REQUIRE(core::min(3.0, 3.0) == 3.0);
}

TEST_CASE("max")
{
    REQUIRE(core::max(4.0, 3.0) == 4.0);
    REQUIRE(core::max(3.0, 4.0) == 4.0);
    REQUIRE(core::min(3.0, 3.0) == 3.0);
}

TEST_CASE("lerp")
{
    REQUIRE(core::lerp(0.25, 1, 2) == 1.25);
    auto lerp = core::lerp(0.5);
    REQUIRE(lerp(2, 5) == 3.5);
    REQUIRE(lerp(5, 2) == 3.5);
}

TEST_CASE("make_pair")
{
    REQUIRE(core::make_pair(2, 3.14) == std::pair{ 2, 3.14 });
}

TEST_CASE("make_tuple")
{
    REQUIRE(core::make_tuple(2, 3.14) == std::tuple{ 2, 3.14 });
}

TEST_CASE("tie")
{
    int x = 3;
    double y = 3.14;
    auto tuple = core::tie(x, y);
    REQUIRE(&core::get<0>(tuple) == &x);
    REQUIRE(&core::get<1>(tuple) == &y);
}

TEST_CASE("get")
{
    auto tuple = core::make_tuple(2, 3, 'x');
    REQUIRE(core::get<0>(tuple) == 2);
}

TEST_CASE("dereference")
{
    int x = 3;
    REQUIRE(core::dereference(&x) == 3);
}
