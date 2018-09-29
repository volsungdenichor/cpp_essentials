#include <catch.hpp>
#include <cpp_essentials/sq/slice.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("slice")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::slice(3, 5)) == vec(7, 3));
}

TEST_CASE("take")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::take(3)) == vec(2, 4, 5));
}

TEST_CASE("drop")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::drop(3)) == vec(7, 3, 1, 2));
}

TEST_CASE("py_slice")
{
    std::vector<int> vect{ 2, 4, 5, 7, 3, 1, 2 };
    REQUIRE((vect | sq::py_slice(3, nil)) == vec(7, 3, 1, 2));
    REQUIRE((vect | sq::py_slice(-3, nil)) == vec(3, 1, 2));
    REQUIRE((vect | sq::py_slice(nil, 3)) == vec(2, 4, 5));
    REQUIRE((vect | sq::py_slice(nil, -3)) == vec(2, 4, 5, 7));
    REQUIRE((vect | sq::py_slice(nil, nil)) == vec(2, 4, 5, 7, 3, 1, 2));
    REQUIRE((vect | sq::py_slice(4, 1)) == empty_vec<int>());
    REQUIRE((vect | sq::py_slice(4, 4)) == empty_vec<int>());
    REQUIRE((vect | sq::py_slice(40, 50)) == empty_vec<int>());

}
