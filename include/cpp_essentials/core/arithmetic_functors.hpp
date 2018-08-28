#ifndef CPP_ESSENTIALS_CORE_ARITHMETIC_FUNCTORS_HPP_
#define CPP_ESSENTIALS_CORE_ARITHMETIC_FUNCTORS_HPP_

#pragma once

#include <functional>

namespace cpp_essentials::core
{

namespace detail
{

template <class Op>
class unary_operator
{
public:
    using operator_type = Op;
    static constexpr operator_type operator_instance = {};

    template <class T>
    constexpr auto operator ()(const T& item) const
    {
        return operator_instance(item);
    }

    constexpr const auto& operator ()() const
    {
        return *this;
    }
};

template <class Op>
class binary_operator
{
public:
    using operator_type = Op;
    static constexpr operator_type operator_instance = {};

    template <class T>
    constexpr auto bind_left(const T& value) const
    {
        return [=](const auto& item) { return operator_instance(value, item); };
    }

    template <class T>
    constexpr auto bind_right(const T& value) const
    {
        return [=](const auto& item) { return operator_instance(item, value); };
    }

    template <class L, class R>
    constexpr auto operator ()(const L& lhs, const R& rhs) const
    {
        return operator_instance(lhs, rhs);
    }

    template <class T>
    constexpr auto operator ()(const T& value) const
    {
        return bind_right(value);
    }

    constexpr const auto& operator ()() const
    {
        return *this;
    }
};

} /* namespace detail */

constexpr detail::unary_operator<std::negate<>> negate = {};
constexpr detail::unary_operator<std::logical_not<>> logical_not = {};

constexpr detail::binary_operator<std::plus<>> plus = {};
constexpr detail::binary_operator<std::minus<>> minus = {};
constexpr detail::binary_operator<std::multiplies<>> multiplies = {};
constexpr detail::binary_operator<std::divides<>> divides = {};
constexpr detail::binary_operator<std::modulus<>> modulus = {};

constexpr detail::binary_operator<std::equal_to<>> equal_to = {};
constexpr detail::binary_operator<std::not_equal_to<>> not_equal_to = {};
constexpr detail::binary_operator<std::less<>> less = {};
constexpr detail::binary_operator<std::less_equal<>> less_equal = {};
constexpr detail::binary_operator<std::greater<>> greater = {};
constexpr detail::binary_operator<std::greater_equal<>> greater_equal = {};

constexpr detail::binary_operator<std::logical_and<>> logical_and = {};
constexpr detail::binary_operator<std::logical_or<>> logical_or = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ARITHMETIC_FUNCTORS_HPP_ */


