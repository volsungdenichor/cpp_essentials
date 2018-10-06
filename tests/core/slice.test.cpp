#include <catch.hpp>
#include <cpp_essentials/core/slice.hpp>

#include <../tests/test_helpers.hpp>

using namespace cpp_essentials;

TEST_CASE("slice")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::slice(3, 5)) == vec(7, 3));
}

TEST_CASE("take")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::take(3)) == vec(2, 4, 5));
}

TEST_CASE("drop")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::drop(3)) == vec(7, 3, 1, 2));
}

TEST_CASE("take_back")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::take_back(3)) == vec(3, 1, 2));
}

TEST_CASE("drop_back")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::drop_back(3)) == vec(2, 4, 5, 7));
}

TEST_CASE("py_slice")
{
    auto vect = vec(2, 4, 5, 7, 3, 1, 2);
    REQUIRE((vect | core::py_slice(2, 4)) == vec(5, 7));
    REQUIRE((vect | core::py_slice(3, nil)) == vec(7, 3, 1, 2));
    REQUIRE((vect | core::py_slice(-3, nil)) == vec(3, 1, 2));
    REQUIRE((vect | core::py_slice(nil, 3)) == vec(2, 4, 5));
    REQUIRE((vect | core::py_slice(nil, -3)) == vec(2, 4, 5, 7));
    REQUIRE((vect | core::py_slice(nil, nil)) == vec(2, 4, 5, 7, 3, 1, 2));
    REQUIRE((vect | core::py_slice(4, 1)) == empty_vec<int>());
    REQUIRE((vect | core::py_slice(4, 4)) == empty_vec<int>());
    REQUIRE((vect | core::py_slice(40, 50)) == empty_vec<int>());

}
