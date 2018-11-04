#ifndef CPP_ESSENTIALS_CORE_CONTAINERS_HPP_
#define CPP_ESSENTIALS_CORE_CONTAINERS_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>

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

} /* namespace detail */

static constexpr detail::to_container_fn<std::vector> to_vector = {};
static constexpr detail::to_container_fn<std::set> to_set = {};
static constexpr detail::to_container_fn<std::list> to_list = {};
static constexpr detail::to_container_fn<std::forward_list> to_forward_list = {};
static constexpr detail::to_container_fn<std::list> to_deque = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_CONTAINERS_HPP_ */
