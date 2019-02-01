#ifndef CPP_ESSENTIALS_META_SERIALIZATION_HPP_
#define CPP_ESSENTIALS_META_SERIALIZATION_HPP_

#pragma once

#include <cpp_essentials/meta/meta.hpp>
#include <json.hpp>

namespace cpp_essentials::meta
{

template <class T, class Type, class Extra>
void to_json(nlohmann::json& json, const member_info<T, Type, Extra>& info, const Type& value)
{
    const auto name = info.name.data();
    if constexpr (is_optional_v<Type>)
    {
        if (value)
        {
            json[name] = *value;
        }
        else
        {
            json[name] = nullptr;
        }
    }
    else
    {
        json[name] = value;
    }
}

template <class T>
T get_default_value(const mandatory_member&, std::string_view name)
{
    throw std::runtime_error{ core::stringify("missing field '", name, "'") };
}

template <class T, class U>
T get_default_value(const default_member<U>& extra, std::string_view)
{
    return extra.value;
}

template <class T>
T get_default_value(const default_member<>&, std::string_view)
{
    return {};
}

template <class T, class Type, class Extra>
void from_json(const nlohmann::json& json, const member_info<T, Type, Extra>& info, Type& value)
{
    try
    {
        auto it = json.find(info.name.data());
        value = it != json.end() ? Type{ it->get<underlying_type_t<Type>>() } : get_default_value<Type>(info.extra, info.name);
    }
    catch (std::exception& ex)
    {
        throw std::runtime_error{ core::stringify("JSON to '", name_of<T>(), "': ", ex.what()) };
    }
}


template <class T>
void to_json(nlohmann::json& json, const T& item)
{
    if constexpr (is_structure<T>)
    {
        for_each(item, [&json](const auto& info, const auto& value)
        {
            to_json(json, info, value);
        });
    }
    else if constexpr (is_enumeration<T>)
    {
        json = meta::name_of(item);
    }
}

template <class T>
void from_json(const nlohmann::json& json, T& item)
{
    if constexpr (is_structure<T>)
    {
        for_each(item, [&json](const auto& info, auto& value)
        {
            from_json(json, info, value);
        });
    }
    else if constexpr (is_enumeration<T>)
    {
        item = parse_enum<T>(json.get<std::string>());
    }
}

} /* namespace cpp_essentials::meta */

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_registered<T>>>
void to_json(nlohmann::json& json, const T& item)
{
    ::cpp_essentials::meta::to_json(json, item);
}

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_registered<T>>>
void from_json(const nlohmann::json& json, T& item)
{
    ::cpp_essentials::meta::from_json(json, item);
}

#endif /* CPP_ESSENTIALS_META_SERIALIZATION_HPP_ */