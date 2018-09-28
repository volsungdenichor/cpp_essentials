#ifndef CPP_ESSENTIALS_SQ_PY_SLICE_HPP_
#define CPP_ESSENTIALS_SQ_PY_SLICE_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/functors.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/nil.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct py_slice_t : core::adaptable<py_slice_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::ForwardRange<Range>>
    auto operator ()(Range&& range, int begin_index, int end_index) const
    {
        const auto r = core::make_range(range);
        const auto size = static_cast<int>(r.size());
        
        const auto adjust_index = [=](auto index)
        {
            return core::clamp(index >= 0 ? index : index + size, 0, size);
        };

        begin_index = begin_index == nil ? 0 : adjust_index(begin_index);
        end_index = end_index == nil ? size : adjust_index(end_index);
        
        if (begin_index >= 0 && end_index >= 0 && begin_index < end_index)
        {
            auto b = std::next(r.begin(), begin_index);
            auto e = std::next(b, end_index - begin_index);
            return core::make_range(b, e);
        }

        return core::make_range(r.end(), r.end());

    }
};

} /* namespace detail */

static constexpr detail::py_slice_t py_slice = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_PY_SLICE_HPP_ */

