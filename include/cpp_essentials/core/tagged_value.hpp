#ifndef CPP_ESSENTIALS_CORE_TAGGED_VALUE_HPP_
#define CPP_ESSENTIALS_CORE_TAGGED_VALUE_HPP_

#pragma once

#include <iostream>
#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

template <class T, class Tag>
class tagged_value
{
public:
    using value_type = T;
    using tag_type = Tag;

    using const_reference = const T&;
    using reference = T&;

    constexpr tagged_value() = default;

    constexpr explicit tagged_value(value_type value)
        : _value{ std::move(value) }
    {
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    constexpr tagged_value(const tagged_value<U, tag_type>& other)
        : tagged_value{ static_cast<value_type>(other.get()) }
    {
    }

    constexpr tagged_value& operator =(tagged_value other)
    {
        std::swap(_value, other._value);
        return *this;
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    constexpr tagged_value& operator =(const tagged_value<U, tag_type>& other)
    {
        return *this = tagged_value{ other };
    }

    template <class U = T, CONCEPT = cc::Convertible<U, bool>>
    constexpr explicit operator bool() const
    {
        return static_cast<bool>(_value);
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    constexpr operator tagged_value<U, tag_type>() const
    {
        return *this;
    }

    template <class U, CONCEPT = cc::Convertible<U, value_type>>
    constexpr tagged_value<U, tag_type> as() const
    {
        return *this;
    }

    constexpr const_reference get() const
    {
        return _value;
    }

private:
    value_type _value;
};



template <class T, class Tag>
std::ostream& operator <<(std::ostream& os, const tagged_value<T, Tag>& item)
{
    os << item.get();

    if constexpr (cc::is_detected_v<cc::HasOstreamOperator, Tag>)
    {
        static const Tag tag_instance{};

        os << "{" << tag_instance << "}";
    }
    return os;
}


template <class T, class U, class Tag, CONCEPT = cc::EqualityCompare<T, U>>
bool operator ==(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() == rhs.get();
}

template <class T, class U, class Tag, CONCEPT = cc::EqualityCompare<T, U>>
bool operator !=(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() != rhs.get();
}


template <class T, class U, class Tag, CONCEPT = cc::LessThanCompare<T, U>>
bool operator <(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() < rhs.get();
}

template <class T, class U, class Tag, CONCEPT = cc::LessThanCompare<T, U>>
bool operator >(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() > rhs.get();
}

template <class T, class U, class Tag, CONCEPT = cc::LessThanCompare<T, U>>
bool operator <=(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() <= rhs.get();
}

template <class T, class U, class Tag, CONCEPT = cc::LessThanCompare<T, U>>
bool operator >=(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    return lhs.get() >= rhs.get();
}


template <class T, class Tag>
auto operator +(const tagged_value<T, Tag>& item)
{
    return item;
}

template <class T, class Tag>
auto operator -(const tagged_value<T, Tag>& item)
{
    using return_type = tagged_value<T, Tag>;
    return return_type{ -item.get() };
}


template <class T, class U, class Tag, CONCEPT = cc::Add<T, U>>
auto operator +(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    using return_type = tagged_value<cc::Add<T, U>, Tag>;
    return return_type{ lhs.get() + rhs.get() };
}

template <class T, class U, class Tag, CONCEPT = cc::Subtract<T, U>>
auto operator -(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    using return_type = tagged_value<cc::Subtract<T, U>, Tag>;
    return return_type{ lhs.get() - rhs.get() };
}

template <class T, class U, class Tag, CONCEPT = cc::Multiply<T, U>, CONCEPT = cc::Arithmetic<T>>
auto operator *(const tagged_value<T, Tag>& lhs, const U& rhs)
{
    using return_type = tagged_value<cc::Multiply<T, U>, Tag>;
    return return_type{ lhs.get() * rhs };
}

template <class T, class U, class Tag, CONCEPT = cc::Multiply<T, U>, CONCEPT = cc::Arithmetic<T>>
auto operator *(const T& lhs, const tagged_value<U, Tag>& rhs)
{
    return rhs * lhs;
}

template <class T, class U, class Tag1, class Tag2, CONCEPT = cc::Multiply<T, U>, CONCEPT = cc::Multiply<Tag1, Tag2>>
auto operator *(const tagged_value<T, Tag1>& lhs, const tagged_value<U, Tag2>& rhs)
{
    using return_type = tagged_value<cc::Multiply<T, U>, cc::Multiply<Tag1, Tag2>>;
    return return_type{ lhs.get() * rhs.get() };
}

template <class T, class U, class Tag, CONCEPT = cc::Divide<T, U>, CONCEPT = cc::Arithmetic<U>>
auto operator /(const tagged_value<T, Tag>& lhs, const U& rhs)
{
    using return_type = tagged_value<cc::Divide<T, U>, Tag>;
    return return_type{ lhs.get() / rhs };
}

template <class T, class U, class Tag, CONCEPT = cc::Divide<T, U>, CONCEPT = cc::Divide<int, Tag>, CONCEPT = cc::Arithmetic<T>>
auto operator /(const T& lhs, const tagged_value<U, Tag>& rhs)
{
    using return_type = tagged_value<cc::Divide<T, U>, cc::Divide<int, Tag>>;
    return return_type{ lhs / rhs.get() };
}

template <class T, class U, class Tag1, class Tag2, CONCEPT = cc::Divide<T, U>, CONCEPT = cc::Divide<Tag1, Tag2>>
auto operator /(const tagged_value<T, Tag1>& lhs, const tagged_value<U, Tag2>& rhs)
{
    using return_type = tagged_value<cc::Divide<T, U>, cc::Divide<Tag1, Tag2>>;
    return return_type{ lhs.get() / rhs.get() };
}

template <class T, class U, class Tag, CONCEPT = cc::Divide<T, U>>
auto operator /(const tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    using return_type = cc::Divide<T, U>;
    return return_type{ lhs.get() / rhs.get() };
}


template <class T, class U, class Tag, CONCEPT = cc::Add<T, U>>
auto& operator +=(tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

template <class T, class U, class Tag, CONCEPT = cc::Subtract<T, U>>
auto& operator -=(tagged_value<T, Tag>& lhs, const tagged_value<U, Tag>& rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

template <class T, class U, class Tag, CONCEPT = cc::Multiply<T, U>, CONCEPT = cc::Arithmetic<U>>
auto& operator *=(tagged_value<T, Tag>& lhs, const U& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

template <class T, class U, class Tag, CONCEPT = cc::Divide<T, U>, CONCEPT = cc::Arithmetic<U>>
auto& operator /=(tagged_value<T, Tag>& lhs, const U& rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

template <class T, class Tag, CONCEPT = cc::Increment<T>>
auto& operator ++(tagged_value<T, Tag>& item)
{
    auto val = item.get();
    ++val;
    item = tagged_value<T, Tag>{ std::move(val) };
    return item;
}

template <class T, class Tag, CONCEPT = cc::Decrement<T>>
auto& operator --(tagged_value<T, Tag>& item)
{
    auto val = item.get();
    --val;
    item = tagged_value<T, Tag>{ std::move(val) };
    return item;
}

template <class T, class Tag, CONCEPT = cc::Increment<T>>
auto operator ++(tagged_value<T, Tag>& item, int)
{
    auto temp = item;
    ++item;
    return temp;
}

template <class T, class Tag, CONCEPT = cc::Decrement<T>>
auto operator --(tagged_value<T, Tag>& item, int)
{
    auto temp = item;
    --item;
    return temp;
}

} /* namespace cpp_essentials::core */

namespace std
{

template <class T, class Tag>
struct is_floating_point<::cpp_essentials::core::tagged_value<T, Tag>> : std::is_floating_point<T> {};

template <class T, class Tag>
struct is_integral<::cpp_essentials::core::tagged_value<T, Tag>> : std::is_integral<T> {};

template <class T, class Tag>
struct is_signed<::cpp_essentials::core::tagged_value<T, Tag>> : std::is_signed<T> {};

template <class T, class Tag>
struct is_unsigned<::cpp_essentials::core::tagged_value<T, Tag>> : std::is_unsigned<T> {};


template <class T, class Tag>
struct hash<::cpp_essentials::core::tagged_value<T, Tag>>
{
    using argument_type = ::cpp_essentials::core::tagged_value<T, Tag>;
    using result_type = std::size_t;

    result_type operator()(const argument_type& item) const noexcept
    {
        static constexpr hash<T> inner_hash = {};
        return inner_hash(item.get());
    }
};


template <class T, class Tag>
struct numeric_limits<::cpp_essentials::core::tagged_value<T, Tag>>
{
    using value_type = ::cpp_essentials::core::tagged_value<T, Tag>;

    static constexpr value_type min()
    {
        return value_type{ numeric_limits<T>::min() };
    }

    static constexpr value_type max()
    {
        return value_type{ numeric_limits<T>::max() };
    }

    static constexpr value_type lowest()
    {
        return value_type{ numeric_limits<T>::lowest() };
    }

    static constexpr value_type epsilon()
    {
        return value_type{ numeric_limits<T>::epsilon() };
    }

    static constexpr value_type round_error()
    {
        return value_type{ numeric_limits<T>::round_error() };
    }

    static constexpr value_type denorm_min()
    {
        return value_type{ numeric_limits<T>::denorm_min() };
    }

    static constexpr value_type infinity()
    {
        return value_type{ numeric_limits<T>::infinity() };
    }

    static constexpr value_type quiet_NaN()
    {
        return value_type{ numeric_limits<T>::quiet_NaN() };
    }

    static constexpr value_type signaling_NaN()
    {
        return value_type{ numeric_limits<T>::signaling_NaN() };
    }
};

} /* namespace std */

#endif /* CPP_ESSENTIALS_CORE_TAGGED_VALUE_HPP_ */
