#pragma once

#include <optional>
#include <millrind/core/type_traits.hpp>

namespace millrind::core
{

namespace detail
{

template <class T>
struct storage
{
    using const_reference = const T&;
    using reference = T&;

    constexpr storage() = default;

    constexpr storage(T value)
        : _value{ std::move(value) }
    {
    }

    template <class U, class = std::enable_if_t<std::is_constructible_v<T, U>>>
    constexpr storage(U value)
        : _value{ std::move(value) }
    {
    }

    constexpr storage(const storage&) = default;

    constexpr storage(storage&&) = default;

    constexpr const_reference ref() const
    {
        return _value;
    }

    constexpr reference ref()
    {
        return _value;
    }

    constexpr operator const_reference() const
    {
        return ref();
    }

    constexpr operator reference()
    {
        return ref();
    }

    void swap(storage& other)
    {
        std::swap(_value, other._value);
    }

    T _value;
};

template <class T>
struct storage<T&>
{
    using const_reference = T&;
    using reference = T&;

    constexpr storage(T& value)
        : _value{ &value }
    {
    }

    constexpr storage(const storage&) = default;

    constexpr storage(storage&&) = default;

    constexpr const_reference ref() const
    {
        return *_value;
    }

    constexpr reference ref()
    {
        return *_value;
    }

    constexpr operator const_reference() const
    {
        return ref();
    }

    constexpr operator reference()
    {
        return ref();
    }

    void swap(storage& other)
    {
        std::swap(_value, other._value);
    }

    T* _value;
};

template <class T>
struct optional_storage
{
    using value_type = T;

    using const_reference = const T&;
    using reference = T&;

    using const_pointer = const T*;
    using pointer = T*;

    constexpr optional_storage() = default;

    constexpr optional_storage(T value)
        : _value{ std::move(value) }
    {
    }

    template <class U, class = std::enable_if_t<std::is_constructible_v<T, U>>>
    constexpr optional_storage(U value)
        : _value{ std::move(value) }
    {
    }

    constexpr bool has_value() const
    {
        return _value.has_value();
    }

    constexpr const_reference ref() const&
    {
        return _value.value();
    }

    constexpr reference ref()&
    {
        return _value.value();
    }

    constexpr decltype(auto) ref()&&
    {
        return std::move(_value).value();
    }

    constexpr const_pointer ptr() const
    {
        return std::addressof(ref());
    }

    constexpr pointer ptr()
    {
        return std::addressof(ref());
    }

    void swap(optional_storage& other)
    {
        std::swap(_value, other._value);
    }

    std::optional<T> _value;
};

template <class T>
struct optional_storage<T&>
{
    using value_type = T;

    using const_reference = T&;
    using reference = T&;

    using const_pointer = T*;
    using pointer = T*;

    constexpr optional_storage() = default;

    constexpr optional_storage(T* value)
        : _value{ value }
    {
    }

    constexpr optional_storage(T& value)
        : _value{ &value }
    {
    }

    constexpr optional_storage(const std::reference_wrapper<T>& value)
        : optional_storage{ value.get() }
    {
    }

    constexpr bool has_value() const
    {
        return _value;
    }

    constexpr const_reference ref() const&
    {
        return *_value;
    }

    constexpr reference ref()&
    {
        return *_value;
    }

    constexpr decltype(auto) ref()&&
    {
        return *_value;
    }

    constexpr const_pointer ptr() const
    {
        return _value;
    }

    constexpr pointer ptr()
    {
        return _value;
    }

    void swap(optional_storage& other)
    {
        std::swap(_value, other._value);
    }

    T* _value;
};

} // namespace detail

} // namespace millrind::core
