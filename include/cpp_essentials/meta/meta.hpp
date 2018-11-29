#ifndef CPP_ESSENTIALS_META_META_HPP_
#define CPP_ESSENTIALS_META_META_HPP_

#pragma once

#include <string_view>
#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::meta
{

using none = std::tuple<>;

struct unregistered_type {};
struct structure_type {};
struct enumeration_type {};

struct default_member {};
struct member_or_value {};

template <class Type, class Members>
struct type_info
{
    using type = Type;

    std::string_view name;
    Members members;
};

template <class T, class Type, class Tag>
struct member_info;

template <class T, class Type>
struct member_info<T, Type, default_member>
{
    member_info(std::string_view name, Type T::*ptr)
        : name{ name }
        , ptr{ ptr }
    {
    }

    std::string_view name;
    Type T::*ptr;
};

template <class T, class Type>
struct member_info<T, Type, member_or_value>
{
    member_info(std::string_view name, Type T::*ptr, Type default_value)
        : name{ name }
        , ptr{ ptr }
        , default_value{ std::move(default_value) }
    {
    }

    std::string_view name;
    Type T::*ptr;
    Type default_value;
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

template <class Type, class T>
auto member(Type T::*ptr, std::string_view name) -> member_info<T, Type, default_member>
{
    return { name, ptr };
}

template <class Type, class T>
auto member_or(Type T::*ptr, std::string_view name, Type default_value) -> member_info<T, Type, member_or_value>
{
    return { name, ptr, std::move(default_value) };
}

template <class Type, class T>
auto member_or_default(Type T::*ptr, std::string_view name) -> member_info<T, Type, member_or_value>
{
    return member_or(ptr, name, Type{});
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
using type_info_type = typename std::decay_t<decltype(get_type_info<T>())>::type;

template <class T>
static constexpr bool is_registered = !std::is_same_v<unregistered_type, type_info_type<T>>;

template <class T>
static constexpr bool is_structure = std::is_same_v<structure_type, type_info_type<T>>;

template <class T>
static constexpr bool is_enumeration = std::is_same_v<enumeration_type, type_info_type<T>>;


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

} /* namespace cpp_essentials::meta */

#endif /* CPP_ESSENTIALS_META_META_HPP_ */