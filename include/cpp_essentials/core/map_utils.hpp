#ifndef CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_
#define CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_

#pragma once

#include <map>
#include <unordered_map>

#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/functors.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
struct is_associative_container : std::false_type {};

template <class K, class V>
struct is_associative_container<std::map<K, V>> : std::true_type {};

template <class K, class V>
struct is_associative_container<std::multimap<K, V>> : std::true_type {};

template <class K, class V>
struct is_associative_container<std::unordered_map<K, V>> : std::true_type {};

template <class K, class V>
struct is_associative_container<std::unordered_multimap<K, V>> : std::true_type {};

template <class T>
static constexpr bool is_associative_container_v = is_associative_container<T>::value;

template <class T>
using AssociativeContainer = std::enable_if_t<is_associative_container_v<T>>;

struct at_fn
{
    template <class Map, class K, CONCEPT = detail::AssociativeContainer<std::decay_t<Map>>>
    auto operator ()(Map&& item, const K& key) const
    {
        return views::map(make_range(item.equal_range(key)), get_value);
    }
};

} /* namespace detail */

static constexpr auto at = adaptable{ detail::at_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_ */