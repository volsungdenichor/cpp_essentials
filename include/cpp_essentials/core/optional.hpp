#ifndef CPP_ESSENTIALS_CORE_OPTIONAL_HPP_
#define CPP_ESSENTIALS_CORE_OPTIONAL_HPP_

#pragma once

#include <stdexcept>
#include <iostream>
#include <optional>

#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

struct none_t
{
};

static constexpr none_t none;

template <class T>
class optional;

class optional_access_error : public std::runtime_error
{
public:
    optional_access_error()
        : std::runtime_error{ "optional_access_error" }
    {
    }
};



namespace detail
{

struct value_tag { };
struct optional_tag { };

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<optional<T>> : std::true_type {};

template <class Adaptee>
class optional_base
{
private:
    using self_type = Adaptee;

public:
    template <class Func>
    auto then(Func func) const
    {
        using type = decltype(func(self().value()));

        using result_type = std::conditional_t
            < is_optional<type>::value
            , type
            , optional<type>>;

        return self().has_value() ? result_type{ func(self().value()) } : result_type{};
    }

    template <class T>
    decltype(auto) value_or(const T& default_value) const
    {
        return self().has_value() ? self().value() : default_value;
    }

    template <class Func>
    decltype(auto) value_or_eval(Func func) const
    {
        return self().has_value() ? self().value() : func();
    }

    template <class T>
    auto and_optional(const optional<T>& other) const
    {
        return self().has_value() ? other : optional<T>{};
    }

    template <class T>
    auto or_optional(const optional<T>& other) const
    {
        return self().has_value() ? self() : other;
    }

private:
    const self_type& self() const
    {
        return static_cast<const self_type&>(*this);
    }
};

} /* namespace detail */

template <class T>
class optional : public detail::optional_base<optional<T>>
{
public:
    using value_type = T;

    using const_reference = const T&;
    using reference = T & ;

    using const_pointer = const T*;
    using pointer = T * ;

    optional()
        : _has_value{ false }
    {
    }

    optional(none_t)
        : optional{}
    {
    }

    optional(const value_type& value)
        : optional{ value, detail::value_tag{} }
    {
    }

    optional(value_type&& value)
        : optional{ std::move(value), detail::value_tag{} }
    {
    }

    optional(const optional& other)
        : optional{ other, detail::optional_tag{} }
    {
    }

    optional(optional&& other)
        : optional{ std::move(other), detail::optional_tag{} }
    {
    }

#if 1
    optional(const std::optional<T>& other)
        : optional{ other, detail::optional_tag{} }
    {
    }

    optional(std::optional<T>&& other)
        : optional{ std::move(other), detail::optional_tag{} }
    {
    }
#endif

    ~optional()
    {
        destroy();
    }

    optional& operator =(optional other)
    {
        swap(other);
        return *this;
    }

    operator std::optional<T>() const
    {
        return has_value() ? std::optional<T>{ value() } : std::optional<T>{};
    }

    bool has_value() const
    {
        return _has_value;
    }

    bool operator !() const
    {
        return !has_value();
    }

    explicit operator bool() const
    {
        return has_value();
    }

    const_reference operator *() const
    {
        return *get();
    }

    reference operator *()
    {
        return *get();
    }

    const_pointer operator ->() const
    {
        return get();
    }

    pointer operator ->()
    {
        return get();
    }

    const_reference value() const
    {
        return *get();
    }

    reference value()
    {
        return *get();
    }

    const_pointer get() const
    {
        check_not_null();
        return do_get();
    }

    pointer get()
    {
        check_not_null();
        return do_get();
    }

    void swap(optional& other)
    {
        std::swap(_storage, other._storage);
        std::swap(_has_value, other._has_value);
    }

private:
    template <class U>
    optional(U&& value, detail::value_tag)
        : _has_value{ true }
    {
        construct(std::move(value));
    }

    template <class U>
    optional(U&& other, detail::optional_tag)
        : _has_value{ other }
    {
        if (other)
        {
            construct(std::move(*other));
        }
    }

    const_pointer do_get() const
    {
        return reinterpret_cast<const_pointer>(&_storage);
    }

    pointer do_get()
    {
        return reinterpret_cast<pointer>(&_storage);
    }

    void check_not_null() const
    {
        if (!has_value())
        {
            throw optional_access_error{};
        }
    }

    void destroy()
    {
        if (has_value())
        {
            do_get()->~value_type();
            _has_value = false;
        }
    }

    template <class U>
    void construct(U&& value)
    {
        new(&_storage) value_type(std::move(value));
    }

private:
    using storage_type = std::aligned_storage_t<sizeof(T), alignof(T)>;

    storage_type _storage;
    bool _has_value;
};


template <class T>
class optional<T&> : public detail::optional_base<optional<T&>>
{
public:
    using value_type = std::remove_const_t<T>;
    using reference = T&;
    using pointer = T*;

    optional()
        : _ptr{}
    {
    }

    optional(none_t)
        : optional{}
    {
    }

    optional(reference value)
        : _ptr{ &value }
    {
    }

    optional(const optional& other)
        : _ptr{ other._ptr }
    {
    }

    optional(value_type&&) = delete;

    optional(optional&& other)
        : optional{}
    {
        std::swap(_ptr, other._ptr);
    }

    optional& operator =(optional other)
    {
        swap(other);
        return *this;
    }

    bool has_value() const
    {
        return _ptr;
    }

    bool operator !() const
    {
        return !has_value();
    }

    explicit operator bool() const
    {
        return has_value();
    }

    reference operator *() const
    {
        return *get();
    }

    pointer operator ->() const
    {
        return get();
    }

    reference value() const
    {
        return *get();
    }

    pointer get() const
    {
        check_not_null();
        return _ptr;
    }

    void swap(optional& other)
    {
        std::swap(_ptr, other._ptr);
    }

private:
    void check_not_null() const
    {
        if (!has_value())
        {
            throw optional_access_error{};
        }
    }

private:
    pointer _ptr;
};


template <class T>
struct underlying_type
{
    using type = T;
};

template <class T>
struct underlying_type<optional<T>>
{
    using type = T;
};

template <class T>
struct underlying_type<std::optional<T>>
{
    using type = T;
};

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<optional<T>> : std::true_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
using underlying_type_t = typename underlying_type<T>::type;

template <class T>
static constexpr bool is_optional_v = is_optional<T>::value;


template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const optional<T>& lhs, const optional<U>& rhs)
{
    return (!lhs && !rhs) || (lhs && rhs && *lhs == *rhs);
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const optional<T>& lhs, const optional<U>& rhs)
{
    return !(lhs == rhs);
}



template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const optional<T>& lhs, const U& rhs)
{
    return lhs && *lhs == rhs;
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const optional<T>& lhs, const U& rhs)
{
    return !(lhs == rhs);
}



template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator ==(const T& lhs, const optional<U>& rhs)
{
    return rhs == lhs;
}

template <class T, class U, class = cc::EqualityCompare<T, U>>
bool operator !=(const T& lhs, const optional<U>& rhs)
{
    return !(lhs == rhs);
}



template <class T>
bool operator ==(const optional<T>& lhs, none_t)
{
    return !lhs;
}

template <class T>
bool operator !=(const optional<T>& lhs, none_t)
{
    return lhs.has_value();
}



template <class T>
bool operator ==(none_t, const optional<T>& rhs)
{
    return !rhs.has_value();
}

template <class T>
bool operator !=(none_t, const optional<T>& rhs)
{
    return rhs.has_value();
}


struct make_optional_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return optional<T> { std::move(value) };
    }

    template <class T>
    auto operator ()(bool condition, T value) const
    {
        return condition ? optional<T> { std::move(value) } : optional<T>{};
    }
};

static constexpr make_optional_fn make_optional = {};


struct eval_optional_fn
{
    template <class Func>
    auto operator ()(bool condition, Func func) const
    {
        using type = decltype(func());

        return condition ? optional<type> { func() } : optional<type>{};
    }
};

static constexpr eval_optional_fn eval_optional = {};



template <class T>
std::ostream& operator <<(std::ostream& os, const optional<T>& item)
{
    if (item)
    {
        os << *item;
    }
    else
    {
        os << "{none}";
    }

    return os;
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_OPTIONAL_HPP_ */
