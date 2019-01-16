#ifndef CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_
#define CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_

#include <numeric>

#include <cpp_essentials/cc/cc.hpp>
#include <cpp_essentials/math/detail/matrix.base.hpp>

namespace cpp_essentials::math
{

namespace detail
{

struct dot_fn
{
    template <class T, class U, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const -> cc::Multiply<T, U>
    {
        return core::inner_product(lhs._data, rhs._data, cc::Multiply<T, U> {});
    }
};

struct norm_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr dot_fn _dot = {};
        return _dot(item, item);
    }
};

struct length_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr norm_fn _norm = {};
        return sqrt(_norm(item));
    }
};

struct normalize_fn
{
    template <class T, size_t D>
    auto& operator ()(vector<T, D>& item) const
    {
        static constexpr length_fn _length = {};
        auto len = _length(item);

        if (len)
        {
            item /= len;
        }

        return item;
    }
};

struct unit_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr normalize_fn _normalize = {};
        vector<T, D> result{ item };
        _normalize(result);
        return result;
    }
};

struct distance_fn
{
    template <class T, class U, size_t D >
    auto operator ()(const vector<T, D>& lhs, const vector<U, D>& rhs) const
    {
        static constexpr length_fn _length = {};
        return _length(rhs - lhs);
    }
};

struct cross_fn
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

struct projection_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr dot_fn _dot = {};
        static constexpr norm_fn _norm = {};
        return rhs * (_dot(rhs, lhs) / _norm(rhs));
    }
};

struct rejection_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr projection_fn _projection = {};
        return lhs - _projection(lhs, rhs);
    }
};

struct perpendicular_fn
{
    template <class T>
    auto operator ()(const vector_2d<T>& value) const
    {
        return vector<T, 2> { -value[1], value[0] };
    }
};

struct angle_fn
{
    template <class T>
    auto operator ()(const vector_2d<T>& lhs, const vector_2d<T>& rhs) const
    {
        static constexpr cross_fn _cross = {};
        static constexpr dot_fn _dot = {};
        return atan2(_cross(lhs, rhs), _dot(lhs, rhs));
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
        static constexpr dot_fn _dot = {};
        static constexpr length_fn _length = {};
        return acos(_dot(lhs, rhs) / (_length(lhs) * _length(rhs)));
    }
};

struct bisector_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr unit_fn _unit = {};
        return _unit(_unit(lhs) + _unit(rhs));
    }
};

} /* namespace detail */

static constexpr detail::dot_fn dot = {};
static constexpr detail::norm_fn norm = {};
static constexpr detail::length_fn length = {};
static constexpr detail::normalize_fn normalize = {};
static constexpr detail::unit_fn unit = {};
static constexpr detail::distance_fn distance = {};
static constexpr detail::cross_fn cross = {};
static constexpr detail::projection_fn projection = {};
static constexpr detail::rejection_fn rejection = {};
static constexpr detail::perpendicular_fn perpendicular = {};
static constexpr detail::angle_fn angle = {};
static constexpr detail::bisector_fn bisector = {};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_ */