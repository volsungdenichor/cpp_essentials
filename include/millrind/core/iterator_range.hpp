#pragma once

#include <millrind/core/type_traits.hpp>
#include <millrind/core/detail/iterator_helpers.hpp>
#include <utility>
#include <algorithm>
#include <numeric>

#include "iterators/map_iterator.hpp"
#if 0
#include "iterators/filter_iterator.hpp"
#include "iterators/iterate_iterator.hpp"
#include "iterators/chain_iterator.hpp"
#include "iterators/filter_map_iterator.hpp"
#include "iterators/flat_map_iterator.hpp"
#include "iterators/zip_iterator.hpp"
#include "iterators/repeat_iterator.hpp"
#include "iterators/owning_iterator.hpp"
#include "iterators/numeric_iterator.hpp"
#include "iterators/any_iterator.hpp"
#endif

namespace millrind::core
{

struct range_opt
{
	static constexpr int exclusive = 0;
	static constexpr int inclusive = 1;
};

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
	using reverse_iterator = decltype(detail::make_reverse_iterator(std::declval<iterator>()));

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
	constexpr operator Container() const
	{
		return { begin(), end() };
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

#if 0
	core::optional<reference> first() const
	{
		return !empty()
			? core::optional<reference>{ front() }
		: core::none;
	}

	core::optional<reference> get(size_type index) const
	{
		return drop(index).first();
	}
#endif

	iterator_range<reverse_iterator> reverse() const
	{
		return { detail::make_reverse_iterator(end()), detail::make_reverse_iterator(begin()) };
	}

	iterator_range take(size_type count) const
	{
		return { begin(), detail::advance(begin(), end(), count) };
	}

	iterator_range drop(size_type count) const
	{
		return { detail::advance(begin(), end(), count), end() };
	}

	template <class Pred>
	iterator_range take_while(Pred&& pred, difference_type offset = {}) const
	{
		return { begin(), detail::advance_while(begin(), end(), std::forward<Pred>(pred), offset) };
	}

	template <class Pred>
	iterator_range take_until(Pred&& pred, difference_type offset = {}) const
	{
		return take_while(logical_negation(std::forward<Pred>(pred)), offset);
	}

	template <class Pred>
	iterator_range drop_while(Pred&& pred, difference_type offset = {}) const
	{
		return { detail::advance_while(begin(), end(), std::forward<Pred>(pred), offset), end() };
	}

	template <class Pred>
	iterator_range drop_until(Pred&& pred, difference_type offset = {}) const
	{
		return drop_while(logical_negation(std::forward<Pred>(pred)), offset);
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
		return reverse().take_while(std::forward<Pred>(pred), offset).reverse();
	}

	template <class Pred>
	iterator_range take_back_until(Pred&& pred, difference_type offset = {}) const
	{
		return take_back_while(logical_negation(std::forward<Pred>(pred)), offset);
	}

	template <class Pred>
	iterator_range drop_back_while(Pred&& pred, difference_type offset = {}) const
	{
		return reverse().drop_while(std::forward<Pred>(pred), offset).reverse();
	}

	template <class Pred>
	iterator_range drop_back_until(Pred&& pred, difference_type offset = {}) const
	{
		return drop_back_while(logical_negation(std::forward<Pred>(pred)), offset);
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
		return trim_while(logical_negation(std::forward<Pred>(pred)), offset);
	}

	template <class Pred>
	bool all_of(Pred&& pred) const
	{
		return std::all_of(begin(), end(), std::forward<Pred>(pred));
	}

	template <class Pred>
	bool any_of(Pred&& pred) const
	{
		return std::any_of(begin(), end(), std::forward<Pred>(pred));
	}

	template <class Pred>
	bool none_of(Pred&& pred) const
	{
		return std::none_of(begin(), end(), std::forward<Pred>(pred));
	}

	template <class Func>
	void for_each(Func&& func) const
	{
		std::for_each(begin(), end(), std::forward<Func>(func));
	}

	template <class Output>
	Output copy(Output output) const
	{
		return std::copy(begin(), end(), std::move(output));
	}

	template <class Func>
	auto map(Func func) const
	{
		auto f = make_func(std::move(func));
		using iter = map_iterator<decltype(f), iterator>;
		return iterator_range<iter>{
			iter{ f, begin() },
			iter{ f, end() }
		};
	}

#if 0
	template <class Pred>
	auto filter(Pred pred) const
	{
		auto p = make_func(std::move(pred));
		using iter = filter_iterator<decltype(p), iterator>;
		return iterator_range<iter>{
			iter{ p, begin(), end() },
				iter{ p, end(), end() }
		};
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

	auto iterate() const
	{
		using iter = iterate_iterator<iterator>;
		return iterator_range<iter>{
			begin(),
				end()
		};
	}

	template <class Func>
	auto flat_map(Func func) const
	{
		auto f = make_func(std::move(func));
		using iter = flat_map_iterator<decltype(f), iterator>;
		return iterator_range<iter>{
			{ f, begin(), end() },
			{ f, end(), end() }
		};
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

		using iter = chain_iterator<iterator, decltype(b)>;
		return iterator_range<iter>{
			{ begin(), b, end(), e },
			{ end(), e, end(), b }
		};
	}

	template <class Func>
	auto filter_map(Func func) const
	{
		auto f = make_func(std::move(func));
		using iter = filter_map_iterator<decltype(f), iterator>;
		return iterator_range<iter>{
			{ f, begin(), end() },
			{ f, end(), end() }
		};
	}
#endif

	iterator _begin, _end;
};

template <class T>
using span = iterator_range<T*>;

template <class T>
using const_span = iterator_range<const T*>;

template <class T>
using cspan = const_span<T>;


template <class Container>
using view = iterator_range<decltype(std::begin(std::declval<Container>()))>;

template <class Container>
using const_view = iterator_range<decltype(std::begin(std::declval<const Container>()))>;

template <class Container>
using cview = const_view<Container>;

//template <class T>
//using iterable = iterator_range<any_iterator<T>>;


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

struct range_fn
{
#if 0

	template <class T>
	auto operator ()(T lo, T up) const
	{
		using iter = numeric_iterator<T>;
		return iterator_range<iter>{
			{ lo },
			{ up }
		};
	}

	template <class T>
	auto operator ()(T up) const
	{
		return (*this)(T{}, up);
	}
#endif
};

struct zip_fn
{
#if 0

	template <class Func, class... Iters>
	static auto make_zip_iterator(Func func, const Iters& ... args)
	{
		return zip_iterator<Func, Iters...>{ func, args... };
	}

	template <typename Func, typename... Ranges>
	auto operator ()(Func&& func, Ranges&& ... ranges) const
	{
		return from(
			make_zip_iterator(func, std::begin(ranges)...),
			make_zip_iterator(func, std::end(ranges)...));
	}
#endif
};

struct repeat_fn
{
#if 0

	template <class T>
	auto operator ()(T value) const
	{
		using iter = repeat_iterator<T>;
		return iterator_range<iter>{
			{ value, 0 },
			{ value, std::numeric_limits<std::ptrdiff_t>::max() }
		};
	}
#endif
};

struct once_fn
{
#if 0

	template <class T>
	auto operator ()(T value) const
	{
		using iter = repeat_iterator<T>;
		return iterator_range<iter>{
			{ value, 0 },
			{ value, 1 }
		};
	}
#endif
};

struct owned_fn
{
#if 0

	template <class Container>
	auto operator ()(Container container) const
	{
		auto ptr = std::make_shared<Container>(std::move(container));
		using iter = owning_iterator<decltype(std::begin(*ptr)), Container>;
		return iterator_range<iter>{
			{ std::begin(*ptr), ptr },
			{ std::end(*ptr), ptr }
		};
	}

	template <class T>
	auto operator ()(std::initializer_list<T> init) const
	{
		return (*this)(std::vector<T>{ init });
	}
#endif
};

static constexpr inline auto range = range_fn{};
static constexpr inline auto zip = zip_fn{};
static constexpr inline auto repeat = repeat_fn{};
static constexpr inline auto once = once_fn{};
static constexpr inline auto owned = owned_fn{};

} // namespace millrind::core
