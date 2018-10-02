#ifndef CPP_ESSENTIALS_GEO_DETAIL_MATRIX_OPERATIONS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_MATRIX_OPERATIONS_HPP_

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/optional.hpp>

#include <cpp_essentials/geo/detail/matrix.base.hpp>
#include <cpp_essentials/geo/detail/matrix.access.hpp>

#ifdef __GNUC__
#  undef minor
#endif

namespace cpp_essentials::geo
{

struct minor_t
{

    template <class T, std::size_t R, std::size_t C>
    auto operator ()(const matrix<T, R, C>& item, std::size_t row, std::size_t col) const -> matrix<T, R - 1, C - 1>
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

static constexpr minor_t minor = {};


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

    template <class T, std::size_t D>
    auto operator ()(const square_matrix<T, D>& item) const
    {
        auto sum = T{};

        for (std::size_t i = 0; i < D; ++i)
        {
            sum += (i % 2 == 0 ? 1 : -1) * item(0, i) * (*this)(minor(item, 0, i));
        }

        return sum;
    }
};

static constexpr determinant_t determinant = {};



struct is_invertible_t
{
    template <class T, std::size_t D>
    bool operator ()(const square_matrix<T, D>& value) const
    {
        return determinant(value);
    }
};

static constexpr is_invertible_t is_invertible = {};


struct invert_t
{
    template <class T, std::size_t D>
    auto operator ()(const square_matrix<T, D>& value) const -> core::optional<square_matrix<T, D>>
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
};

static constexpr invert_t invert = {};


struct transpose_t
{
    template <class T, std::size_t R, std::size_t C>
    auto operator ()(const matrix<T, R, C>& item) const
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

static constexpr transpose_t transpose = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_MATRIX_OPERATIONS_HPP_ */