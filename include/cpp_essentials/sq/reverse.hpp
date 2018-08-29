#ifndef CPP_ESSENTIALS_SQ_REVERSE_HPP_
#define CPP_ESSENTIALS_SQ_REVERSE_HPP_

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/zip_iterator.hpp>

#pragma once

namespace cpp_essentials::sq
{

namespace detail
{

struct reverse_t : core::adaptable<reverse_t>
{
    using adaptable::operator();

    template <class Iter>
    auto make_reverse_iterator(Iter iter) const
    {
        return std::make_reverse_iterator(iter);
    }

    template <class Iter>
    auto make_reverse_iterator(std::reverse_iterator<Iter> iter) const
    {
        return iter.base();
    }

    template <class Range>
    auto operator ()(Range&& range) const
    {
        return core::make_range(
            make_reverse_iterator(std::end(range)),
            make_reverse_iterator(std::begin(range)));
    }
};

} /* namespace detail */

static constexpr detail::reverse_t reverse = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_REVERSE_HPP_ */
