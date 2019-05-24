#include <catch.hpp>
#include <cpp_essentials/sq/sq.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("views::iterate")
{
    auto vect = vec(2, 4, 5);
    auto r = vect | sq::iterate();
    for (size_t i = 0; i < vect.size(); ++i)
    {
        REQUIRE(r[i] == std::next(vect.begin(), i));
    }
}
