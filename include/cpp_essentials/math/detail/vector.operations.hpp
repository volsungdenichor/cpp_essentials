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
        static constexpr auto _dot = dot_fn{};
        return _dot(item, item);
    }
};

struct length_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr auto _norm = norm_fn{};
        return sqrt(_norm(item));
    }
};

struct normalize_fn
{
    template <class T, size_t D>
    auto& operator ()(vector<T, D>& item) const
    {
        static constexpr auto _length = length_fn{};
        auto len = _length(item);

        if (len)
        {
            core::transform((item / len)._data, item._data.begin(), core::cast<T>);
        }

        return item;
    }
};

struct unit_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& item) const
    {
        static constexpr auto _normalize = normalize_fn{};
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
        static constexpr auto _length = length_fn{};
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
        static constexpr auto _dot = dot_fn{};
        static constexpr auto _norm = norm_fn{};
        return rhs * (_dot(rhs, lhs) / _norm(rhs));
    }
};

struct rejection_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr auto _projection = projection_fn{};
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
        static constexpr auto _cross = cross_fn{};
        static constexpr auto _dot = dot_fn{};
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
        static constexpr auto _dot = dot_fn{};
        static constexpr auto _length = length_fn{};
        return acos(_dot(lhs, rhs) / (_length(lhs) * _length(rhs)));
    }
};

struct bisector_fn
{
    template <class T, size_t D>
    auto operator ()(const vector<T, D>& lhs, const vector<T, D>& rhs) const
    {
        static constexpr auto _unit = unit_fn{};
        return _unit(_unit(lhs) + _unit(rhs));
    }
};

} /* namespace detail */

static constexpr auto dot = detail::dot_fn{};
static constexpr auto norm = detail::norm_fn{};
static constexpr auto length = detail::length_fn{};
static constexpr auto normalize = detail::normalize_fn{};
static constexpr auto unit = detail::unit_fn{};
static constexpr auto distance = detail::distance_fn{};
static constexpr auto cross = detail::cross_fn{};
static constexpr auto projection = detail::projection_fn{};
static constexpr auto rejection = detail::rejection_fn{};
static constexpr auto perpendicular = detail::perpendicular_fn{};
static constexpr auto angle = detail::angle_fn{};
static constexpr auto bisector = detail::bisector_fn{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_DETAIL_VECTOR_OPERATIONS_HPP_ */