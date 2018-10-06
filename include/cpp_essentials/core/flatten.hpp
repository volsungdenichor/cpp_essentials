#ifndef CPP_ESSENTIALS_CORE_FLATTEN_HPP_
#define CPP_ESSENTIALS_CORE_FLATTEN_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/detail/flatten_iterator.hpp>

namespace cpp_essentials::core
{

namespace detail
{

struct flatten_t : adaptable<flatten_t>
{
    using adaptable::operator();

    template
        < class Range
        , CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        using outer_iter = cc::range_iter<Range>;
        using inner_iter = cc::range_iter<cc::iter_ref<outer_iter>>;

        using iterator = flatten_iterator<outer_iter, inner_iter>;

        auto b = std::begin(range);
        auto e = std::end(range);

        return make_range(
            iterator{ b, e },
            iterator{ e, e });
    }
};

} /* namespace detail */

static constexpr detail::flatten_t flatten = {};

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FLATTEN_HPP_ */
