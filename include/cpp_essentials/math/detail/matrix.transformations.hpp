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

        get<0, 0>(result) = get<0>(scale);
        get<1, 1>(result) = get<1>(scale);

        return result;
    }

    template <class T>
    square_matrix_3d<T> operator ()(const vector_3d<T>& scale) const
    {
        square_matrix_3d<T> result = identity;

        get<0, 0>(result) = get<0>(scale);
        get<1, 1>(result) = get<1>(scale);
        get<2, 2>(result) = get<2>(scale);

        return result;
    }
};

struct make_rotation_fn
{
    template <class T>
    square_matrix_2d<T> operator ()(T angle) const
    {
        square_matrix_2d<T> result = identity;

        const auto c = cos(angle);
        const auto s = sin(angle);

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

        get<2, 0>(result) = get<0>(offset);
        get<2, 1>(result) = get<1>(offset);

        return result;
    }

    template <class T>
    square_matrix_3d<T> operator ()(const vector_3d<T>& offset) const
    {
        square_matrix_3d<T> result = identity;

        get<3, 0>(result) = get<0>(offset);
        get<3, 1>(result) = get<1>(offset);
        get<3, 2>(result) = get<2>(offset);

        return result;
    }
};

} /* namespace detail */

static constexpr auto make_scale = detail::make_scale_fn{};
static constexpr auto make_rotation = detail::make_rotation_fn{};
static constexpr auto make_translation = detail::make_translation_fn{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_MATRIX_TRANSFORMATIONS_HPP_ */
