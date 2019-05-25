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

struct divisible
{
    template <class L, class R>
    constexpr bool operator ()(L value, R divisor) const
    {
        return value % divisor == 0;
    }
};

} /* namespace detail */

constexpr auto negate = detail::unary_operator<std::negate<>>{};
constexpr auto logical_not = detail::unary_operator<std::logical_not<>>{};

constexpr auto plus = detail::binary_operator<std::plus<>>{};
constexpr auto minus = detail::binary_operator<std::minus<>>{};
constexpr auto multiplies = detail::binary_operator<std::multiplies<>>{};
constexpr auto divides = detail::binary_operator<std::divides<>>{};
constexpr auto modulus = detail::binary_operator<std::modulus<>>{};

constexpr auto equal_to = detail::binary_operator<std::equal_to<>>{};
constexpr auto not_equal_to = detail::binary_operator<std::not_equal_to<>>{};
constexpr auto less = detail::binary_operator<std::less<>>{};
constexpr auto less_equal = detail::binary_operator<std::less_equal<>>{};
constexpr auto greater = detail::binary_operator<std::greater<>>{};
constexpr auto greater_equal = detail::binary_operator<std::greater_equal<>>{};
constexpr auto divisible = detail::binary_operator<detail::divisible>{};

constexpr auto logical_and = detail::binary_operator<std::logical_and<>>{};
constexpr auto logical_or = detail::binary_operator<std::logical_or<>>{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ARITHMETIC_FUNCTORS_HPP_ */


