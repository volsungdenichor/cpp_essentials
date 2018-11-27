#ifndef CPP_ESSENTIALS_META_META_HPP_
#define CPP_ESSENTIALS_META_META_HPP_

#pragma once

#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::meta
{

template <class T>
using is_struct = std::enable_if_t<std::is_class_v<T>>;

template <class T>
using is_enum = std::enable_if_t<std::is_enum_v<T>>;


template <class Type, class T>
struct member_info
{
    member_info(const char* name, Type T::*ptr)
        : name{ name }
        , ptr{ ptr }
    {
    }

    const char* name;
    Type T::*ptr;
};

template <class T>
struct enum_info
{
    enum_info(const char* name, T value)
        : name{ name }
        , value{ value }
    {
    }

    const char* name;
    T value;
};

template <class Type, class T>
auto member(Type T::*ptr, const char* name) -> member_info<Type, T>
{
    return { name, ptr };
}

template <class T>
auto enum_value(T value, const char* name) -> enum_info<T>
{
    return { name, value };
}

template <class Members>
struct member_list
{
    Members members;
};

template <class Values>
struct enum_value_list
{
    Values values;
};

template <class... Members>
auto members(Members&&... members)
{
    return member_list<std::tuple<Members...>>{ std::make_tuple(std::forward<Members>(members)...) };
}

template <class... Values>
auto enum_values(Values&&... values)
{
    return enum_value_list<std::tuple<Values...>>{ std::make_tuple(std::forward<Values>(values)...) };
}

using empty_tuple = std::tuple<>;

template <class T>
auto register_members()
{
    return empty_tuple{};
}

template <class T>
auto register_enum_values()
{
    return empty_tuple{};
}

template <class T>
auto register_name()
{
    return nullptr;
}

template <class T>
constexpr bool members_registered = !std::is_same_v<decltype(register_members<T>()), empty_tuple>;

template <class T>
constexpr bool enum_values_registered = !std::is_same_v<decltype(register_enum_values<T>()), empty_tuple>;

template <class T>
constexpr bool name_registered = !std::is_same_v<decltype(register_name<T>()), std::nullptr_t>;

template <class T>
constexpr bool type_registered = members_registered<T> || enum_values_registered<T>;


template <class T>
const auto& get_members()
{
    static_assert(members_registered<T>, "Members not registrered");
    static const auto m = register_members<T>();
    return m.members;
}

template <class T>
const auto& get_enum_values()
{
    static_assert(enum_values_registered<T>, "Enum values not registrered");
    static const auto m = register_enum_values<T>();
    return m.values;
}

template <class Type, class T>
const char* name_of(Type T::*ptr)
{
    const char* result = nullptr;
    auto offset = core::offset_of(ptr);
    core::visit(get_members<T>(), [offset, &result](const auto& info)
    {
        if (core::offset_of(info.ptr) == offset)
        {
            result = info.name;
        }
    });
    return result;
}

template <class T>
const char* name_of(T value)
{
    const char* result = nullptr;
    core::visit(get_enum_values<T>(), [&value, &result](const auto& info)
    {
        if (info.value == value)
        {
            result = info.name;
        }
    });
    return result;
}

template <class T>
const char* name_of()
{
    static_assert(name_registered<T>, "Type name not registrered");
    static const char* value = register_name<T>();
    return value;
}

template <class T, class Func>
void for_each(Func&& func)
{
    if constexpr (std::is_class_v<T>)
    {
        core::visit(get_members<T>(), func);
    }
    else if constexpr (std::is_enum_v<T>)
    {
        core::visit(get_enum_values<T>(), func);
    }
}

template <class T, class Func>
void for_each(const T& item, Func&& func)
{
    core::visit(get_members<T>(), [&item, &func](const auto& info)
    {
        func(info, item.*info.ptr);
    });
}

template <class T, class Func>
void for_each(T& item, Func&& func)
{
    core::visit(get_members<T>(), [&item, &func](const auto& info)
    {
        func(info, item.*info.ptr);
    });
}

} /* namespace cpp_essentials::meta */

#endif /* CPP_ESSENTIALS_META_META_HPP_ */