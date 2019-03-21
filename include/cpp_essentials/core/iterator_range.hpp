#ifndef CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_
#define CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_

#pragma once

#include <iterator>
#include <algorithm>
#include <tuple>

#include <cpp_essentials/core/assertions.hpp>
#include <cpp_essentials/core/optional.hpp>
#include <cpp_essentials/cc/cc.hpp>

namespace cpp_essentials::core
{

using cc::make_func;

template <class Iter, class Diff>
auto advance(Iter begin, Iter end, Diff n) -> Iter
{
    while (begin != end && n > 0)
    {
        ++begin;
        --n;
    }
    return begin;
}

template <class Iter, class Diff>
auto advance_exactly(Iter begin, Iter end, Diff n) -> Iter
{
    while (begin != end && n > 0)
    {
        ++begin;
        --n;
    }
    EXPECTS(n == 0);
    return begin;
}

template <class Iter, class Diff>
auto advance_back(Iter begin, Iter end, Diff n) -> Iter
{
    while (begin != end && n > 0)
    {
        --end;
        --n;
    }
    return end;
}

template <class Iter, class Diff>
auto advance_back_exactly(Iter begin, Iter end, Diff n) -> Iter
{
    while (begin != end && n > 0)
    {
        --end;
        --n;
    }
    EXPECTS(n == 0);
    return end;
}


template <class Iter, CONCEPT = cc::InputIterator<Iter>>
class iterator_range
{
public:
    using iterator = Iter;
    using traits = std::iterator_traits<iterator>;
    using iterator_category = typename traits::iterator_category;
    using value_type = typename traits::value_type;
    using reference = typename traits::reference;
    using difference_type = typename traits::difference_type;
    using size_type = difference_type;

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

    template <class Range, CONCEPT = cc::InputRange<Range>>
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

    auto back() const -> reference
    {
        EXPECTS(!empty());
        return *std::prev(end());
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

    operator bool() const
    {
        return !empty();
    }


    template <class Container, CONCEPT = cc::Constructible<Container, iterator, iterator>>
    operator Container() const
    {
        return { begin(), end() };
    }

    template <size_t Index>
    auto get() const -> iterator
    {
        if constexpr (Index == 0)
        {
            return begin();
        }
        else if constexpr (Index == 1)
        {
            return end();
        }
    }

private:
    iterator _begin;
    iterator _end;
};



template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator ==(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::equal(std::begin(rhs), std::end(rhs), lhs.begin(), lhs.end());
}

template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator !=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs == rhs);
}

template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator <(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), std::begin(rhs), std::end(rhs));
}

template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator >(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return rhs < lhs;
}

template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator <=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs > rhs);
}

template <class Iter, class Range, CONCEPT = cc::InputRange<Range>>
auto operator >=(const iterator_range<Iter>& lhs, Range&& rhs) -> bool
{
    return !(lhs < rhs);
}


struct make_range_fn
{
    template <class Iter, CONCEPT = cc::InputIterator<Iter>>
    auto operator ()(Iter begin, Iter end) const -> iterator_range<Iter>
    {
        return iterator_range<Iter>{ begin, end };
    }

    template <class Iter, CONCEPT = cc::InputIterator<Iter>>
    auto operator ()(const std::pair<Iter, Iter>& pair) const -> iterator_range<Iter>
    {
        return (*this)(std::get<0>(pair), std::get<1>(pair));
    }

    template <class Range, CONCEPT = cc::InputRange<Range>>
    auto operator ()(Range&& range) const
    {
        return (*this)(std::begin(range), std::end(range));
    }

    template <class T>
    auto operator ()(const optional<T>& item) const
    {
        return from_optional(item);
    }

    template <class T>
    auto operator ()(optional<T>& item) const
    {
        return from_optional(item);
    }

    template <class T>
    auto operator ()(optional<T>&&) const = delete;

    template <class T>
    auto operator ()(const std::optional<T>& item) const
    {
        return from_optional(item);
    }

    template <class T>
    auto operator ()(std::optional<T>& item) const
    {
        return from_optional(item);
    }

    template <class T>
    auto operator ()(std::optional<T>&&) const = delete;

private:
    template <class T>
    auto from_optional(T&& item) const
    {
        using Iter = decltype(&(*item));
        return item
            ? iterator_range<Iter>{ &(*item), 1 }
            : iterator_range<Iter>{};
    }


};

static constexpr make_range_fn make_range = {};

} /* namespace cpp_essentials::core */

namespace std
{

template <class Iter>
struct tuple_size<::cpp_essentials::core::iterator_range<Iter>> : std::integral_constant<size_t, 2> { };

template <size_t Index, class Iter>
struct tuple_element<Index, ::cpp_essentials::core::iterator_range<Iter>> { using type = Iter; };

} /* namespace std */

#endif /* CPP_ESSENTIALS_CORE_ITERATOR_RANGE_HPP_ */
