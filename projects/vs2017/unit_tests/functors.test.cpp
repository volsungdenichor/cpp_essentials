#pragma once

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
