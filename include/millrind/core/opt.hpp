#pragma once

#include <optional>
#include <millrind/core/detail/storage.hpp>


namespace millrind::core
{

static constexpr auto none = std::nullopt;

template <class T>
class opt;

namespace detail
{

template <class T>
struct is_opt : std::false_type {};

template <class T>
struct is_opt<opt<T>> : std::true_type {};

template <class Func, class Opt>
constexpr auto opt_map(Func&& func, Opt&& item)
{
    using type = decltype(invoke_func(FORWARD(func), *FORWARD(item)));
    if (item)
        return opt<type>{ invoke_func(FORWARD(func), *FORWARD(item)) };
    else
        return opt<type>{};
}

template <class O, class Pred, class Opt>
constexpr auto opt_filter(Pred&& pred, Opt&& item)
{
    if (item && invoke_func(FORWARD(pred), *FORWARD(item)))
        return O{ FORWARD(item) };
    else
        return O{};
}

template <class Func, class Opt>
constexpr auto opt_filter_map(Func&& func, Opt&& item)
{
    using type = decltype(invoke_func(FORWARD(func), *FORWARD(item)));
    static_assert(is_opt<type>{}, "opt result required");
    if (item)
        return type{ invoke_func(FORWARD(func), *FORWARD(item)) };
    else
        return type{};
}

template <class Opt, class T>
constexpr decltype(auto) opt_value_or(Opt&& item, T&& default_value)
{
    return item
        ? *FORWARD(item)
        : FORWARD(default_value);
}

template <class Opt, class Func>
constexpr decltype(auto) opt_value_or_else(Opt&& item, Func&& func)
{
    return item
        ? *FORWARD(item)
        : invoke_func(FORWARD(func));
}

template <class Opt, class Func>
constexpr auto opt_or_else(Opt&& item, Func&& func)
{
    using type = decltype(invoke_func(FORWARD(func)));
    static_assert(is_opt<type>{}, "opt result required");

    return item
        ? type{ *FORWARD(item) }
    : type{ invoke_func(FORWARD(func)) };
}

template <class Exception, class = std::enable_if_t<std::is_base_of_v<std::exception, Exception>>>
constexpr Exception&& to_exception(Exception&& exception)
{
    return FORWARD(exception);
}

inline std::runtime_error to_exception(std::string message)
{
    return std::runtime_error{ std::move(message) };
}

template <class Opt, class Exception>
constexpr decltype(auto) opt_value_or_throw(Opt&& item, Exception&& exception)
{
    if (!item)
    {
        if constexpr (std::is_invocable_v<Exception>)
        {
            throw to_exception(invoke_func(exception));
        }
        else
        {
            throw to_exception(FORWARD(exception));
        }
    }

    return *FORWARD(item);
}

} // namespace detail

template <class T>
class opt
{
private:
    using storage_type = detail::optional_storage<T>;

public:
    using reference = typename storage_type::reference;
    using const_reference = typename storage_type::const_reference;

    using pointer = typename storage_type::pointer;
    using const_pointer = typename storage_type::const_pointer;

    constexpr opt()
        : _storage{}
    {
    }

    constexpr opt(std::nullopt_t)
        : opt{}
    {
    }

    template <class U>
    constexpr opt(U&& value)
        : _storage{ FORWARD(value) }
    {
    }

    template <class U>
    constexpr opt(opt<U> other)
        : _storage{ other ? storage_type{ std::move(*other) } : storage_type{} }
    {
    }

    template <class U>
    constexpr opt(std::optional<U> other)
        : _storage{ other ? storage_type{ std::move(*other) } : storage_type{} }
    {
    }

    constexpr opt(const opt&) = default;

    constexpr opt(opt&&) = default;

    constexpr opt& operator=(opt other)
    {
        _storage.swap(other._storage);
        return *this;
    }

    constexpr operator bool() const
    {
        return _storage.has_value();
    }


    constexpr const_reference operator *() const&
    {
        return _storage.ref();
    }

    constexpr reference operator *()&
    {
        return _storage.ref();
    }

    constexpr decltype(auto) operator *()&&
    {
        return std::move(_storage).ref();
    }


    constexpr const_pointer operator ->() const
    {
        return _storage.ptr();
    }

    constexpr pointer operator ->()
    {
        return _storage.ptr();
    }


    constexpr bool has_value() const
    {
        return *this;
    }


    constexpr const_reference value() const&
    {
        return **this;
    }

    constexpr reference value()&
    {
        return **this;
    }

    constexpr decltype(auto) value()&&
    {
        return *std::move(*this);
    }


    template <class U>
    constexpr decltype(auto) value_or(U&& default_value) const&
    {
        return detail::opt_value_or(*this, FORWARD(default_value));
    }

    template <class U>
    constexpr decltype(auto) value_or(U&& default_value)&&
    {
        return detail::opt_value_or(std::move(*this), FORWARD(default_value));
    }


    template <class Func>
    constexpr decltype(auto) value_or_else(Func&& func) const&
    {
        return detail::opt_value_or_else(*this, FORWARD(func));
    }

    template <class Func>
    constexpr decltype(auto) value_or_else(Func&& func)&&
    {
        return detail::opt_value_or_else(std::move(*this), FORWARD(func));
    }


    template <class Exception>
    constexpr decltype(auto) value_or_throw(Exception&& exception) const&
    {
        return detail::opt_value_or_throw(*this, FORWARD(exception));
    }

    template <class Exception>
    constexpr decltype(auto) value_or_throw(Exception&& exception)&&
    {
        return detail::opt_value_or_throw(std::move(*this), FORWARD(exception));
    }


    template <class Func>
    constexpr auto or_else(Func&& func) const&
    {
        return detail::opt_or_else(*this, FORWARD(func));
    }

    template <class Func>
    constexpr auto or_else(Func&& func)&&
    {
        return detail::opt_or_else(std::move(*this), FORWARD(func));
    }


    template <class Func>
    constexpr auto map(Func&& func) const&
    {
        return detail::opt_map(FORWARD(func), *this);
    }

    template <class Func>
    constexpr auto map(Func&& func)&&
    {
        return detail::opt_map(FORWARD(func), std::move(*this));
    }


    template <class Pred>
    constexpr auto filter(Pred&& pred) const&
    {
        return detail::opt_filter<opt>(FORWARD(pred), *this);
    }

    template <class Pred>
    constexpr auto filter(Pred&& pred)&&
    {
        return detail::opt_filter<opt>(FORWARD(pred), std::move(*this));
    }


    template <class Func>
    constexpr auto filter_map(Func&& func) const&
    {
        return detail::opt_filter_map(FORWARD(func), *this);
    }

    template <class Func>
    constexpr auto filter_map(Func&& func)&&
    {
        return detail::opt_filter_map(FORWARD(func), std::move(*this));
    }

private:
    storage_type _storage;
};

template <class T, class U, class = EqualityCompare<T, U>>
constexpr bool operator==(const opt<T>& lhs, const opt<U>& rhs)
{
    return (!lhs && !rhs) || (lhs && rhs && lhs == rhs);
}

template <class T, class U, class = EqualityCompare<T, U>>
constexpr bool operator!=(const opt<T>& lhs, const opt<U>& rhs)
{
    return !(lhs == rhs);
}

template <class T>
constexpr bool operator==(const opt<T>& lhs, std::nullopt_t)
{
    return !lhs;
}

template <class T>
constexpr bool operator!=(const opt<T>& lhs, std::nullopt_t)
{
    return lhs;
}

template <class T>
constexpr bool operator==(std::nullopt_t, const opt<T>& rhs)
{
    return !rhs;
}

template <class T>
constexpr bool operator!=(std::nullptr_t, const opt<T>& rhs)
{
    return rhs;
}

template <class T, class U>
constexpr auto operator|(const opt<T>& lhs, const opt<U>& rhs)
{
    using R = std::common_type_t<T, U>;

    if (lhs)
        return opt<R>{ lhs };
    else
        return opt<R>{ rhs };
}

template <class T, class U>
constexpr auto operator&(const opt<T>& lhs, const opt<U>& rhs)
{
    using R = std::common_type_t<T, U>;

    if (!lhs)
        return opt<R>{ lhs };
    else
        return opt<R>{ rhs };
}

template <class T>
std::ostream& operator <<(std::ostream& os, const opt<T>& item)
{
    if (item)
        return os << "some(" << *item << ")";
    else
        return os << "none";
}

struct some_fn
{
    template <class T>
    constexpr opt<T> operator()(T value) const
    {
        return { std::move(value) };
    }

    template <class T>
    constexpr opt<T&> operator()(std::reference_wrapper<T> value) const
    {
        return { value.get() };
    }
};

static constexpr auto some = some_fn{};

} // namespace millrind::core