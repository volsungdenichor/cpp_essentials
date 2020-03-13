#ifndef CPP_ESSENTIALS_CORE_VIEWS_UNZIP_HPP_
#define CPP_ESSENTIALS_CORE_VIEWS_UNZIP_HPP_

#pragma once

#include <cpp_essentials/core/views/map.hpp>
#include <cpp_essentials/core/functors.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct unzip_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return create(FORWARD(range), std::make_index_sequence<std::tuple_size_v<cc::range_val<Range>>>{});
    }

private:
    template <class Range, size_t... Index>
    auto create(Range&& range, std::index_sequence<Index...>) const
    {
        return std::make_tuple(core::views::map(range, get<Index>)...);
    }
};

} /* namespace detail */

static constexpr auto unzip = detail::unzip_fn{};

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_VIEWS_UNZIP_HPP_ */