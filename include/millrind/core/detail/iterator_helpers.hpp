#pragma once

#include <millrind/core/type_traits.hpp>
#include <algorithm>

namespace millrind::core
{

namespace detail
{

struct advance_iterator_fn
{
	template <class Iter, class Diff>
	auto operator ()(Iter b, Iter e, Diff n) const -> Iter
	{
		if constexpr (is_detected_v<RandomAccessIterator, Iter>)
		{
			return std::next(b, std::min<Diff>(std::distance(b, e), n));
		}
		else
		{
			while (b != e && n > 0)
			{
				++b;
				--n;
			}
			return b;
		}
	}
};

static constexpr auto advance_iterator = advance_iterator_fn{};

struct advance_iterator_while_fn
{
	template <class Iter, class UnaryPred, class Diff>
	auto operator ()(Iter b, Iter e, UnaryPred pred, Diff diff = {}) const -> Iter
	{
		while (b != e && invoke_func(pred, *b))
		{
			++b;
		}
		return advance_iterator(b, e, diff);
	}
};

static constexpr auto advance_iterator_while = advance_iterator_while_fn{};

} // namespace detail

} // millrind::core
