#ifndef CPP_ESSENTIALS_GEO_DETAIL_MATRIX_CREATION_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_MATRIX_CREATION_HPP_

#pragma once

#include <cpp_essentials/math/functors.hpp>
#include <cpp_essentials/geo/detail/matrix.base.hpp>

namespace cpp_essentials::geo
{

struct identity_matrix_builder_t
{
    template <std::size_t D, class T = double>
    auto make() const -> square_matrix<T, D>
    {
        square_matrix<T, D> result;

        for (std::size_t r = 0; r < result.row_count(); ++r)
        {
            for (std::size_t c = 0; c < result.col_count(); ++c)
            {
                result(r, c) = r == c ? T(1) : T(0);
            }
        }

        return result;
    }

    template <class T, std::size_t D>
    operator square_matrix<T, D>() const
    {
        return make<D, T>();
    }
};

static constexpr identity_matrix_builder_t identity = {};


template <int Value>
struct matrix_builder_t
{
    template <std::size_t R, std::size_t C, class T = double>
    auto make() const -> matrix<T, R, C>
    {
        matrix<T, R, C> result;

        core::fill(result._data, T(Value));

        return result;
    }

    template <size_t D, class T = double>
    auto make() const -> vector<T, D>
    {
        return make<1, D, T>();
    }

    template <class T, std::size_t R, std::size_t C>
    operator matrix<T, R, C>() const
    {
        return make<R, C, T>();
    }
};

static constexpr matrix_builder_t<0> zeros = {};
static constexpr matrix_builder_t<1> ones = {};


struct make_translation_t
{
    template <class T>
    auto operator ()(const vector_2d<T>& offset) const -> square_matrix_2d<T>
    {
        square_matrix_2d<T> result = identity;

        get<2, 0>(result) = offset[0];
        get<2, 1>(result) = offset[1];

        return result;
    }

    template <class T>
    auto operator ()(const vector_3d<T>& offset) -> square_matrix_3d<T>
    {
        square_matrix_3d<T> result = identity;

        get<3, 0>(result) = offset[0];
        get<3, 1>(result) = offset[1];
        get<3, 2>(result) = offset[2];

        return result;
    }
};

static constexpr make_translation_t make_translation = {};


struct make_scale_t
{
    template <class T>
    auto operator ()(const vector_2d<T>& scale) const -> square_matrix_2d<T>
    {
        square_matrix_2d<T> result = identity;

        get<0, 0>(result) = scale[0];
        get<1, 1>(result) = scale[1];

        return result;
    }

    template <class T>
    auto operator ()(const vector_2d<T>& scale, const vector_2d<T>& pivot) const -> square_matrix_2d<T>
    {
        return make_translation(-pivot)
            * (*this)(scale)
            * make_translation(pivot);
    }

    template <class T>
    auto operator ()(const vector_3d<T>& scale) const -> square_matrix_3d<T>
    {
        square_matrix_3d<T> result = identity;

        get<0, 0>(result) = scale[0];
        get<1, 1>(result) = scale[1];
        get<2, 2>(result) = scale[2];

        return result;
    }

    template <class T>
    auto operator ()(const vector_3d<T>& scale, const vector_3d<T>& pivot) const -> square_matrix_3d<T>
    {
        return make_translation(-pivot)
            * (*this)(scale)
            * make_translation(pivot);
    }
};

static constexpr make_scale_t make_scale = {};


struct make_rotation_t
{
    template <class T>
    auto operator ()(T angle) const -> square_matrix_2d<T>
    {
        square_matrix_2d<T> result = identity;

        auto c = math::cos(angle);
        auto s = math::sin(angle);

        get<0, 0>(result) = c;
        get<0, 1>(result) = s;
        get<1, 0>(result) = -s;
        get<1, 1>(result) = c;

        return result;
    }

    template <class T>
    auto operator ()(T angle, const vector_2d<T>& pivot) const -> square_matrix_2d<T>
    {
        return make_translation(-pivot)
            * (*this)(angle)
            * make_translation(pivot);
    }
};

static constexpr make_rotation_t make_rotation = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_MATRIX_CREATION_HPP_ */
