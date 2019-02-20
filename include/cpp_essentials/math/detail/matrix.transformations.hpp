#ifndef CPP_ESSENTIALS_MATH_DETAIL_MATRIX_TRANSFORMATIONS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_MATRIX_TRANSFORMATIONS_HPP_

#pragma once

#include <cpp_essentials/math/detail/matrix.creation.hpp>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::math
{

namespace detail
{

struct make_scale_fn
{
    template <class T>
    square_matrix_2d<T> operator ()(const vector_2d<T>& scale) const
    {
        square_matrix_2d<T> result = identity;

        get<0, 0>(result) = scale[0];
        get<1, 1>(result) = scale[1];

        return result;
    }
};

struct make_rotation_fn
{
    template <class T>
    square_matrix_2d<T> operator ()(T angle) const
    {
        square_matrix_2d<T> result = identity;

        auto c = cos(angle);
        auto s = sin(angle);

        get<0, 0>(result) = c;
        get<0, 1>(result) = s;
        get<1, 0>(result) = -s;
        get<1, 1>(result) = c;

        return result;
    }
};

struct make_translation_fn
{
    template <class T>
    square_matrix_2d<T> operator ()(const vector_2d<T>& offset) const
    {
        square_matrix_2d<T> result = identity;

        get<2, 0>(result) = offset[0];
        get<2, 1>(result) = offset[1];

        return result;
    }
};

} /* namespace detail */

static constexpr detail::make_scale_fn make_scale = {};
static constexpr detail::make_rotation_fn make_rotation = {};
static constexpr detail::make_translation_fn make_translation = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_TRANSFORMATIONS_HPP_ */
