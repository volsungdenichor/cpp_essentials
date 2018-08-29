#include <catch.hpp>
#include <cpp_essentials/sq/iterate.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("iterate")
{
    std::vector<int> vect{ 2, 4, 5 };
    auto r = vect | sq::iterate();
    for (size_t i = 0; i < vect.size(); ++i)
    {
        REQUIRE(r[i] == std::next(vect.begin(), i));
    }
}
