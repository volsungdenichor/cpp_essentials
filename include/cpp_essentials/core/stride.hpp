#ifndef CPP_ESSENTIALS_CORE_STRIDE_HPP_
#define CPP_ESSENTIALS_CORE_STRIDE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/stride_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct stride_fn
{    
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int step) const
    {
        auto[b, e] = make_range(range);
        return make_range(
            stride_iterator{ b, step, e },
            stride_iterator{ e, step, e });
    }
};

} /* namespace detail */

static constexpr detail::stride_fn stride = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_STRIDE_HPP_ */
