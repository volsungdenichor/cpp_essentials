#ifndef CPP_ESSENTIALS_META_META_HPP_
#define CPP_ESSENTIALS_META_META_HPP_

#pragma once

#include <cpp_essentials/core/tuple.hpp>
#include <cpp_essentials/core/optional.hpp>

#include <json.hpp>

namespace cpp_essentials::meta
{

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};

template <class T>
struct is_optional<core::optional<T>> : std::true_type {};


template <class Type, class T>
struct Member
{
    Member(const char* name, Type T::*ptr, std::optional<Type> default_value)
        : name{ name }
        , ptr{ ptr }
        , offset{ core::offset_of(ptr) }
        , default_value{ std::move(default_value) }
    {
    }

    const char* name;
    Type T::*ptr;
    size_t offset;
    std::optional<Type> default_value;
};

template <class Type, class T>
auto member(Type T::*ptr, const char* name) -> Member<Type, T>
{
    return { name, ptr, std::nullopt };
}

template <class Type, class T>
auto member(Type T::*ptr, const char* name, Type default_value) -> Member<Type, T>
{
    return { name, ptr, std::move(default_value) };
}

template <class... Members>
auto members(Members&&... members)
{
    return std::make_tuple(std::forward<Members>(members)...);
}

template <class T>
auto register_members();

template <class T>
const auto& get_members()
{
    static const auto m = register_members<T>();
    return m;
}

template <class Type, class T>
void serialize_member(const Member<Type, T>& member, const Type& value, nlohmann::json& json)
{
    if constexpr (is_optional<Type>::value)
    {
        if (value)
        {
            json[member.name] = *value;
        }
        else
        {
            json[member.name] = nullptr;
        }
    }
    else
    {
        json[member.name] = value;
    }
}

template <class Type, class T>
void deserialize_member(const Member<Type, T>& member, Type& value, const nlohmann::json& json)
{
    auto it = json.find(member.name);
    if (it == json.end())
    {
        if constexpr (is_optional<Type>::value)
        {
            value = std::nullopt;
        }
        else
        {
            if (member.default_value)
            {
                value = *member.default_value;
            }
            else
            {
                using namespace std::string_literals;
                throw std::runtime_error{ "Missing mandatory field '"s + member.name + "'" };
            }
        }
    }
    else
    {
        value = *it;
    }
}

template <class T>
void serialize(const T& item, nlohmann::json& json)
{
    core::visit(get_members<T>(), [&json, &item](const auto& member)
    {
        serialize_member(member, item.*member.ptr, json);
    });
}

template <class T>
nlohmann::json serialize(const T& item)
{
    nlohmann::json result;
    serialize(item, result);
    return result;
}

template <class T>
void deserialize(T& item, const nlohmann::json& json)
{
    core::visit(get_members<T>(), [&json, &item](const auto& member) { deserialize_member(member, item.*member.ptr, json); });
}

template <class T>
T deserialize(const nlohmann::json& json)
{
    T result;
    deserialize(result, json);
    return result;   
}

template <class Type, class T>
const char* name_of(Type T::*ptr)
{
    const char* result = nullptr;
    auto offset = core::offset_of(ptr);
    core::visit(get_members<T>(), [offset, &result](const auto& member)
    {
        if (member.offset == offset)
        {
            result = member.name;
        }
    });
    return result;
}

} /* namespace cpp_essentials::meta */

#endif /* CPP_ESSENTIALS_META_META_HPP_ */