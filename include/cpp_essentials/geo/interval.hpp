#ifndef CPP_ESSENTIALS_GEO_INTERVAL_HPP_
#define CPP_ESSENTIALS_GEO_INTERVAL_HPP_

#pragma once

#include <array>
#include <vector>
#include <functional>
#include <initializer_list>

#include <cpp_essentials/cc/cc.hpp>

#include <cpp_essentials/core/arithmetic_functors.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/output.hpp>

#include <cpp_essentials/core/algorithm.hpp>

namespace cpp_essentials::geo
{

namespace detail
{

template <class T, class FloatingPoint>
struct numeric_min_step_base { };

template <class T>
struct numeric_min_step_base<T, std::true_type>
{
    static constexpr T value = std::numeric_limits<T>::epsilon();
};

template <class T>
struct numeric_min_step_base<T, std::false_type>
{
    static constexpr T value = T(1);
};



template <class T>
struct numeric_min_step : numeric_min_step_base<T, typename std::is_floating_point<T>::type> { };

} /* namespace detail */


template <class T>
class interval
{
public:
    using value_type = T;

    static constexpr value_type negative_infinity = std::numeric_limits<T>::lowest();
    static constexpr value_type positive_infinity = std::numeric_limits<T>::max();

    constexpr interval(value_type lower, value_type upper)
        : _data { { lower, std::max(lower, upper) } }
    {
    }

    constexpr interval()
        : interval{ value_type {}, value_type {} }
    {
    }   

    const value_type& operator [](size_t index) const
    {
        return _data[index];
    }

    const value_type& lower() const
    {
        return _data[0];
    }

    const value_type& upper() const
    {
        return _data[1];
    }

    value_type min() const
    {
        return lower();
    }

    value_type max() const
    {
        return upper() - detail::numeric_min_step<T>::value;
    }

    value_type size() const
    {
        return upper() - lower();
    }

    bool empty() const
    {
        return lower() >= upper();
    }

    std::array<T, 2> _data;
};

template
    < class T
    , class U
    , CONCEPT = cc::Add<T, U>>
auto& operator +=(interval<T>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::plus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Add<T, U>>
auto operator +(const interval<T>& lhs, U rhs) -> interval<cc::Add<T, U>>
{
    interval<cc::Add<T, U>> result;
    core::transform(lhs._data, result._data.begin(), core::plus(rhs));
    return result;
}

template
    < class T
    , class U
    , CONCEPT = cc::Add<T, U>>
auto operator +(T lhs, const interval<U>& rhs) -> interval<cc::Add<T, U>>
{
    return rhs + lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Subtract<T, U>>
auto& operator -=(interval<T>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::minus(rhs));
    return lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Subtract<T, U>>
auto operator -(const interval<T>& lhs, U rhs) -> interval<cc::Subtract<T, U>>
{
    interval<cc::Subtract<T, U>> result;
    core::transform(lhs._data, result._data.begin(), core::minus(rhs));
    return result;
}

template
    < class T
    , class U
    , CONCEPT = cc::Multiply<T, U>>
auto& operator *=(interval<T>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(const interval<T>& lhs, U rhs) -> interval<cc::Multiply<T, U>>
{
    interval<cc::Multiply<T, U>> result;
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template
    < class T
    , class U
    , CONCEPT = cc::Multiply<T, U>>
auto operator *(T lhs, const interval<T>& rhs) -> interval<cc::Multiply<T, U>>
{
    return rhs * lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Divide<T, U>>
auto& operator /=(interval<T>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::divides(rhs));
    return lhs;
}

template
    < class T
    , class U
    , CONCEPT = cc::Divide<T, U>>
auto operator /(const interval<T>& lhs, U rhs) -> interval<cc::Divide<T, U>>
{
    interval<cc::Divide<T, U>> result;
    core::transform(lhs._data, result._data.begin(), core::divides(rhs));
    return result;
}

template <class T, class U>
bool operator ==(const interval<T>& lhs, const interval<U>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U>
bool operator !=(const interval<T>& lhs, const interval<U>& rhs)
{
    return !(lhs == rhs);
}


template <class T>
std::ostream& operator <<(std::ostream& os, const interval<T>& item)
{
    if (item.empty())
    {
        return os << "{ empty }";
    }

    os << "[";

    if (item.lower() == interval<T>::negative_infinity)
    {
        os << "-inf";
    }
    else
    {
        os << item.lower();
    }

    os << ":";

    if (item.upper() == interval<T>::positive_infinity)
    {
        os << "+inf";
    }
    else
    {
        os << item.upper();
    }

    os << ")";

    return os;
}

namespace detail
{

struct make_interval_fn
{
    template <class T>
    auto operator ()(T lower, T upper) const -> interval<T>
    {
        return { lower, upper };
    }
};

struct make_union_fn
{
    template <class T>
    auto operator ()(const interval<T>& lhs, const interval<T>& rhs) const -> interval<T>
    {
        if (lhs.empty())
        {
            return rhs;
        }

        if (rhs.empty())
        {
            return lhs;
        }

        return { std::min(lhs.lower(), rhs.lower()), std::max(lhs.upper(), rhs.upper()) };
    }
};

struct make_intersection_fn
{
    template <class T>
    auto operator ()(const interval<T>& lhs, const interval<T>& rhs) const -> interval<T>
    {
        if (lhs.empty() || rhs.empty())
        {
            return {};
        }

        return { std::max(lhs.lower(), rhs.lower()), std::min(lhs.upper(), rhs.upper()) };
    }
};

} /* namespace detail */

static constexpr detail::make_interval_fn make_interval = {};
static constexpr detail::make_union_fn make_union = {};
static constexpr detail::make_intersection_fn make_intersection = {};

} /* namespace cpp_essentials::geo */

#endif /* CPP_ESSENTIALS_GEO_INTERVAL_HPP_ */
