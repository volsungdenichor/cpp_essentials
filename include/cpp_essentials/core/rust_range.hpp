#ifndef CPP_ESSENTIALS_CORE_RUST_RANGE_HPP_
#define CPP_ESSENTIALS_CORE_RUST_RANGE_HPP_

#pragma once

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/detail/rust_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct make_rust_range_fn
{
    template <class Func>
    auto operator ()(Func&& func) const
    {
        static_assert(core::is_optional_v<std::invoke_result_t<Func>>, "optional result required");
        return core::make_default_ended_range(rust_iterator{ std::forward<Func>(func) });
    }
};

} /* namespace detail */

static constexpr detail::make_rust_range_fn make_rust_range = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_RUST_RANGE_HPP_ */