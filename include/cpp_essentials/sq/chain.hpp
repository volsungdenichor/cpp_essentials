#ifndef CPP_ESSENTIALS_SQ_CHAIN_HPP_
#define CPP_ESSENTIALS_SQ_CHAIN_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/chain_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

template <class Iter1, class Iter2>
auto chain(Iter1 b1, Iter1 e1, Iter2 b2, Iter2 e2)
{
    return core::make_range(
        chain_iterator{ b1, b2, e1, e2 },
        chain_iterator{ e1, e2, e1, b2 });
}

struct append_t : core::adaptable<append_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2>
    auto operator ()(Range1&& range1, Range2&& range2) const
    {
        static_assert(concepts::InputRange<Range1>, "append: input range required");
        static_assert(concepts::InputRange<Range2>, "append: input range required");
        
        return chain(std::begin(range1), std::end(range1), std::begin(range2), std::end(range2));
    }
};

struct prepend_t : core::adaptable<prepend_t>
{
    using adaptable::operator();

    template
        < class Range1
        , class Range2>
    auto operator ()(Range1&& range1, Range2&& range2) const
    {
        static_assert(concepts::InputRange<Range1>, "prepend: input range required");
        static_assert(concepts::InputRange<Range2>, "prepend: input range required");

        return chain(std::begin(range2), std::end(range2), std::begin(range1), std::end(range1));
    }
};

} /* namespace chain */

static constexpr detail::append_t chain = {};
static constexpr detail::append_t append = {};
static constexpr detail::prepend_t prepend = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_CHAIN_HPP_ */
