#ifndef CPP_ESSENTIALS_CORE_FLATTEN_HPP_
#define CPP_ESSENTIALS_CORE_FLATTEN_HPP_

#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/flatten_iterator.hpp>

namespace cpp_essentials::core::views
{

namespace detail
{

struct flatten_fn
{
    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        using cpp_essentials::core::detail::flatten_iterator;
        auto b = std::begin(range);
        auto e = std::end(range);

        return make_range(
            flatten_iterator{ b, e },
            flatten_iterator{ e, e });
    }
};

} /* namespace detail */

static constexpr auto flatten = adaptable{ detail::flatten_fn{} };

} /* namespace cpp_essentials::core::views */

#endif /* CPP_ESSENTIALS_CORE_FLATTEN_HPP_ */
