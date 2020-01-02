#ifndef CPP_ESSENTIALS_MATH_CONSTANTS_HPP_
#define CPP_ESSENTIALS_MATH_CONSTANTS_HPP_

#pragma once

#include <cmath>

namespace cpp_essentials::math
{

namespace detail
{

enum class constant_type
{
    zero,
    one,
    pi,
    half_pi,
    two_pi,
    e
};

template <constant_type C>
struct constant_value_provider
{
    static const double value;
};

template <constant_type C>
class constant_value
{
public:
    static const double& value()
    {
        static const double v = constant_value_provider<C>::value;
        return v;
    }

    template <class T>
    const T& as() const
    {
        static const T v = static_cast<T>(value());
        return v;
    }

    template <class T>
    operator T() const
    {
        return as<T>();
    }

    friend std::ostream& operator<<(std::ostream& os, const constant_value& item)
    {
        return os << item.value();
    }
};

template <>
const double constant_value_provider<constant_type::zero>::value = 0.0;

template <>
const double constant_value_provider<constant_type::one>::value = 1.0;

template <>
const double constant_value_provider<constant_type::pi>::value = std::asin(1.0) * 2.0;

template <>
const double constant_value_provider<constant_type::two_pi>::value = std::asin(1.0) * 4.0;

template <>
const double constant_value_provider<constant_type::half_pi>::value = std::asin(1.0);

template <>
const double constant_value_provider<constant_type::e>::value = std::exp(1.0);



template <constant_type C, class T>
bool operator ==(T lhs, const constant_value<C>& rhs)
{
    return lhs == rhs.template as<T>();
}

template <constant_type C, class T>
bool operator ==(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() == rhs;
}



template <constant_type C, class T>
bool operator !=(T lhs, const constant_value<C>& rhs)
{
    return lhs != rhs.template as<T>();
}

template <constant_type C, class T>
bool operator !=(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() == rhs;
}



template <constant_type C, class T>
bool operator <(T lhs, const constant_value<C>& rhs)
{
    return lhs < rhs.template as<T>();
}

template <constant_type C, class T>
bool operator <(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() < rhs;
}



template <constant_type C, class T>
bool operator >(T lhs, const constant_value<C>& rhs)
{
    return lhs > rhs.template as<T>();
}

template <constant_type C, class T>
bool operator >(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() > rhs;
}



template <constant_type C, class T>
bool operator <=(T lhs, const constant_value<C>& rhs)
{
    return lhs <= rhs.template as<T>();
}

template <constant_type C, class T>
bool operator <=(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() <= rhs;
}



template <constant_type C, class T>
bool operator >=(T lhs, const constant_value<C>& rhs)
{
    return lhs >= rhs.template as<T>();
}

template <constant_type C, class T>
bool operator >=(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() >= rhs;
}



template <constant_type C, class T>
T operator -(const constant_value<C>& lhs, T rhs)
{
    return lhs.template as<T>() - rhs;
}

template <constant_type C, class T>
T operator -(T lhs, const constant_value<C>& rhs)
{
    return lhs - rhs.template as<T>();
}

} /* namespace detail */

static constexpr auto zero = detail::constant_value<detail::constant_type::zero>{};
static constexpr auto one = detail::constant_value<detail::constant_type::one>{};

static constexpr auto pi = detail::constant_value<detail::constant_type::pi>{};
static constexpr auto two_pi = detail::constant_value<detail::constant_type::two_pi>{};
static constexpr auto half_pi = detail::constant_value<detail::constant_type::half_pi>{};

static constexpr auto e = detail::constant_value<detail::constant_type::e>{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_CONSTANTS_HPP_ */
