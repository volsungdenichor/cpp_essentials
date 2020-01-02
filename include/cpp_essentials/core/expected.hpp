#ifndef CPP_ESSENTIALS_CORE_EXPECTED_HPP_
#define CPP_ESSENTIALS_CORE_EXPECTED_HPP_

#include <variant>
#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/optional.hpp>

#pragma once

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
struct unexpected
{
    T value;
};

struct make_unexpected_fn
{
    template <class T>
    unexpected<T> operator ()(T item) const
    {
        return { std::move(item) };
    }
};

} /* namespace detail */

static constexpr auto make_unexpected = detail::make_unexpected_fn{};

template <class T, class E>
class expected
{
private:
    using unexpected_type = detail::unexpected<E>;

public:
    using value_type = T;
    using error_type = E;

    expected() = default;

    template <class U>
    expected(U value)
        : _storage{ value_type(std::move(value)) }
    {
    }

    template <class U>
    expected(detail::unexpected<U> other)
        : _storage{ unexpected_type{ error_type{ std::move(other.value) } } }
    {
    }

    template <class OT, class OE>
    expected(const expected<OT, OE>& other)
    {
        if (other.has_value())
        {
            *this = other.value();
        }
        else
        {
            *this = other.unexpected();
        }
    }

    expected(const expected&) = default;
    expected(expected&&) = default;

    expected& operator =(expected other)
    {
        std::swap(_storage, other._storage);
        return *this;
    }

    template <class U>
    operator optional<U>() const
    {
        return has_value() ? optional<U>{ value() } : optional<U>{};
    }

    explicit operator bool() const
    {
        return has_value();
    }

    const value_type& operator *() const
    {
        return value();
    }

    const value_type* operator ->() const
    {
        return &value();
    }

    const unexpected_type& unexpected() const
    {
        EXPECTS(!has_value());
        return std::get<unexpected_type>(_storage);
    }

    const value_type& value() const
    {
        EXPECTS(has_value());
        return std::get<value_type>(_storage);
    }

    const error_type& error() const
    {
        return unexpected().value;
    }

    bool has_value() const
    {
        return std::holds_alternative<value_type>(_storage);
    }

    bool has_error() const
    {
        return !has_value();
    }

private:
    using storage_type = std::variant<value_type, unexpected_type>;
    storage_type _storage;
};

template <class T, class E>
class expected<T&, E>
{
private:
    using unexpected_type = detail::unexpected<E>;

public:
    using value_type = T;
    using error_type = E;

    expected() = default;

    expected(value_type& value)
        : _storage{ &value }
    {
    }

    template <class U>
    expected(detail::unexpected<U> other)
        : _storage{ unexpected_type{ error_type{ std::move(other.value) } } }
    {
    }

    template <class OE>
    expected(const expected<T&, OE>& other)
    {
        if (other.has_value())
        {
            *this = other.value();
        }
        else
        {
            *this = other.unexpected();
        }
    }

    expected(const expected&) = default;
    expected(expected&&) = default;

    expected& operator =(expected other)
    {
        std::swap(_storage, other._storage);
        return *this;
    }

    template <class U>
    operator optional<U>() const
    {
        return has_value() ? optional<U>{ value() } : optional<U>{};
    }

    explicit operator bool() const
    {
        return has_value();
    }

    value_type& operator *() const
    {
        return value();
    }

    value_type* operator ->() const
    {
        return &value();
    }

    const unexpected_type& unexpected() const
    {
        EXPECTS(!has_value());
        return std::get<unexpected_type>(_storage);
    }

    value_type& value() const
    {
        EXPECTS(has_value());
        return *std::get<value_type*>(_storage);
    }

    const error_type& error() const
    {
        return unexpected().value;
    }

    bool has_value() const
    {
        return std::holds_alternative<value_type*>(_storage);
    }

    bool has_error() const
    {
        return !has_value();
    }

private:
    using storage_type = std::variant<value_type*, unexpected_type>;
    storage_type _storage;
};

template <class T, class E1, class U, class E2, class = cc::EqualityCompare<T, U>>
bool operator ==(const expected<T, E1>& lhs, const expected<U, E2>& rhs)
{
    return (!lhs && !rhs) || (lhs && rhs && *lhs == *rhs);
}

template <class T, class E1, class U, class E2, class = cc::EqualityCompare<T, U>>
bool operator !=(const expected<T, E1>& lhs, const expected<U, E2>& rhs)
{
    return !(lhs == rhs);
}



template <class T, class E, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const expected<T, E>& lhs, const U& rhs)
{
    return lhs && *lhs == rhs;
}

template <class T, class E, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const expected<T, E>& lhs, const U& rhs)
{
    return !(lhs == rhs);
}



template <class T, class U, class E, class = cc::EqualityCompare<T, U>>
bool operator ==(const T& lhs, const expected<U, E>& rhs)
{
    return rhs == lhs;
}

template <class T, class U, class E, class = cc::EqualityCompare<T, U>>
bool operator !=(const T& lhs, const expected<U, E>& rhs)
{
    return !(lhs == rhs);
}

template <class T, class E>
struct is_optional<expected<T, E>> : std::true_type {};

template <class T, class E>
struct underlying_type<expected<T, E>>
{
    using type = T;
};

template <class T, class E>
std::ostream& operator <<(std::ostream& os, const expected<T, E>& item)
{
    if (item)
    {
        return os << item.value();
    }
    else
    {
        return os << item.error();
    }
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_EXPECTED_HPP_ */


