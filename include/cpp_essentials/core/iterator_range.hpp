#ifndef CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_

#pragma once

#include <iterator>
#include <algorithm>
#include <tuple>

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/concepts/concepts.hpp>

namespace cpp_essentials::core
{

template <class Iter, class Diff>
auto advance(Iter it, Iter end, Diff n) -> Iter
{
    while (it != end && n > 0)
    {
        ++it;
        --n;
    }
    return it;
}


template <class Iter, CONCEPT_IF(concepts::InputIterator<Iter>)>
class iterator_range
{
public:
    using iterator = Iter;
    using traits = std::iterator_traits<iterator>;
    using iterator_category = typename traits::iterator_category;
    using value_type = typename traits::value_type;
    using reference = typename traits::reference;
    using difference_type = typename traits::difference_type;
    using size_type = std::make_unsigned_t<difference_type>;

    iterator_range()
        : iterator_range{ iterator{}, iterator{} }
    {
    }

    iterator_range(iterator begin, iterator end)
        : _begin{ begin }
        , _end{ end }
    {
    }

    iterator_range(iterator begin, size_type size)
        : iterator_range{ begin, std::next(begin, size) }
    {
    }

    template <class Range, CONCEPT_IF(concepts::InputRange<Range>)>
    iterator_range(Range&& range)
        : iterator_range{ std::begin(range), std::end(range) }
    {
    }


    auto begin() const -> iterator
    {
        return _begin;
    }

    auto end() const -> iterator
    {
        return _end;
    }


    auto size() const -> size_type
    {
        return static_cast<size_type>(std::distance(begin(), end()));
    }

    auto empty() const -> bool
    {
        return begin() == end();
    }


    auto front() const -> reference
    {
        EXPECTS(!empty());
        return *begin();
    }    

    auto operator [](size_type index) const -> reference
    {
        return *std::next(begin(), index);
    }

    auto at(size_type index) const -> reference
    {
        auto it = advance(begin(), end(), index);
        EXPECTS(it != end());
        return *it;
    }


    auto operator *() const -> reference
    {
        return front();
    }

    explicit operator bool() const
    {
        return !empty();
    }


    template <class Container, CONCEPT_IF(std::is_constructible<Container, Iter, Iter>::value)>
    operator Container() const
    {
        return { begin(), end() };
    }

private:
    iterator _begin;
    iterator _end;
};



template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator ==(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::equal(std::begin(rhs), std::end(rhs), lhs.begin(), lhs.end());
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator !=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs == rhs);
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator <(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), std::begin(rhs), std::end(rhs));
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator >(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return rhs < lhs;
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator <=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs > rhs);
}

template <class Iter, class Range, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator >=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs < rhs);
}

#if 0
template <class Range, class Iter, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator ==(Range&& lhs, const iterator_range<Iter>& rhs) -> bool
{
    return rhs == lhs;
}

template <class Range, class Iter, CONCEPT_IF(concepts::InputRange<Range>)>
auto operator !=(Range&& lhs, const iterator_range<Iter>& rhs) -> bool
{
    return rhs != lhs;
}
#endif




template
    < class Iter
    , CONCEPT_IF(concepts::InputIterator<Iter>)>
auto make_range(Iter begin, Iter end) -> iterator_range<Iter>
{
    return iterator_range<Iter>{ begin, end };
}

template
< class Iter
    , CONCEPT_IF(concepts::InputIterator<Iter>)>
auto make_range(const std::pair<Iter, Iter>& pair) -> iterator_range<Iter>
{
    return make_range(std::get<0>(pair), std::get<1>(pair));
}

template
< class Range
    , CONCEPT_IF(concepts::InputRange<Range>)>
auto make_range(Range&& range)
{
    return make_range(std::begin(range), std::end(range));
}

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_ */
