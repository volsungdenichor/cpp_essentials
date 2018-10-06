#include <catch.hpp>
#include <cpp_essentials/core/iterate.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("iterate")
{
    auto vect = vec(2, 4, 5);
    auto r = vect | core::iterate();
    for (size_t i = 0; i < vect.size(); ++i)
    {
        REQUIRE(r[i] == std::next(vect.begin(), i));
    }
}
