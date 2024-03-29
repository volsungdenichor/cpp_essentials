#ifndef CPP_ESSENTIALS_CORE_JOIN_HPP_
#define CPP_ESSENTIALS_CORE_JOIN_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/output.hpp>
#include <cpp_essentials/core/serialization.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct join_fn
{
    template
        < class Range
        , class = cc::InputRange<Range>>
    auto operator ()(Range&& range, const char* separator) const -> std::string
    {
        return stringify(core::delimit(range, separator));
    }
};

} /* namespace detail */

static constexpr auto join = detail::join_fn{};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_JOIN_HPP_ */
