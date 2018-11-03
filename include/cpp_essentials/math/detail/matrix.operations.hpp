#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/optional.hpp>

#include <cpp_essentials/math/detail/matrix.base.hpp>
#include <cpp_essentials/math/detail/matrix.access.hpp>

#ifdef __GNUC__
#  undef minor
#endif

namespace cpp_essentials::math
{

namespace detail
{

struct minor_t
{
    template <class T, size_t R, size_t C>
    auto operator ()(const matrix<T, R, C>& item, size_t row, size_t col) const -> matrix<T, R - 1, C - 1>
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
};

struct determinant_t
{
    template <class T>
    auto operator ()(const square_matrix<T, 1>& item) const
    {
        return get<0, 0>(item);
    }

    template <class T>
    auto operator ()(const square_matrix<T, 2>& item) const
    {
        return
            + get<0, 0>(item) * get<1, 1>(item)
            - get<0, 1>(item) * get<1, 0>(item);
    }

    template <class T>
    auto operator ()(const square_matrix<T, 3>& item) const
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
    auto operator ()(const square_matrix<T, D>& item) const
    {
        static constexpr minor_t minor = {};

        auto sum = T{};

        for (size_t i = 0; i < D; ++i)
        {
            sum += (i % 2 == 0 ? 1 : -1) * item(0, i) * (*this)(minor(item, 0, i));
        }

        return sum;
    }
};

struct is_invertible_t
{
    template <class T, size_t D>
    bool operator ()(const square_matrix<T, D>& value) const
    {
        static constexpr determinant_t determinant = {};
        return determinant(value);
    }
};

struct invert_t
{
    template <class T, size_t D>
    auto operator ()(const square_matrix<T, D>& value) const -> core::optional<square_matrix<T, D>>
    {
        static constexpr determinant_t determinant = {};
        static constexpr minor_t minor = {};

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
};

struct transpose_t
{
    template <class T, size_t R, size_t C>
    auto operator ()(const matrix<T, R, C>& item) const -> matrix<T, C, R>
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
};

} /* namespace detail */

static constexpr detail::minor_t minor = {};
static constexpr detail::determinant_t determinant = {};
static constexpr detail::is_invertible_t is_invertible = {};
static constexpr detail::invert_t invert = {};
static constexpr detail::transpose_t transpose = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_OPERATIONS_HPP_ */