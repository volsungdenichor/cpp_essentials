#ifndef CPP_ESSENTIALS_META_SERIALIZATION_HPP_
#define CPP_ESSENTIALS_META_SERIALIZATION_HPP_

#pragma once

#include <cpp_essentials/meta/meta.hpp>
#include <json.hpp>

namespace cpp_essentials::meta
{

template <class T>
std::optional<T> try_parse(std::string_view text)
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
T parse(std::string_view text)
{
    static_assert(is_registered<T>, "Type not registered");
    static_assert(is_enumeration<T>, "Enumeration required");

    const auto result = try_parse<T>(text);
    if (!result)
    {
        throw std::runtime_error{ "Unknown enum value" };
    }
    return *result;
}

template <class T>
struct is_optional : std::false_type {};

template <class T>
struct is_optional<std::optional<T>> : std::true_type {};


template <class T, class Type, class Tag>
void to_json(nlohmann::json& json, const member_info<T, Type, Tag>& info, const Type& value)
{
    json[info.name.data()] = value;
}

template <class T, class Type>
void from_json(const nlohmann::json& json, const member_info<T, Type, default_member>& info, Type& value)
{
    auto it = json.find(info.name.data());
    if (it != json.end())
    {
        it->get_to(value);
    }
    else
    {
        if constexpr (is_optional<T>::value)
        {
            value = Type{};
        }
        else
        {
            using namespace std::string_literals;
            throw std::runtime_error{ "Mandatory member missing: "s + std::string{ info.name } };
        }
    }
}

template <class T, class Type>
void from_json(const nlohmann::json& json, const member_info<T, Type, member_or_value>& info, Type& value)
{
    auto it = json.find(info.name);
    if (it != json.end())
    {
        it->get_to(value);
    }
    else
    {
        value = info.default_value;
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
        item = parse<T>(json.get<std::string>());
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

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_registered<T>>>
std::ostream& operator <<(std::ostream& os, const T& item)
{
    if constexpr (::cpp_essentials::meta::is_structure<T>)
    {
        meta::for_each(item, [&os](const auto& info, const auto& value)
        {
            os << info.name << "=" << value << ";";
        });
    }
    else if constexpr (::cpp_essentials::meta::is_enumeration<T>)
    {
        os << meta::name_of(item);
    }
    return os;
}

template <class T, CONCEPT = std::enable_if_t<cpp_essentials::meta::is_registered<T>>>
std::istream& operator >>(std::istream& is, T& item)
{
    if constexpr (::cpp_essentials::meta::is_structure<T>)
    {
        meta::for_each(item, [&is](const auto& info, auto& value)
        {
            // TODO
        });
    }
    else if constexpr (::cpp_essentials::meta::is_enumeration<T>)
    {
        std::string temp;
        is >> temp;
        item = meta::template parse<T>(temp);
    }
    return is;
}

#endif /* CPP_ESSENTIALS_META_SERIALIZATION_HPP_ */