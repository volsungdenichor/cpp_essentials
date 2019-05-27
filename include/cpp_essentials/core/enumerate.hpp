#ifndef CPP_ESSENTIALS_CORE_ENUMERATE_HPP_
#define CPP_ESSENTIALS_CORE_ENUMERATE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/zip.hpp>
#include <cpp_essentials/core/numeric.hpp>

namespace cpp_essentials::core::views
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


struct enumerate_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, int start = 0) const
    {
        using cpp_essentials::core::views::zip;
        using cpp_essentials::core::infinite_range;
        return zip(range, infinite_range(start), make_indexed_item{});
    }
};

} /* namespace detail */

static constexpr auto enumerate = adaptable{ detail::enumerate_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_ENUMERATE_HPP_ */
