#ifndef CPP_ESSENTIALS_CORE_JOIN_HPP_
#define CPP_ESSENTIALS_CORE_JOIN_HPP_

#pragma once

#include <cpp_essentials/core/output.hpp>
#include <sstream>

namespace cpp_essentials::core
{

namespace detail
{

struct join_t
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range, const char* separator) const -> std::string
    {
        std::stringstream ss;

        ss << core::delimit(range, separator);

        return ss.str();
    }
};

} /* namespace detail */

static constexpr detail::join_t join = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_JOIN_HPP_ */
