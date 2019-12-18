#ifndef CPP_ESSENTIALS_CORE_CONTAINERS_HPP_
#define CPP_ESSENTIALS_CORE_CONTAINERS_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>

#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <deque>

namespace cpp_essentials::core
{

namespace detail
{

template <class C>
using basic_string = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;


template <template <class> class Container>
struct to_container_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> Container<cc::range_val<Range>>
    {        
        return { std::begin(range), std::end(range) };
    }
};

struct to_string_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> std::string
    {
        return { std::begin(range), std::end(range) };
    }
};

struct to_string_view_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> std::string_view
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e
            ? std::string_view{ std::addressof(*b), std::string_view::size_type(std::distance(b, e)) }
            : std::string_view{};
    }
};

} /* namespace detail */

static constexpr auto to_vector = adaptable{ detail::to_container_fn<std::vector>{} };
static constexpr auto to_set = adaptable{ detail::to_container_fn<std::set>{} };
static constexpr auto to_list = adaptable{ detail::to_container_fn<std::list>{} };
static constexpr auto to_forward_list = adaptable{ detail::to_container_fn<std::forward_list>{} };
static constexpr auto to_deque = adaptable{ detail::to_container_fn<std::list>{} };
static constexpr auto to_string = adaptable{ detail::to_string_fn{} };
static constexpr auto to_string_view = adaptable{ detail::to_string_view_fn{} };

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CONTAINERS_HPP_ */
