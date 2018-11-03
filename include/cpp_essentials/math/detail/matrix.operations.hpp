#ifndef GEO_MATRIX_OPERATIONS_HPP_
#define GEO_MATRIX_OPERATIONS_HPP_

#include <core/assertions.hpp>
#include <core/optional.hpp>

#include <geo/matrix.base.hpp>
#include <geo/matrix.access.hpp>

#ifdef __GNUC__
#  undef minor
#endif

namespace geo
{

template <class T, size_t R, size_t C>
auto minor(const matrix<T, R, C>& item, size_t row, size_t col)
{
    static_assert(R > 1, "minor: invalid row.");
    static_assert(C > 1, "minor: invalid col.");

    EXPECTS(row < R);
    EXPECTS(col < C);

    matrix<T, R - 1, C - 1> result;

    for (size_t r = 0; r < result.row_count(); ++r)
    {
        for (size_t c = 0; c < result.col_count(); ++c)
        {
            result(r, c) = item(r + (r < row ? 0 : 1), c + (c < col ? 0 : 1));
        }
    }

    return result;
}

template <size_t Row, size_t Col, class T, size_t R, size_t C>
auto minor(const matrix<T, R, C>& item)
{
    static_assert(Row < R, "minor: invalid row.");
    static_assert(Col < C, "minor: invalid col.");

    return minor(item, Row, Col);
}



template <class T>
auto determinant(const square_matrix<T, 1>& item)
{
    return get<0, 0>(item);
}

template <class T>
auto determinant(const square_matrix<T, 2>& item)
{
    return
        + get<0, 0>(item) * get<1, 1>(item)
        - get<0, 1>(item) * get<1, 0>(item);
}

template <class T>
auto determinant(const square_matrix<T, 3>& item)
{
    return
        + get<0, 0>(item) * get<1, 1>(item) * get<2, 2>(item)
        + get<0, 1>(item) * get<1, 2>(item) * get<2, 0>(item)
        + get<0, 2>(item) * get<1, 0>(item) * get<2, 1>(item)
        - get<0, 2>(item) * get<1, 1>(item) * get<2, 0>(item)
        - get<0, 0>(item) * get<1, 2>(item) * get<2, 1>(item)
        - get<0, 1>(item) * get<1, 0>(item) * get<2, 2>(item);
}

template <class T, size_t D>
auto determinant(const square_matrix<T, D>& item)
{
    auto sum = T {};

    for (size_t i = 0; i < D; ++i)
    {
        sum += (i % 2 == 0 ? 1 : -1) * item(0, i) * determinant(minor(item, 0, i));
    }

    return sum;
}



template <class T, size_t D>
bool is_invertible(const square_matrix<T, D>& value)
{
    return determinant(value);
}

template <class T, size_t D>
auto invert(const square_matrix<T, D>& value) -> core::optional<square_matrix<T, D>>
{
    auto det = determinant(value);

    if (!det)
    {
        return{};
    }

    square_matrix<T, D> result;

    for (size_t r = 0; r < D; ++r)
    {
        for (size_t c = 0; c < D; ++c)
        {
            result(c, r) = T((r + c) % 2 == 0 ? 1 : -1) * determinant(minor(value, r, c)) / det;
        }
    }

    return result;
}



template <class T, size_t R, size_t C>
auto transpose(const matrix<T, R, C>& item)
{
    matrix<T, C, R> result;

    for (size_t r = 0; r < item.row_count(); ++r)
    {
        for (size_t c = 0; c < item.col_count(); ++c)
        {
            result(c, r) = item(r, c);
        }
    }

    return result;
}

} /* namespace geo */

#endif /* GEO_MATRIX_OPERATIONS_HPP_ */