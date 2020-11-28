#pragma once

#include <millrind/core/iterator_facade.hpp>
#include <millrind/core/functors.hpp>

namespace millrind::core
{

template <class Iter>
class enumerating_iterator : public iterator_facade<enumerating_iterator<Iter>>
{
public:
	enumerating_iterator() = default;

	enumerating_iterator(Iter iter, std::ptrdiff_t index)
		: _iter{ std::move(iter) }
		, _index{ index }
	{
	}

	enumerating_iterator(const enumerating_iterator&) = default;

	enumerating_iterator(enumerating_iterator&&) = default;

	auto deref() const -> std::pair<std::ptrdiff_t, iter_ref<Iter>>
	{
		return { _index, *_iter };
	}

	void inc()
	{
		++_iter;
		++_index;
	}

	template <class It = Iter, class = BidirectionalIterator<It>>
	void dec()
	{
		--_iter;
		--_index;
	}

	bool is_equal(const enumerating_iterator& other) const
	{
		return _iter == other._iter;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	bool is_less(const enumerating_iterator& other) const
	{
		return _iter < other._iter;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	void advance(std::ptrdiff_t offset)
	{
		_iter += offset;
		_index += offset;
	}

	template <class It = Iter, class = RandomAccessIterator<It>>
	auto distance_to(const enumerating_iterator& other) const
	{
		return other._iter - _iter;
	}

private:
	Iter _iter;
	std::ptrdiff_t _index;
};

} // namespace millrind::core

CORE_ITERATOR_TRAIRS(::millrind::core::enumerating_iterator)
