#ifndef CPP_ESSENTIALS_SQ_STRIDE_HPP_
#define CPP_ESSENTIALS_SQ_STRIDE_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/stride_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct stride_t : core::adaptable<stride_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int step) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return core::make_range(
            stride_iterator{ b, step, e },
            stride_iterator{ e, step, e });
    }
};

} /* namespace detail */

static constexpr detail::stride_t stride = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_STRIDE_HPP_ */
