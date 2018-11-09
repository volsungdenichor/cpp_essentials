#include <catch.hpp>
#include <cpp_essentials/geo/interval.hpp>

using namespace cpp_essentials;

TEST_CASE("interval properties")
{
    auto interval = geo::make_interval(0, 50);
    REQUIRE(interval.lower() == 0);
    REQUIRE(interval.upper() == 50);
    REQUIRE(interval.min() == 0);
    REQUIRE(interval.max() == 49);
    REQUIRE(interval.size() == 50);
    REQUIRE(interval.empty() == false);
}

TEST_CASE("interval addition")
{
    auto interval = geo::make_interval(0, 50);
    REQUIRE((interval + 100) == geo::make_interval(100, 150));
    REQUIRE((100 + interval) == geo::make_interval(100, 150));
}

TEST_CASE("interval addition assignment")
{
    auto interval = geo::make_interval(0, 50);
    interval += 10;
    REQUIRE(interval == geo::make_interval(10, 60));
}

TEST_CASE("interval subtraction assignment")
{
    auto interval = geo::make_interval(0, 50);
    interval -= 10;
    REQUIRE(interval == geo::make_interval(-10, 40));
}

TEST_CASE("interval subtraction")
{
    auto interval = geo::make_interval(0, 50);
    REQUIRE((interval - 100) == geo::make_interval(-100, -50));
}

TEST_CASE("interval multiplication assignment")
{
    auto interval = geo::make_interval(0, 50);
    interval *= 2;
    REQUIRE(interval == geo::make_interval(0, 100));
}

TEST_CASE("interval multiplication")
{
    auto interval = geo::make_interval(0, 50);
    REQUIRE((interval * 2.5) == geo::make_interval(0.0, 125.0));
}

TEST_CASE("interval division assignment")
{
    auto interval = geo::make_interval(0, 50);
    interval /= 2;
    REQUIRE(interval == geo::make_interval(0, 25));
}

TEST_CASE("interval division")
{
    auto interval = geo::make_interval(0, 50);
    REQUIRE((interval / 2.5) == geo::make_interval(0.0, 20.0));
}
