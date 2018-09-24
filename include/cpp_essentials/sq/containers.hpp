#ifndef CPP_ESSENTIALS_SQ_CONTAINERS_HPP_
#define CPP_ESSENTIALS_SQ_CONTAINERS_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/iterate_iterator.hpp>

#include <vector>
#include <list>
#include <forward_list>
#include <set>
#include <deque>

#include <string>

namespace cpp_essentials::sq
{

namespace detail
{

template <class C>
using basic_string = std::basic_string<C, std::char_traits<C>, std::allocator<C>>;


template <template <class> class Container>
struct to_t : core::adaptable<to_t<Container>>
{
    using core::adaptable<to_t<Container>>::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> Container<cc::range_val<Range>>
    {        
        return { std::begin(range), std::end(range) };
    }
};

template <class C = char>
struct to_string_t : core::adaptable<to_string_t<C>>
{
    using core::adaptable<to_string_t<C>>::operator();  

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const -> basic_string<C>
    {
        return { std::begin(range), std::end(range) };
    }

};

} /* namespace detail */

static constexpr detail::to_t<std::vector> to_vector = {};
static constexpr detail::to_t<std::set> to_set = {};
static constexpr detail::to_t<std::list> to_list = {};
static constexpr detail::to_t<std::forward_list> to_forward_list = {};
static constexpr detail::to_t<std::list> to_deque = {};
static constexpr detail::to_string_t<char> to_string = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_CONTAINERS_HPP_ */
