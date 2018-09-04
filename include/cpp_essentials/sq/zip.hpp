#ifndef CPP_ESSENTIALS_SQ_ZIP_HPP_
#define CPP_ESSENTIALS_SQ_ZIP_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/iterator_range.hpp>
#include <cpp_essentials/sq/detail/zip_iterator.hpp>

namespace cpp_essentials::sq
{

namespace detail
{

struct zip_t : core::adaptable<zip_t>
{
    using adaptable::operator();

    template <class Range1, class Range2, class BinaryFunc>
    auto operator ()(Range1&& range1, Range2&& range2, BinaryFunc func) const
    {
        return core::make_range(
            zip_iterator{ func, std::begin(range1), std::begin(range2) },
            zip_iterator{ func, std::end(range1), std::end(range2) });
    }
};

} /* namespace detail */

static constexpr detail::zip_t zip = {};

} /* namespace cpp_essentials::sq */

#endif /* CPP_ESSENTIALS_SQ_ZIP_HPP_ */
