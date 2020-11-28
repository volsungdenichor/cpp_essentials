#pragma once

#include <utility>
#include <algorithm>
#include <numeric>

#include <millrind/core/opt.hpp>
#include <millrind/core/type_traits.hpp>
#include <millrind/core/detail/iterator_helpers.hpp>

#include <millrind/core/iterators.hpp>

namespace millrind::core
{

struct range_opt
{
    static constexpr int exclusive = 0;
    static constexpr int inclusive = 1;
};

template <class Iter>
class iterator_range;

namespace detail
{

struct make_range_fn
{
    template <class Iter>
    auto operator ()(Iter begin, Iter end) const -> iterator_range<Iter>
    {
        return { std::move(begin), std::move(end) };
    }

    template <class Iter>
    auto operator ()(std::pair<Iter, Iter> pair) const -> iterator_range<Iter>
    {
        return (*this)(std::get<0>(pair), std::get<1>(pair));
    }

    template <class Range>
    auto operator ()(Range&& range) const
    {
        return (*this)(std::begin(range), std::end(range));
    }
};

} // namespace detail

static constexpr auto make_range = detail::make_range_fn{};

template <class Iter>
class iterator_range
{
private:
    using iterator_traits = std::iterator_traits<Iter>;

public:
    using iterator = Iter;
    using iterator_category = iter_cat<iterator>;
    using reference = iter_ref<iterator>;
    using value_type = iter_val<iterator>;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reverse_iterator = decltype(make_reverse_iterators(std::declval<iterator>(), std::declval<iterator>()));

    constexpr iterator_range() = default;

    constexpr iterator_range(iterator begin, iterator end)
        : _begin{ std::move(begin) }
        , _end{ std::move(end) }
    {
    }

    constexpr iterator_range(std::pair<iterator, iterator> pair)
        : iterator_range{ std::move(std::get<0>(pair)), std::move(std::get<1>(pair)) }
    {
    }

    constexpr iterator_range(const iterator_range&) = default;

    constexpr iterator_range(iterator_range&&) = default;

    template <class Range, class = InputRange<Range>>
    constexpr iterator_range(Range&& range)
        : iterator_range{ std::begin(range), std::end(range) }
    {
    }

    constexpr iterator_range& operator =(iterator_range other)
    {
        std::swap(_begin, other._begin);
        std::swap(_end, other._end);
        return *this;
    }

    template <class Container, class = std::enable_if_t<std::is_constructible_v<Container, iterator, iterator>>>
    constexpr Container to() const
    {
        return { begin(), end() };
    }

    template <class Container, class = std::enable_if_t<std::is_constructible_v<Container, iterator, iterator>>>
    constexpr operator Container() const
    {
        return to<Container>();
    }

    template <template<class...> class Container>
    constexpr Container<value_type> to() const
    {
        return to<Container<value_type>>();
    }

    constexpr iterator begin() const
    {
        return _begin;
    }

    constexpr iterator end() const
    {
        return _end;
    }

    constexpr size_type size() const
    {
        return (size_type)std::distance(begin(), end());
    }

    constexpr bool empty() const
    {
        return begin() == end();
    }

    constexpr reference operator [](size_type index) const
    {
        return *std::next(begin(), index);
    }

    constexpr reference at(size_type index) const
    {
        return *std::next(begin(), index);
    }

    constexpr reference front() const
    {
        return *begin();
    }

    constexpr reference back() const
    {
        return reverse().front();
    }

    constexpr opt<reference> first() const
    {
        return !empty()
            ? opt<reference>{ front() }
        : opt<reference>{};
    }

    constexpr opt<reference> get(size_type index) const
    {
        return drop(index).first();
    }

    constexpr iterator_range<reverse_iterator> reverse() const
    {
        return make_range(make_reverse_iterators(begin(), end()));
    }

    constexpr iterator_range take(size_type count) const
    {
        return make_range(
            begin(),
            advance(count));
    }

    constexpr iterator_range drop(size_type count) const
    {
        return make_range(
            advance(count),
            end());
    }

    template <class Pred>
    constexpr iterator_range take_while(Pred&& pred, difference_type offset = {}) const
    {
        return make_range(
            begin(),
            find_if(FORWARD(pred), offset));
    }

    template <class Pred>
    constexpr iterator_range take_until(Pred&& pred, difference_type offset = {}) const
    {
        return take_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    constexpr iterator_range drop_while(Pred&& pred, difference_type offset = {}) const
    {
        return make_range(
            find_if(FORWARD(pred), offset),
            end());
    }

    template <class Pred>
    constexpr iterator_range drop_until(Pred&& pred, difference_type offset = {}) const
    {
        return drop_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    constexpr iterator_range take_back(size_type count) const
    {
        return reverse()
            .take(count)
            .reverse();
    }

    constexpr iterator_range drop_back(size_type count) const
    {
        return reverse()
            .drop(count)
            .reverse();
    }

    template <class Pred>
    constexpr iterator_range take_back_while(Pred&& pred, difference_type offset = {}) const
    {
        return reverse()
            .take_while(wrap_func(FORWARD(pred)), offset)
            .reverse();
    }

    template <class Pred>
    constexpr iterator_range take_back_until(Pred&& pred, difference_type offset = {}) const
    {
        return take_back_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    constexpr iterator_range drop_back_while(Pred&& pred, difference_type offset = {}) const
    {
        return reverse()
            .drop_while(wrap_func(FORWARD(pred)), offset)
            .reverse();
    }

    template <class Pred>
    constexpr iterator_range drop_back_until(Pred&& pred, difference_type offset = {}) const
    {
        return drop_back_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    constexpr iterator_range trim(size_type count) const
    {
        return drop(count)
            .reverse()
            .drop(count)
            .reverse();
    }

    template <class Pred>
    constexpr iterator_range trim_while(Pred&& pred, difference_type offset = {}) const
    {
        return drop_while(pred, offset)
            .reverse()
            .drop_while(pred, offset)
            .reverse();
    }

    template <class Pred>
    constexpr iterator_range trim_until(Pred&& pred, difference_type offset = {}) const
    {
        return trim_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    constexpr bool matches(Pred&& pred) const
    {
        return invoke_func(FORWARD(pred), *this);
    }

    template <class Pred>
    constexpr bool all_of(Pred&& pred) const
    {
        return std::all_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Pred>
    constexpr bool any_of(Pred&& pred) const
    {
        return std::any_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Pred>
    constexpr bool none_of(Pred&& pred) const
    {
        return std::none_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Func>
    constexpr void for_each(Func&& func) const
    {
        std::for_each(begin(), end(), wrap_func(FORWARD(func)));
    }

    template <class Output>
    constexpr Output copy(Output output) const
    {
        return std::copy(begin(), end(), std::move(output));
    }

    template <class Func>
    constexpr auto map(Func&& func) const
    {
        return make_range(make_map_iterators(FORWARD(func), begin(), end()));
    }

    template <class Pred>
    constexpr auto filter(Pred&& pred) const
    {
        return make_range(make_filter_iterators(FORWARD(pred), begin(), end()));
    }

    template <class Pred>
    constexpr auto take_if(Pred&& pred) const
    {
        return filter(FORWARD(pred));
    }

    template <class Pred>
    constexpr auto drop_if(Pred&& pred) const
    {
        return filter(logical_negation(FORWARD(pred)));
    }

    template <class Func>
    constexpr auto filter_map(Func&& func) const
    {
        return make_range(make_filter_map_iterators(FORWARD(func), begin(), end()));
    }

    constexpr auto iterate() const
    {
        return make_range(make_iterate_iterators(begin(), end()));
    }

    const auto enumerate() const
    {
        return make_range(make_enumerating_iterators(begin(), end()));
    }

    template <class Func>
    constexpr auto flat_map(Func&& func) const
    {
        return make_range(make_flat_map_iterators(FORWARD(func), begin(), end()));
    }

#if 0
    constexpr auto flatten() const
    {
        return make_range(make_flatten_iterators(begin(), end()));
    }
#endif

    template <class Range>
    auto chain(Range&& r) const
    {
        return make_range(make_chain_iterators(begin(), end(), std::begin(r), std::end(r)));
    }

    template <class Func, class T>
    T accumulate(Func&& func, T init) const
    {
        return std::accumulate(begin(), end(), std::move(init), FORWARD(func));
    }

private:
    template <class Pred>
    constexpr iterator find_if(Pred&& pred, difference_type offset) const
    {
        return detail::advance_iterator_while(begin(), end(), wrap_func(FORWARD(pred)), offset);
    }

    constexpr iterator advance(difference_type count) const
    {
        return detail::advance_iterator(begin(), end(), count);
    }

public:
    iterator _begin, _end;
};

template <class T>
using span = iterator_range<T*>;

template <class T>
using const_span = iterator_range<const T*>;

template <class T>
using cspan = const_span<T>;


template <class Container>
using view = iterator_range<range_iter<Container>>;

template <class Container>
using const_view = iterator_range<range_iter<const Container>>;

template <class Container>
using cview = const_view<Container>;

template <class T>
using iterable = iterator_range<any_iterator<T>>;


struct range_fn
{
    template <class T>
    auto operator ()(T lo, T up) const
    {
        return make_range(make_numeric_iterators(lo, up));
    }

    template <class T>
    auto operator ()(T up) const
    {
        return (*this)(T{}, up);
    }
};

struct zip_fn
{
    template <class Range1, class Range2, class Func>
    auto operator ()(Range1&& range1, Range2&& range2, Func&& func) const
    {
        return make_range(make_zip_iterators(func, FORWARD(range1), FORWARD(range2)));
    }

    template <class Range1, class Range2>
    auto operator ()(Range1&& range1, Range2&& range2) const
    {
        using type1 = range_ref<Range1>;
        using type2 = range_ref<Range2>;
        return (*this)(FORWARD(range1), FORWARD(range2), [](type1 lt, type2 rt) { return std::pair<type1, type2>{ FORWARD(lt), FORWARD(rt) }; });
    }
};

struct repeat_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return make_range(make_repeat_iterators(std::move(value), std::numeric_limits<std::ptrdiff_t>::max()));
    }
};

struct once_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return make_range(make_repeat_iterators(std::move(value), 1));
    }
};

struct owned_fn
{
    template <class Container>
    auto operator ()(Container container) const
    {
        return make_range(make_owning_iterators(std::move(container)));
    }

    template <class T>
    auto operator ()(std::initializer_list<T> init) const
    {
        return (*this)(std::vector<T>{ init });
    }
};

struct make_generator_fn
{
    template <class Func>
    auto operator ()(Func&& func) const
    {
        return make_range(make_generating_iterators(FORWARD(func)));
    }
};

static constexpr inline auto range = range_fn{};
static constexpr inline auto zip = zip_fn{};
static constexpr inline auto repeat = repeat_fn{};
static constexpr inline auto once = once_fn{};
static constexpr inline auto owned = owned_fn{};
static constexpr inline auto make_generator = make_generator_fn{};

} // namespace millrind::core
