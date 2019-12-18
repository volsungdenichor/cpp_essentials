#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

#include <map>

using namespace cpp_essentials;

TEST_CASE("map_utils::map_equal_range")
{
    std::multimap<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 2, "C" },
        { 3, "D" }
    };

    REQUIRE(sq::map_equal_range(map, 2).size() == 2);
    REQUIRE(sq::map_equal_range(map, 3).size() == 1);
    REQUIRE(sq::map_equal_range(map, 4).size() == 0);
}

TEST_CASE("map_utils::map_at")
{
    std::multimap<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 2, "C" },
        { 3, "D" }
    };

    using namespace std::string_literals;
    REQUIRE((sq::map_at(map, 2) | sq::to_vector()) == vec("B"s, "C"s));
}

TEST_CASE("map_utils::map_try_get")
{
    std::map<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 3, "D" }
    };

    using namespace std::string_literals;
    REQUIRE(sq::map_try_get(map, 2) == "B"s);
    REQUIRE(sq::map_try_get(map, 4) == core::none);
}

TEST_CASE("map_utils::map_get")
{
    std::map<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 3, "D" }
    };

    using namespace std::string_literals;
    REQUIRE(sq::map_get(map, 2) == "B"s);
    REQUIRE_THROWS(sq::map_get(map, 4));
}
