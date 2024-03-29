#ifndef CPP_ESSENTIALS_CORE_VIEWS_STRIDE_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_STRIDE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/stride_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct stride_fn
{    
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, int step) const
    {
        using cpp_essentials::core::detail::stride_iterator;
        auto[b, e] = make_range(range);
        return make_range(
            stride_iterator{ b, step, e },
            stride_iterator{ e, step, e });
    }
};

} /* namespace detail */

static constexpr auto stride = detail::stride_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_STRIDE_HPP_ */
