#ifndef CPP_ESSENTIALS_CORE_INDEX_HPP_
#define CPP_ESSENTIALS_CORE_INDEX_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/zip.hpp>
#include <cpp_essentials/core/numeric.hpp>

namespace cpp_essentials::core
{

namespace detail
{

template <class T>
struct indexed_item
{
    using value_type = T;

    const int index;
    value_type value;

    friend std::ostream& operator <<(std::ostream& os, const indexed_item& item)
    {
        return os << "[" << item.index << "] " << item.value;
    }
};

struct make_indexed_item
{
    template <class T>
    auto operator ()(T&& value, int index) const -> indexed_item<T>
    {
        return { index, std::forward<T>(value) };
    }
};


struct index_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int start = 0) const
    {
        return ::cpp_essentials::core::zip(range, ::cpp_essentials::core::infinite_range(start), make_indexed_item{});
    }
};

} /* namespace detail */

static constexpr detail::index_t index = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_INDEX_HPP_ */
