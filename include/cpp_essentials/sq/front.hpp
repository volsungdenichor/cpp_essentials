#ifndef CPP_ESSENTIALS_SQ_FRONT_HPP_
#define CPP_ESSENTIALS_SQ_FRONT_HPP_

#pragma once

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/optional.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct front_t : core::adaptable<front_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        EXPECTS(b != e);
        return *b;
    }
};

struct front_or_t : core::adaptable<front_or_t>
{
    using adaptable::operator();

    template <class Range, class T>
    auto operator ()(Range&& range, T default_value) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e ? *b : default_value;
    }
};

struct front_or_eval_t : core::adaptable<front_or_eval_t>
{
    using adaptable::operator();

    template <class Range, class Func>
    auto operator ()(Range&& range, Func func) const
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return b != e ? *b : func();
    }
};

struct front_or_none_t : core::adaptable<front_or_none_t>
{
    using adaptable::operator();

    template <class Range>
    auto operator ()(Range&& range) const -> decltype(auto)
    {
        auto b = std::begin(range);
        auto e = std::end(range);
        return core::eval_optional(b != e, [&]() -> decltype(auto) { return *b; });
    }
};

} /* namespace detail */

static constexpr detail::front_t front = {};
static constexpr detail::front_or_t front_or = {};
static constexpr detail::front_or_eval_t front_or_eval = {};
static constexpr detail::front_or_none_t front_or_none = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_MAP_HPP_ */
