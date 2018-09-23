#ifndef CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_
#define CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_

#pragma once

#include <cpp_essentials/cc/cc.hpp>
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
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> cc::iter_ref<Iter>
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
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> core::optional<cc::iter_ref<Iter>>
    {
        if (found == end)
        {
            return {};
        }

        return *found;
    }
};

template <class T>
using ReturnPolicy = cc::BaseOf<return_policy_base_t, T>;

} /* detail */

using return_found = detail::return_found_t;
using return_found_end = detail::return_found_end_t;
using return_found_next = detail::return_found_next_t;
using return_begin_found = detail::return_begin_found_t;
using return_begin_next = detail::return_begin_next_t;
using return_next_end = detail::return_next_end_t;
using return_value = detail::return_value_t;
using return_optional = detail::return_optional_t;

using default_return_policy_t = detail::return_found_end_t;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_RETURN_POLICY_HPP_ */