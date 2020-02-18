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

    REQUIRE((map | sq::map_equal_range(2)).size() == 2);
    REQUIRE((map | sq::map_equal_range(3)).size() == 1);
    REQUIRE((map | sq::map_equal_range(4)).size() == 0);
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
    REQUIRE((map | sq::map_at(2) | sq::to_vector()) == vec("B"s, "C"s));
}

TEST_CASE("map_utils::map_try_get")
{
    std::map<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 3, "D" }
    };

    using namespace std::string_literals;
    REQUIRE((map | sq::map_try_get(2)) == "B"s);
    REQUIRE((map | sq::map_try_get(4)) == core::none);
}

TEST_CASE("map_utils::map_get")
{
    std::map<int, std::string> map{
        { 1, "A" },
        { 2, "B" },
        { 3, "D" }
    };

    using namespace std::string_literals;
    REQUIRE((map | sq::map_get(2)) == "B"s);
    REQUIRE_THROWS(map | sq::map_get(4));
}
