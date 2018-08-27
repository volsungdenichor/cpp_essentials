#include <catch.hpp>
#include <cpp_essentials/core/ptr_vector.hpp>

using namespace cpp_essentials;

TEST_CASE("ptr_vector")
{
    core::ptr_vector<int> vect;
    REQUIRE(vect.empty() == true);

    vect.emplace_back(10);
    vect.emplace_back(12);
    vect.emplace_back(16);

    REQUIRE(vect.empty() == false);
    REQUIRE(vect.size() == 3);

    REQUIRE(vect[0] == 10);
    REQUIRE(vect[1] == 12);
    REQUIRE(vect[2] == 16);
    REQUIRE(*vect.begin() == 10);

    vect.erase(vect.begin() + 1);
    REQUIRE(vect.size() == 2);

    REQUIRE(vect[0] == 10);
    REQUIRE(vect[1] == 16);
}
