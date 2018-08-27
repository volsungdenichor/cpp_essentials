#ifndef CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_
#define CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_

#pragma once

#include <cpp_essentials/concepts/concepts.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/core/iterator_range.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct return_policy_base_t {};

struct return_found_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter /*end*/) const -> Iter
    {
        return found;
    }
};

struct return_found_end_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return make_range(found, end);
    }
};

struct return_found_next_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return found != end ? make_range(found, std::next(found)) : make_range(found, found);
    }
};

struct return_begin_found_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter begin, Iter found, Iter /*end*/) const -> iterator_range<Iter>
    {
        return make_range(begin, found);
    }
};

struct return_begin_next_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter begin, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return found != end ? make_range(begin, std::next(found)) : make_range(found, found);
    }
};

struct return_next_end_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return found != end ? make_range(std::next(found), end) : make_range(found, found);
    }
};

struct return_value_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> concepts::iterator_reference<Iter>
    {
        if (found == end)
        {
            throw std::runtime_error{ "found == end" };
        }

        return *found;
    }
};

struct return_optional_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> core::optional<concepts::iterator_reference<Iter>>
    {
        if (found == end)
        {
            return {};
        }

        return *found;
    }
};

template <class T>
using is_return_policy = std::is_base_of<return_policy_base_t, T>;

} /* detail */

using default_return_policy_t = detail::return_found_end_t;

static constexpr detail::return_found_t return_found = {};
static constexpr detail::return_found_end_t return_found_end = {};
static constexpr detail::return_found_next_t return_found_next = {};
static constexpr detail::return_begin_found_t return_begin_found = {};
static constexpr detail::return_begin_next_t return_begin_next = {};
static constexpr detail::return_next_end_t return_next_end = {};
static constexpr detail::return_value_t return_value = {};
static constexpr detail::return_optional_t return_optional = {};

static constexpr default_return_policy_t return_default = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_ */