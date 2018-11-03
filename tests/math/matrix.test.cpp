#include <catch.hpp>
#include <cpp_essentials/math/matrix.hpp>

using namespace cpp_essentials;

TEST_CASE("make_vector")
{   
    REQUIRE(math::make_vector(1.0, 2.0) == math::vector_2d<double> { 1, 2 });
}

TEST_CASE("matrix::structured bindings")
{
    auto[x, y, z] = math::make_vector(2, 3, 5);
    REQUIRE(x == 2);
    REQUIRE(y == 3);
    REQUIRE(z == 5);
}

TEST_CASE("ones")
{
    math::vector_2d<double> v = math::ones;
    REQUIRE(v == math::vector_2d<double> { 1, 1 });
}

TEST_CASE("matrix copy")
{
    math::square_matrix_2d<int> original = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<float> actual{ original };
    REQUIRE(actual == original);
}

TEST_CASE("matrix assignment")
{
    math::square_matrix_2d<int> original = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<float> actual;
    actual = original;
    REQUIRE(actual == original);
}

TEST_CASE("matrix plus")
{
    math::square_matrix_2d<int> original = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    REQUIRE(+original == original);
}

TEST_CASE("matrix negation")
{
    math::square_matrix_2d<int> original = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };   
    REQUIRE(-original == math::square_matrix_2d<int>{ { -1, -1, -1 }, { -2, -2, -2 }, { -3, -3, -3 } });
}

TEST_CASE("matrix addition assignment")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<int> b = { { 1, 2, 3 }, { 1, 2, 3 }, { 1, 2, 3 } };
    a += b;
    REQUIRE(a == math::square_matrix_2d<int>{ { 2, 3, 4 }, { 3, 4, 5 }, { 4, 5, 6 } });
}

TEST_CASE("matrix addition")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<int> b = { { 1, 2, 3 }, { 1, 2, 3 }, { 1, 2, 3 } };
    REQUIRE((a + b) == math::square_matrix_2d<int>{ { 2, 3, 4 }, { 3, 4, 5 }, { 4, 5, 6 } });
}

TEST_CASE("matrix subtraction assignment")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<int> b = { { 1, 2, 3 }, { 1, 2, 3 }, { 1, 2, 3 } };
    a -= b;
    REQUIRE(a == math::square_matrix_2d<int>{ { 0, -1, -2 }, { 1, 0, -1 }, { 2, 1, 0 } });
}

TEST_CASE("matrix subtraction")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<int> b = { { 1, 2, 3 }, { 1, 2, 3 }, { 1, 2, 3 } };
    REQUIRE((a - b) == math::square_matrix_2d<int>{ { 0, -1, -2 }, { 1, 0, -1 }, { 2, 1, 0 } });
}

TEST_CASE("matrix scalar multiplication assignment")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };   
    a *= 2;
    REQUIRE(a == math::square_matrix_2d<int>{ { 2, 2, 2 }, { 4, 4, 4 }, { 6, 6, 6 } });
}

TEST_CASE("matrix scalar multiplication")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };   
    REQUIRE((a * 2.5) == math::square_matrix_2d<double> { { 2.5, 2.5, 2.5 }, { 5, 5, 5 }, { 7.5, 7.5, 7.5 } });
    REQUIRE((2.5 * a) == math::square_matrix_2d<double> { { 2.5, 2.5, 2.5 }, { 5, 5, 5 }, { 7.5, 7.5, 7.5 } });
}

TEST_CASE("matrix identity-matrix multiplication")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    math::square_matrix_2d<int> b = math::identity;
    REQUIRE((a * b) == a);
    REQUIRE((b * a) == a);
}

TEST_CASE("matrix matrix multiplication")
{
    math::square_matrix_2d<int> a = { { 1, 2, 1 }, { 2, 3, 2 }, { 3, 4, 3 } };
    math::square_matrix_2d<int> b = { { 2, 1, 1 }, { 1, 2, 2 }, { 1, 2, 1 } };
    REQUIRE((a * b) == math::square_matrix_2d<int>{ {5, 7, 6 }, { 9, 12, 10 }, { 13, 17, 14 } });
    REQUIRE((b * a) == math::square_matrix_2d<int>{ {7, 11, 7 }, { 11, 16, 11 }, { 8, 12, 8 } });
}

TEST_CASE("matrix scalar division assignment")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    a /= 2;
    REQUIRE(a == math::square_matrix_2d<int>{ { 0, 0, 0 }, { 1, 1, 1 }, { 1, 1, 1 } });
}

TEST_CASE("matrix scalar division")
{
    math::square_matrix_2d<int> a = { { 1, 1, 1 }, { 2, 2, 2 }, { 3, 3, 3 } };
    REQUIRE((a / 2.0) == math::square_matrix_2d<double> { { 0.5, 0.5, 0.5 }, { 1, 1, 1 }, { 1.5, 1.5, 1.5 } });   
}

TEST_CASE("matrix::elementwise")
{
    math::vector_2d<double> a = { 2.0, 3.0 };
    math::vector_2d<double> b = { 2.5, 1.5 };
    REQUIRE(math::elementwise_multiply(a, b) == math::make_vector(5.0, 4.5));
    REQUIRE(math::elementwise_divide(b, a) == math::make_vector(1.25, 0.5));
}