#ifndef CPP_ESSENTIALS_META_META_HPP_
#define CPP_ESSENTIALS_META_META_HPP_

#pragma once

#include <string_view>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/serialization.hpp>

namespace cpp_essentials::meta
{

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
static constexpr bool is_optional_v = is_optional<T>::value;

template <class T>
struct underlying_type
{
    using type = T;
};

template <class T>
struct underlying_type<std::optional<T>>
{
    using type = T;
};

template <class T>
using underlying_type_t = typename underlying_type<T>::type;

using none = std::tuple<>;

struct unregistered_type {};
struct structure_type {};
struct enumeration_type {};

struct mandatory_member {};

template <class T = void>
struct default_member
{
    T value;
};

template <>
struct default_member<void> {};


template <class Type, class Members>
struct type_info
{
    using type = Type;

    std::string_view name;
    Members members;
};

template <class T>
default_member<T> default_value(T value)
{
    return { std::move(value) };
}

inline default_member<> default_value()
{
    return {};
}

inline mandatory_member mandatory()
{
    return {};
}

template <class T, class Type, class Extra>
struct member_info
{
    member_info(std::string_view name, Type T::*ptr, Extra extra)
        : name{ name }
        , ptr{ ptr }
        , extra{ std::move(extra) }
    {
    }

    std::string_view name;
    Type T::*ptr;
    Extra extra;
};

template <class T>
struct enum_info
{
    enum_info(std::string_view name, T value)
        : name{ name }
        , value{ value }
    {
    }

    std::string_view name;
    T value;
};

template <class T, class Type, class Extra = std::conditional_t<is_optional_v<Type>, default_member<>, mandatory_member>>
auto member(Type T::*ptr, std::string_view name, Extra extra = {}) -> member_info<T, Type, Extra>
{
    return { name, ptr, std::move(extra) };
}

template <class T>
auto enum_value(T value, std::string_view name) -> enum_info<T>
{
    return { name, value };
}

template <class T>
auto register_type()
{
    return type_info<unregistered_type, none>{};
}

template <class T>
const auto& get_type_info()
{
    static const auto info = register_type<T>();
    return info;
}

template <class T>
using type_info_type = std::decay_t<decltype(get_type_info<T>())>;

template <class T>
static constexpr bool is_registered = !std::is_same_v<unregistered_type, typename type_info_type<T>::type>;

template <class T>
static constexpr bool is_structure = std::is_same_v<structure_type, typename type_info_type<T>::type>;

template <class T>
static constexpr bool is_enumeration = std::is_same_v<enumeration_type, typename type_info_type<T>::type>;


template <class... Members>
auto structure(std::string_view name, Members&&... members)
{
    using members_tuple = std::tuple<Members...>;
    return type_info<structure_type, members_tuple>{ name, members_tuple{ std::forward<Members>(members)... } };
}

template <class... Members>
auto enumeration(std::string_view name, Members&&... members)
{
    using members_tuple = std::tuple<Members...>;
    return type_info<enumeration_type, members_tuple>{ name, members_tuple{ std::forward<Members>(members)... } };
}

template <class T, class Func>
void for_each(Func&& func)
{
    static_assert(is_registered<T>, "Type not registered");

    core::visit(get_type_info<T>().members, func);
}

template <class T, class Func>
void for_each(const T& item, Func&& func)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_structure<T>, "Structure required");

    core::visit(get_type_info<T>().members, [&item, &func](const auto& info)
    {
        func(info, item.*info.ptr);
    });
}

template <class T, class Func>
void for_each(T& item, Func&& func)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_structure<T>, "Structure required");

    core::visit(get_type_info<T>().members, [&item, &func](const auto& info)
    {
        func(info, item.*info.ptr);
    });
}

template <class Type, class T>
std::string_view name_of(Type T::*ptr)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_structure<T>, "Structure required");

    std::string_view result = {};
    auto offset = core::offset_of(ptr);
    for_each<T>([offset, &result](const auto& info)
    {
        if (core::offset_of(info.ptr) == offset)
        {
            result = info.name;
        }
    });
    return result;
}

template <class T>
std::string_view name_of(T value)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_enumeration<T>, "Enumeration required");

    std::string_view result = {};
    for_each<T>([&value, &result](const auto& info)
    {
        if (info.value == value)
        {
            result = info.name;
        }
    });
    return result;
}

template <class T>
std::string_view name_of()
{
    static_assert(is_registered<T>, "Type not registered");

    return get_type_info<T>().name;
}

template <class T, class Func>
void invoke(const T& item, std::string_view member_name, Func&& func)
{
    bool found = false;
    for_each(item, [&member_name, &func, &found](const auto& info, const auto& value)
    {
        if (info.name == member_name)
        {
            func(value);
            found = true;
        }
    });

    if (!found)
    {
        throw std::runtime_error{ core::to_string("Member '", member_name, "' not found in '", name_of<T>(), "'") };
    }
}

template <class T, class Func>
void invoke(T& item, std::string_view member_name, Func&& func)
{
    bool found = false;
    for_each(item, [&member_name, &func, &found](const auto& info, auto& value)
    {
        if (info.name == member_name)
        {
            func(value);
            found = true;
        }
    });

    if (!found)
    {
        throw std::runtime_error{ core::to_string("Member '", member_name, "' not found in '", name_of<T>(), "'") };
    }
}

template <class T>
std::optional<T> try_parse_enum(std::string_view text)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_enumeration<T>, "Enumeration required");

    std::optional<T> result = std::nullopt;
    for_each<T>([&result, &text](const auto& info)
    {
        if (info.name == text)
        {
            result = info.value;
        }
    });
    return result;
}

template <class T>
T parse_enum(std::string_view text)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_enumeration<T>, "Enumeration required");

    const auto result = try_parse_enum<T>(text);
    if (!result)
    {
        throw std::runtime_error{ "Unknown enum value" };
    }
    return *result;
}

} /* namespace cpp_essentials::meta */

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_structure<T>>>
std::ostream& operator <<(std::ostream& os, const T& item)
{
    os << "(";
    bool first = true;
    meta::for_each(item, [&os, &first](const auto& /* info */, const auto& value)
    {
        if (!first)
        {
            os << ";";
        }
        first = false;
        os << value;
    });
    os << ")";
    return os;
}

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_enumeration<T>>>
std::ostream& operator <<(std::ostream& os, T item)
{
    return os << meta::name_of(item);
}

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_enumeration<T>>>
std::istream& operator >>(std::istream& is, T& item)
{
    std::string temp;
    is >> temp;
    item = meta::template parse_enum<T>(temp);
    return is;
}

#endif /* CPP_ESSENTIALS_META_META_HPP_ */