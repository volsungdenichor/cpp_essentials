#ifndef CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_

#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/math/detail/matrix.base.hpp>

namespace cpp_essentials::math
{

namespace detail
{

struct dot_t
{
    template <class T, class U, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> cc::Multiply<T, U>
    {
        return core::inner_product(lhs._data, rhs._data, cc::Multiply<T, U> {});
    }
};

struct norm_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr dot_t dot = {};
        return dot(item, item);
    }
};

struct length_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr norm_t norm = {};
        return sqrt(norm(item));
    }
};

struct normalize_t
{
    template <class T, size_t D>
    auto& operator ()(vector<T, D>& item) const
    {
        static constexpr length_t length = {};
        auto len = length(item);

        if (len)
        {
            item /= len;
        }

        return item;
    }
};

struct unit_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr normalize_t normalize = {};
        vector<T, D> result{ item };
        normalize(result);
        return result;
    }
};

struct distance_t
{
    template <class T, class U, size_t D >
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const
    {
        static constexpr length_t length = {};
        return length(rhs - lhs);
    }
};

struct cross_t
{
    template <class T, class U>
    auto operator ()(const vector_2d<T>& lhs, const vector_2d<U>& rhs) const -> cc::Multiply<T, U>
    {
        return lhs[0] * rhs[1] - lhs[1] * rhs[0];
    }

    template <class T, class U>
    auto operator ()(const vector_3d<T>& lhs, const vector_3d<U>& rhs) const -> vector<cc::Multiply<T, U>, 3>
    {
        return vector<cc::Multiply<T, U>, 3>{ {
            lhs[1] * rhs[2] - lhs[2] * rhs[1],
            lhs[2] * rhs[0] - lhs[0] * rhs[2],
            lhs[0] * rhs[1] - lhs[1] * rhs[0] }};
    }
};

struct projection_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr dot_t dot = {};
        static constexpr norm_t norm = {};
        return rhs * (dot(rhs, lhs) / norm(rhs));
    }
};

struct rejection_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr projection_t projection = {};
        return lhs - projection(lhs, rhs);
    }
};

struct perpendicular_t
{
    template <class T>
    auto operator ()(const vector_2d<T>& value) const
    {
        return vector<T, 2> { -value[1], value[0] };
    }
};

struct angle_t
{
    template <class T>
    auto operator ()(const vector_2d<T>& lhs, const vector_2d<T>& rhs) const
    {
        static constexpr cross_t cross = {};
        static constexpr dot_t dot = {};
        return atan2(cross(lhs, rhs), dot(lhs, rhs));
    }

    template <class T>
    auto operator ()(const vector_2d<T>& value) const
    {
        static const vector_2d<T> horizontal = { 1, 0 };
        return (*this)(horizontal, value);
    }

    template <class T>
    auto operator ()(const vector_3d<T>& lhs, const vector_3d<T>& rhs) const
    {
        static constexpr dot_t dot = {};
        static constexpr length_t length = {};
        return acos(dot(lhs, rhs) / (length(lhs) * length(rhs)));
    }
};

struct bisector_t
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr unit_t unit = {};
        return unit(unit(lhs) + unit(rhs));
    }
};

} /* namespace detail */

static constexpr detail::dot_t dot = {};
static constexpr detail::norm_t norm = {};
static constexpr detail::length_t length = {};
static constexpr detail::normalize_t normalize = {};
static constexpr detail::unit_t unit = {};
static constexpr detail::distance_t distance = {};
static constexpr detail::cross_t cross = {};
static constexpr detail::projection_t projection = {};
static constexpr detail::rejection_t rejection = {};
static constexpr detail::perpendicular_t perpendicular = {};
static constexpr detail::angle_t angle = {};
static constexpr detail::bisector_t bisector = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_ */