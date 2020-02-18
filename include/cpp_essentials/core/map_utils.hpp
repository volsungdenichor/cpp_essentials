#ifndef CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_
#define CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_

#pragma once

#include <map>
#include <unordered_map>

#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/serialization.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
struct is_associative_container : std::false_type {};

template <class... Args>
struct is_associative_container<std::map<Args...>> : std::true_type {};

template <class... Args>
struct is_associative_container<std::multimap<Args...>> : std::true_type {};

template <class... Args>
struct is_associative_container<std::unordered_map<Args...>> : std::true_type {};

template <class... Args>
struct is_associative_container<std::unordered_multimap<Args...>> : std::true_type {};

template <class T>
static constexpr bool is_associative_container_v = is_associative_container<T>::value;

template <class T>
using AssociativeContainer = std::enable_if_t<is_associative_container_v<T>>;

struct map_equal_range_fn
{
    template <class Map, class K, CONCEPT = AssociativeContainer<std::decay_t<Map>>>
    auto operator ()(Map&& item, const K& key) const
    {
        return make_range(item.equal_range(key));
    }
};

struct map_at_fn
{
    template <class Map, class K, CONCEPT = AssociativeContainer<std::decay_t<Map>>>
    auto operator ()(Map&& item, const K& key) const
    {
        static constexpr auto _map_equal_range = map_equal_range_fn{};
        return views::map(_map_equal_range(item, key), get_value);
    }
};

struct map_try_get_fn
{
    template <class Map, class K, CONCEPT = AssociativeContainer<Map>>
    core::optional<const typename Map::mapped_type&> operator ()(const Map& map, const K& key) const
    {
        using result_type = core::optional<const typename Map::mapped_type&>;
        auto it = map.find(key);
        return it != map.end() ? result_type{ it->second } : result_type{};
    }

    template <class Map, class K, CONCEPT = AssociativeContainer<Map>>
    core::optional<typename Map::mapped_type&> operator ()(Map& map, const K& key) const
    {
        using result_type = core::optional<typename Map::mapped_type&>;
        auto it = map.find(key);
        return it != map.end() ? result_type{ it->second } : result_type{};
    }
};

struct map_get_fn
{
    template <class Map, class K, CONCEPT = AssociativeContainer<Map>>
    const typename Map::mapped_type& operator ()(const Map& map, const K& key) const
    {
        static constexpr auto _map_try_get = map_try_get_fn{};
        return _map_try_get(map, key).value_or_throw([&]()
        {
            return ::cpp_essentials::core::str("Missing key '", key, "'");
        });
    }

    template <class Map, class K, CONCEPT = AssociativeContainer<Map>>
    typename Map::mapped_type& operator ()(Map& map, const K& key) const
    {
        static constexpr auto _map_try_get = map_try_get_fn{};
        return _map_try_get(map, key).value_or_throw([&]()
        {
            return ::cpp_essentials::core::str("Missing key '", key, "'");
        });
    }
};

} /* namespace detail */

static constexpr auto map_equal_range = detail::map_equal_range_fn{};
static constexpr auto map_at = detail::map_at_fn{};
static constexpr auto map_try_get = detail::map_try_get_fn{};
static constexpr auto map_get = detail::map_get_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_MAP_UTILS_HPP_ */