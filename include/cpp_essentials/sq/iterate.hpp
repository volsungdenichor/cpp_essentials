#ifndef CPP_ESSENTIALS_SQ_ITERATE_HPP_
#define CPP_ESSENTIALS_SQ_ITERATE_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/iterate_iterator.hpp>

#pragma once

namespace cpp_essentials::sq
{

namespace detail
{

struct iterate_t : core::adaptable<iterate_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range) const
    {
        return core::make_range(
            iterate_iterator{ std::begin(range) },
            iterate_iterator{ std::end(range) });
    }
};

} /* namespace detail */

static constexpr detail::iterate_t iterate = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_MAP_HPP_ */
