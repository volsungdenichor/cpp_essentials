#ifndef CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_TRAITS_HPP_
#define CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_TRAITS_HPP_

#include <cpp_essentials/geo/detail/linear_shape.base.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

template <class Shape>
struct linear_shape_traits
{
};

template <class T, size_t D>
struct linear_shape_traits<segment<T, D>>
{
    using shape_type = segment<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static const size_t dimension = D;

    static vertex_type get_point(const shape_type& shape, size_t index)
    {
        return shape[index];
    }

    template <class P>
    static bool contains_parameter(P parameter)
    {
        return P(0) < parameter && parameter < P(1);
    }
};



template <class T, size_t D>
struct linear_shape_traits<ray<T, D>>
{
    using shape_type = ray<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static const size_t dimension = D;

    static vertex_type get_point(const shape_type& shape, size_t index)
    {
        return shape[index];
    }

    template <class P>
    static bool contains_parameter(P parameter)
    {
        return parameter >= P(0);
    }
};



template <class T, size_t D>
struct linear_shape_traits<line<T, D>>
{
    using shape_type = line<T, D>;

    using value_type = T;

    using vertex_type = vector<T, D>;

    using segment_type = segment<T, D>;

    static const size_t dimension = D;

    static vertex_type get_point(const shape_type& shape, size_t index)
    {
        return shape[index];
    }

    template <class P>
    static bool contains_parameter(P)
    {
        return true;
    }
};

template <class T, class E>
auto get_line_intersection_parameter(
    const vector<T, 2>& a0,
    const vector<T, 2>& a1,
    const vector<T, 2>& p,
    E epsilon) -> core::optional<T>
{
    auto dir = a1 - a0;

    auto d = p - a0;

    auto det = cross(dir, d);

    return core::make_optional(
        core::approx_equal_to(det, math::zero, epsilon),
        dot(d, dir) / norm(dir));
}

template <class T, class E>
auto get_line_intersection_parameters(
    const vector<T, 2>& a0,
    const vector<T, 2>& a1,
    const vector<T, 2>& b0,
    const vector<T, 2>& b1,
    E epsilon) -> core::optional<std::tuple<T, T>>
{
    auto dir_a = a1 - a0;
    auto dir_b = b1 - b0;

    auto det = cross(dir_a, dir_b);
    auto v = b0 - a0;

    return core::make_optional(
        !core::approx_equal_to(det, math::zero, epsilon),
        std::make_tuple(cross(v, dir_b) / det, cross(v, dir_a) / det));
}

} /* namespace detail */

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_DETAIL_LINEAR_SHAPE_TRAITS_HPP_ */