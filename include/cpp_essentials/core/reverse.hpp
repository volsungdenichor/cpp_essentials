#ifndef CPP_ESSENTIALS_CORE_REVERSE_HPP_
#define CPP_ESSENTIALS_CORE_REVERSE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct reverse_fn
{
    template <class Iter>
    auto make_iterator(Iter iter) const
    {
        return std::make_reverse_iterator(iter);
    }

    template <class Iter>
    auto make_iterator(std::reverse_iterator<Iter> iter) const
    {
        return iter.base();
    }

    template
        < class Range
        , CONCEPT = cc::BidirectionalRange<Range>>
    auto operator ()(Range&& range) const
    {
        return make_range(
            make_iterator(std::end(range)),
            make_iterator(std::begin(range)));
    }
};

} /* namespace detail */

static constexpr auto reverse = adaptable{ detail::reverse_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_REVERSE_HPP_ */
