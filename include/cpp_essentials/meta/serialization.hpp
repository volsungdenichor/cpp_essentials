#ifndef CPP_ESSENTIALS_META_SERIALIZATION_HPP_
#define CPP_ESSENTIALS_META_SERIALIZATION_HPP_

#pragma once

#include <cpp_essentials/meta/meta.hpp>
#include <json.hpp>

namespace cpp_essentials::meta
{

template <class T, CONCEPT = is_enum<T>>
std::optional<T> try_parse(const std::string& text)
{
    std::optional<T> result = std::nullopt;
    core::visit(get_enum_values<T>(), [&result, &text](const auto& info)
    {
        if (info.name == text)
        {
            result = info.value;
        }
    });
    return result;
}

template <class T, CONCEPT = is_enum<T>>
T parse(const std::string& text)
{
    const auto result = try_parse<T>(text);
    if (!result)
    {
        throw std::runtime_error{ "Unknown enum value" };
    }
    return *result;
}

} /* namespace cpp_essentials::meta */

template <class T, CONCEPT = std::enable_if_t<::cpp_essentials::meta::type_registered<T>>>
void to_json(nlohmann::json& json, const T& item)
{
    if constexpr (::cpp_essentials::meta::members_registered<T>)
    {
        meta::for_each(item, [&json](const auto& info, const auto& value)
        {
            json[info.name] = value;
        });
    }
    else if constexpr (::cpp_essentials::meta::enum_values_registered<T>)
    {
        json = meta::name_of(item);
    }
}

template <class T, CONCEPT = std::enable_if_t<::cpp_essentials::meta::type_registered<T>>>
void from_json(const nlohmann::json& json, T& item)
{
    if constexpr (::cpp_essentials::meta::members_registered<T>)
    {
        meta::for_each(item, [&json](const auto& info, auto& value)
        {
            value = json[info.name];
        });
    }
    else if constexpr (::cpp_essentials::meta::enum_values_registered<T>)
    {
        item = meta::template parse<T>(json.get<std::string>());
    }
}

template <class T, CONCEPT = std::enable_if_t<::cpp_essentials::meta::type_registered<T>>>
std::ostream& operator <<(std::ostream& os, const T& item)
{
    if constexpr (::cpp_essentials::meta::members_registered<T>)
    {
        meta::for_each(item, [&os](const auto& info, const auto& value)
        {
            os << info.name << "=" << value << ";";
        });
    }
    else if constexpr (::cpp_essentials::meta::enum_values_registered<T>)
    {
        os << meta::name_of(item);
    }
    return os;
}

template <class T, CONCEPT = std::enable_if_t<::cpp_essentials::meta::type_registered<T>>>
std::istream& operator >>(std::istream& is, T& item)
{
    if constexpr (::cpp_essentials::meta::members_registered<T>)
    {
        meta::for_each(item, [&is](const auto& info, auto& value)
        {
            // TODO
        });
    }
    else if constexpr (::cpp_essentials::meta::enum_values_registered<T>)
    {
        std::string temp;
        is >> temp;
        item = meta::template parse<T>(temp);
    }
    return is;
}

#endif /* CPP_ESSENTIALS_META_SERIALIZATION_HPP_ */