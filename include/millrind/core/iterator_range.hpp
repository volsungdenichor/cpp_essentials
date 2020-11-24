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
    using reverse_iterator = decltype(make_reverse_iterator(std::declval<iterator>()));

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

    template <class Range>
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

    reference operator [](size_type index) const
    {
        return *std::next(begin(), index);
    }

    reference at(size_type index) const
    {
        return *std::next(begin(), index);
    }

    reference front() const
    {
        return *begin();
    }

    reference back() const
    {
        return reverse().front();
    }

    opt<reference> first() const
    {
        return !empty()
            ? opt<reference>{ front() }
        : none;
    }

    opt<reference> get(size_type index) const
    {
        return drop(index).first();
    }

    iterator_range<reverse_iterator> reverse() const
    {
        return make_range(
            make_reverse_iterator(end()),
            make_reverse_iterator(begin()));
    }

    iterator_range take(size_type count) const
    {
        return make_range(
            begin(),
            advance(count));
    }

    iterator_range drop(size_type count) const
    {
        return make_range(
            advance(count),
            end());
    }

    template <class Pred>
    iterator_range take_while(Pred&& pred, difference_type offset = {}) const
    {
        return make_range(
            begin(),
            find_if(FORWARD(pred), offset));
    }

    template <class Pred>
    iterator_range take_until(Pred&& pred, difference_type offset = {}) const
    {
        return take_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    iterator_range drop_while(Pred&& pred, difference_type offset = {}) const
    {
        return make_range(
            find_if(FORWARD(pred), offset),
            end());
    }

    template <class Pred>
    iterator_range drop_until(Pred&& pred, difference_type offset = {}) const
    {
        return drop_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    iterator_range take_back(size_type count) const
    {
        return reverse().take(count).reverse();
    }

    iterator_range drop_back(size_type count) const
    {
        return reverse().drop(count).reverse();
    }

    template <class Pred>
    iterator_range take_back_while(Pred&& pred, difference_type offset = {}) const
    {
        return reverse().take_while(wrap_func(FORWARD(pred)), offset).reverse();
    }

    template <class Pred>
    iterator_range take_back_until(Pred&& pred, difference_type offset = {}) const
    {
        return take_back_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    iterator_range drop_back_while(Pred&& pred, difference_type offset = {}) const
    {
        return reverse().drop_while(wrap_func(FORWARD(pred)), offset).reverse();
    }

    template <class Pred>
    iterator_range drop_back_until(Pred&& pred, difference_type offset = {}) const
    {
        return drop_back_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    iterator_range trim(size_type count) const
    {
        return drop(count).reverse().drop(count).reverse();
    }

    template <class Pred>
    iterator_range trim_while(Pred&& pred, difference_type offset = {}) const
    {
        return drop_while(pred, offset).reverse().drop_while(pred, offset).reverse();
    }

    template <class Pred>
    iterator_range trim_until(Pred&& pred, difference_type offset = {}) const
    {
        return trim_while(logical_negation(wrap_func(FORWARD(pred))), offset);
    }

    template <class Pred>
    bool all_of(Pred&& pred) const
    {
        return std::all_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Pred>
    bool any_of(Pred&& pred) const
    {
        return std::any_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Pred>
    bool none_of(Pred&& pred) const
    {
        return std::none_of(begin(), end(), wrap_func(FORWARD(pred)));
    }

    template <class Func>
    void for_each(Func&& func) const
    {
        std::for_each(begin(), end(), wrap_func(FORWARD(func)));
    }

    template <class Output>
    Output copy(Output output) const
    {
        return std::copy(begin(), end(), std::move(output));
    }

    template <class Func>
    auto map(Func func) const
    {
        return make_range(
            make_map_iterator(func, begin()),
            make_map_iterator(func, end()));
    }

    template <class Pred>
    auto filter(Pred pred) const
    {
        return make_range(
            make_filter_iterator(pred, begin(), end()),
            make_filter_iterator(pred, end(), end()));
    }

    template <class Pred>
    auto take_if(Pred pred) const
    {
        return filter(std::move(pred));
    }

    template <class Pred>
    auto drop_if(Pred pred) const
    {
        return filter(logical_negation(std::move(pred)));
    }

    template <class Func>
    auto filter_map(Func func) const
    {
        return make_range(
            make_filter_map_iterator(func, begin(), end()),
            make_filter_map_iterator(func, end(), end()));
    }

    auto iterate() const
    {
        return make_range(
            make_iterate_iterator(begin()),
            make_iterate_iterator(end()));
    }

    template <class Func>
    auto flat_map(Func func) const
    {
        return make_range(
            make_flat_map_iterator(func, begin(), end()),
            make_flat_map_iterator(func, end(), end()));
    }

    auto flatten() const
    {
        return flat_map(core::identity);
    }

    template <class Range>
    auto chain(Range&& r) const
    {
        auto b = std::begin(r);
        auto e = std::end(r);

        return make_range(
            make_chain_iterator(begin(), b, end(), e),
            make_chain_iterator(end(), e, end(), b));
    }

private:
    template <class Pred>
    iterator find_if(Pred&& pred, difference_type offset) const
    {
        return detail::advance_iterator_while(begin(), end(), wrap_func(FORWARD(pred)), offset);
    }

    iterator advance(difference_type count) const
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
        return make_range(
            make_iterate_iterator(lo),
            make_iterate_iterator(up));
    }

    template <class T>
    auto operator ()(T up) const
    {
        return (*this)(T{}, up);
    }
};

struct zip_fn
{
    template <typename Func, typename... Ranges>
    auto operator ()(Func&& func, Ranges&& ... ranges) const
    {
        return make_range(
            make_zip_iterator(func, std::begin(ranges)...),
            make_zip_iterator(func, std::end(ranges)...));
    }
};

struct repeat_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return make_range(
            make_repeat_iterator(value, 0),
            make_repeat_iterator(value, std::numeric_limits<std::ptrdiff_t>::max()));
    }
};

struct once_fn
{
    template <class T>
    auto operator ()(T value) const
    {
        return make_range(
            make_repeat_iterator(value, 0),
            make_repeat_iterator(value, 1));
    }
};

struct owned_fn
{
    template <class Container>
    auto operator ()(Container container) const
    {
        auto ptr = std::make_shared<Container>(std::move(container));
        return make_range(
            make_owning_iterator(ptr, std::begin(*ptr)),
            make_owning_iterator(ptr, std::end(*ptr)));
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
        return make_range(
            make_generating_iterator(FORWARD(func)),
            {});
    }
};

static constexpr inline auto range = range_fn{};
static constexpr inline auto zip = zip_fn{};
static constexpr inline auto repeat = repeat_fn{};
static constexpr inline auto once = once_fn{};
static constexpr inline auto owned = owned_fn{};
static constexpr inline auto make_generator = make_generator_fn{};

} // namespace millrind::core
