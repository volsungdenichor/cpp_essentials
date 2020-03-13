#ifndef CPP_ESSENTIALS_CORE_VIEWS_ZIP_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_ZIP_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/zip_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct zip_fn
{
    template
        < class Range1
        , class Range2
        , class BinaryFunc
        , CONCEPT = cc::InputRange<Range1>
        , CONCEPT = cc::InputRange<Range2>
        , CONCEPT = cc::BinaryFunction<BinaryFunc, cc::range_ref<Range1>, cc::range_ref<Range2>>>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryFunc func) const
    {
        using cpp_essentials::core::detail::zip_iterator;
        return make_range(
            zip_iterator{ func, std::begin(range1), std::begin(range2) },
            zip_iterator{ func, std::end(range1), std::end(range2) });
    }
};

} /* namespace detail */

static constexpr auto zip = detail::zip_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_ZIP_HPP_ */
