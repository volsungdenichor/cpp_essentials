#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>
#include <millrind/core/detail/default_constructible_func.hpp>

namespace millrind::core
{

template <class Func, class Iter>
class map_iterator : public iterator_facade<map_iterator<Func, Iter>>
{
public:
	map_iterator() = default;

	map_iterator(Func func, Iter iter)
		: _func{ std::move(func) }
		, _iter{ std::move(iter) }
	{
	}

	map_iterator(const map_iterator&) = default;

	map_iterator(map_iterator&&) = default;

	decltype(auto) deref() const
	{
		return invoke_func(_func, *_iter);
	}

	void inc()
	{
		++_iter;
	}

	template <class It = Iter, class = BidirectionalIterator<It>>
	void dec()
	{
		--_iter;
	}

	bool is_equal(const map_iterator& other) const
	{
		return _iter == other._iter;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	bool is_less(const map_iterator& other) const
	{
		return _iter < other._iter;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	void advance(std::ptrdiff_t offset)
	{
		_iter += offset;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	auto distance_to(const map_iterator& other) const
	{
		return other._iter - _iter;
	}

private:
	detail::default_constructible_func<Func> _func;
	Iter _iter;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::map_iterator)
