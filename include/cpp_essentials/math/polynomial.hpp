#ifndef CPP_ESSENTIALS_MATH_POLYNOMIAL_HPP_
#define CPP_ESSENTIALS_MATH_POLYNOMIAL_HPP_

#pragma once

#include <array>

#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/math/functors.hpp>

namespace cpp_essentials::math
{

template <class T, size_t D>
class polynomial
{
private:
    using data_type = std::array<T, D + 1>;

public:
    using size_type = size_t;

    using value_type = T;

    using reference = typename data_type::reference;
    using const_reference = typename data_type::const_reference;    

    polynomial()
    {
        core::fill(_data, value_type {});
    }

    polynomial(std::initializer_list<value_type> init)
    {
        EXPECTS(init.size() == size());

        core::copy(init, _data.begin());
    }

    template <class U>
    polynomial(std::initializer_list<U> init)
    {
        EXPECTS(init.size() == size());

        core::transform(init, _data.begin(), core::cast<value_type>);
    }

    polynomial(const polynomial& other)
    {
        core::copy(other._data, _data.begin());
    }

    template <class U>
    polynomial(const polynomial<U, D>& other)
    {
        core::transform(other._data, _data.begin(), core::cast<value_type>);
    }


    constexpr size_type size() const
    {
        return _data.size();
    }


    const_reference operator [](size_type index) const
    {
        return _data[index];
    }

    reference operator [](size_type index)
    {
        return _data[index];
    }

    value_type operator ()(value_type x) const
    {
        auto result = value_type {};

        for (size_t i = 0; i < _data.size(); ++i)
        {
            result += _data[i] * value_type(pow(x, i));
        }

        return result;
    }

    data_type _data;
};

template <class T>
using linear_polynomial = polynomial<T, 1>;

template <class T>
using quadratic_polynomial = polynomial<T, 2>;

template <class T>
using cubic_polynomial = polynomial<T, 3>;



template <class T, size_t D>
std::ostream& operator <<(std::ostream& os, const polynomial<T, D>& item)
{
    os << "(";

    for (size_t d = 0; d < item.size(); ++d)
    {
        if (d != 0)
        {
            os << " ";
        }

        os << item[d];
    }

    os << ")";

    return os;
}



template <class T, size_t D>
auto operator +(const polynomial<T, D>& item) -> polynomial<T, D>
{
    return item;
}

template <class T, size_t D>
auto operator -(const polynomial<T, D>& item) -> polynomial<T, D>
{
    polynomial<T, D> result(item);
    core::transform(result._data, result._data.begin(), core::negate);
    return result;
}



template <class T, class U, size_t D1, size_t D2>
auto operator +(const polynomial<T, D1>& lhs, const polynomial<U, D2>& rhs) -> polynomial<cc::Add<T, U>, std::max(D1, D2)>
{
    polynomial<cc::Add<T, U>, std::max(D1, D2)> result;

    for (size_t i = 0; i < lhs.size(); ++i)
    {
        result[i] += lhs[i];
    }

    for (size_t i = 0; i < rhs.size(); ++i)
    {
        result[i] += rhs[i];
    }

    return result;
}

template <class T, class U, size_t D1, size_t D2, class = std::enable_if_t<(D1 >= D2)>>
auto& operator +=(polynomial<T, D1>& lhs, const polynomial<U, D2>& rhs)
{
    for (size_t i = 0; i < rhs.size(); ++i)
    {
        lhs[i] += rhs[i];
    }

    return lhs;
}



template <class T, class U, size_t D1, size_t D2>
auto operator -(const polynomial<T, D1>& lhs, const polynomial<U, D2>& rhs) -> polynomial<cc::Subtract<T, U>, std::max(D1, D2)>
{
    polynomial<cc::Subtract<T, U>, std::max(D1, D2)> result;

    for (size_t i = 0; i < lhs.size(); ++i)
    {
        result[i] += lhs[i];
    }

    for (size_t i = 0; i < rhs.size(); ++i)
    {
        result[i] -= rhs[i];
    }

    return result;
}

template <class T, class U, size_t D1, size_t D2, class = std::enable_if_t<(D1 >= D2)>>
auto& operator -=(polynomial<T, D1>& lhs, const polynomial<U, D2>& rhs)
{
    for (size_t i = 0; i < rhs.size(); ++i)
    {
        lhs[i] -= rhs[i];
    }

    return lhs;
}



template <class T, class U, size_t D, class = cc::Multiply<T, U>>
auto& operator *=(polynomial<T, D>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::multiplies(rhs));
    return lhs;
}

template <class T, class U, size_t D, class = cc::Multiply<T, U>>
auto operator *(const polynomial<T, D>& lhs, U rhs) -> polynomial<cc::Multiply<T, U>, D>
{
    polynomial<cc::Multiply<T, U>, D> result;
    core::transform(lhs._data, result._data.begin(), core::multiplies(rhs));
    return result;
}

template <class T, class U, size_t D, class = cc::Multiply<T, U>>
auto operator *(T lhs, const polynomial<T, D>& rhs) -> polynomial<cc::Multiply<T, U>, D>
{
    return rhs * lhs;
}



template <class T, class U, size_t D, class = cc::Divide<T, U>>
auto& operator /=(polynomial<T, D>& lhs, U rhs)
{
    core::transform(lhs._data, lhs._data.begin(), core::divides(rhs));
    return lhs;
}

template <class T, class U, size_t D, class = cc::Divide<T, U>>
auto operator /(const polynomial<T, D>& lhs, U rhs) -> polynomial<cc::Divide<T, U>, D>
{
    polynomial<cc::Divide<T, U>, D> result;
    core::transform(lhs._data, result._data.begin(), core::divides(rhs));
    return result;
}



template <class T, class U, size_t D1, size_t D2>
auto operator *(const polynomial<T, D1>& lhs, const polynomial<U, D2>& rhs) -> polynomial<cc::Multiply<T, U>, D1 + D2>
{
    polynomial<cc::Multiply<T, U>, D1 + D2> result;

    for (size_t i = 0; i < lhs.size(); ++i)
    {
        for (size_t j = 0; j < rhs.size(); ++j)
        {
            result[i + j] += lhs[i] * rhs[j];
        }
    }

    return result;
}



template <class T, class U, size_t D>
bool operator ==(const polynomial<T, D>& lhs, const polynomial<U, D>& rhs)
{
    return core::equal(lhs._data, rhs._data);
}

template <class T, class U, size_t D>
bool operator !=(const polynomial<T, D>& lhs, const polynomial<U, D>& rhs)
{
    return !(lhs == rhs);
}


struct root_fn
{
    template <class T>
    auto operator ()(const polynomial<T, 1>& polynomial) const -> core::optional<T>
    {
        return core::make_optional(polynomial[1], -polynomial[0] / polynomial[1]);
    }

    template <class T>
    auto operator ()(const polynomial<T, 2>& polynomial) const -> core::optional<std::array<T, 2>>
    {
        const auto [a, b, c] = polynomial._data;        

        const auto delta = sqr(b) - 4 * a * c;

        if (delta < 0)
        {
            return{};
        }

        const auto delta_root = sqrt(delta);
        const auto den = 2 * a;

        return std::array<T, 2> { (-b - delta_root) / den, (-b + delta_root) / den };
    }
};

static constexpr auto root = root_fn{};

} /* namespace cpp_essentials::math */

#endif /* CPP_ESSENTIALS_MATH_POLYNOMIAL_HPP_ */
