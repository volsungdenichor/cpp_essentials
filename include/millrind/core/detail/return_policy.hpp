#pragma once

#include <millrind/core/type_traits.hpp>
//#include <cpp_essentials/core/optional.hpp>
#include <millrind/core/iterator_range.hpp>
#include <tuple>

namespace millrind::core
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
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> core::iterator_range<Iter>
    {
        return make_range(found, end);
    }
};

struct return_found_next_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return found != end 
            ? make_range(found, std::next(found)) 
            : make_range(found, found);
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
        return found != end 
            ? make_range(begin, std::next(found)) 
            : make_range(found, found);
    }
};

struct return_next_end_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iterator_range<Iter>
    {
        return found != end 
            ? make_range(std::next(found), end) 
            : make_range(found, found);
    }
};

struct return_value_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> iter_ref<Iter>
    {
        if (found == end)
            throw std::runtime_error{ "found == end" };

        return *found;
    }
};

struct return_optional_t : return_policy_base_t
{
#if 0
    template <class Iter>
    auto operator ()(Iter /*begin*/, Iter found, Iter end) const -> core::optional<cciter_ref<Iter>>
    {
        if (found == end)
        {
            return {};
        }

        return *found;
    }
#endif
};

struct return_both_t : return_policy_base_t
{
    template <class Iter>
    auto operator ()(Iter begin, Iter found, Iter end) const -> std::pair<iterator_range<Iter>, iterator_range<Iter>>
    {
        return std::pair{
            make_range(begin, found),
            make_range(found, end)
        };
    }
};

template <class T>
using ReturnPolicy = std::enable_if_t<std::is_base_of_v<detail::return_policy_base_t, T>>;

} /* detail */

static constexpr auto return_found = detail::return_found_t{};
static constexpr auto return_found_end = detail::return_found_end_t{};
static constexpr auto return_found_next = detail::return_found_next_t{};
static constexpr auto return_begin_found = detail::return_begin_found_t{};
static constexpr auto return_begin_next = detail::return_begin_next_t{};
static constexpr auto return_next_end = detail::return_next_end_t{};
static constexpr auto return_value = detail::return_value_t{};
static constexpr auto return_optional = detail::return_optional_t{};
static constexpr auto return_both = detail::return_both_t{};

using default_return_policy_t = detail::return_found_end_t;

} // namespace millrind::core
