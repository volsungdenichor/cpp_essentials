#ifndef CPP_ESSENTIALS_SQ_SQ_HPP_
#define CPP_ESSENTIALS_SQ_SQ_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/reverse.hpp>
#include <cpp_essentials/core/chain.hpp>
#include <cpp_essentials/core/chunk.hpp>
#include <cpp_essentials/core/container_helpers.hpp>
#include <cpp_essentials/core/containers.hpp>
#include <cpp_essentials/core/elementwise.hpp>
#include <cpp_essentials/core/enumerate.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/flat_map.hpp>
#include <cpp_essentials/core/flatten.hpp>
#include <cpp_essentials/core/group_by.hpp>
#include <cpp_essentials/core/iterate.hpp>
#include <cpp_essentials/core/join.hpp>
#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/map_utils.hpp>
#include <cpp_essentials/core/numeric.hpp>
#include <cpp_essentials/core/repeat.hpp>
#include <cpp_essentials/core/replace.hpp>
#include <cpp_essentials/core/slice.hpp>
#include <cpp_essentials/core/stride.hpp>
#include <cpp_essentials/core/trim.hpp>
#include <cpp_essentials/core/unzip.hpp>
#include <cpp_essentials/core/zip.hpp>

#include <cpp_essentials/core/output.hpp>

namespace cpp_essentials::sq
{

#define PULL_ADAPTABLE(func) \
    static constexpr auto func = core::adaptable{ core::func }

using core::make_range;

using core::range;
using core::inclusive_range;
using core::infinite_range;
using core::ints;
using core::repeat;
using core::once;

using core::write;
using core::join;

using core::group_by;

using core::map_at;
using core::map_try_get;
using core::map_get;
using core::at;
using core::try_at;

PULL_ADAPTABLE(front);
PULL_ADAPTABLE(front_or_throw);
PULL_ADAPTABLE(front_or);
PULL_ADAPTABLE(front_or_default);
PULL_ADAPTABLE(front_or_eval);
PULL_ADAPTABLE(front_or_none);

PULL_ADAPTABLE(single);
PULL_ADAPTABLE(single_or_throw);
PULL_ADAPTABLE(single_or);
PULL_ADAPTABLE(single_or_default);
PULL_ADAPTABLE(single_or_eval);
PULL_ADAPTABLE(single_or_none);

PULL_ADAPTABLE(size);
PULL_ADAPTABLE(empty);
PULL_ADAPTABLE(non_empty);
PULL_ADAPTABLE(copy_while);
PULL_ADAPTABLE(copy_until);
PULL_ADAPTABLE(overwrite);
PULL_ADAPTABLE(starts_with);
PULL_ADAPTABLE(ends_with);
PULL_ADAPTABLE(contains);
PULL_ADAPTABLE(sum);
PULL_ADAPTABLE(min_value);
PULL_ADAPTABLE(max_value);
PULL_ADAPTABLE(minmax_value);

PULL_ADAPTABLE(accumulate);
PULL_ADAPTABLE(all_of);
PULL_ADAPTABLE(any_of);
PULL_ADAPTABLE(copy);
PULL_ADAPTABLE(copy_if);
PULL_ADAPTABLE(copy_n);
PULL_ADAPTABLE(count);
PULL_ADAPTABLE(count_if);
PULL_ADAPTABLE(for_each);
PULL_ADAPTABLE(is_heap);
PULL_ADAPTABLE(is_partitioned);
PULL_ADAPTABLE(is_permutation);
PULL_ADAPTABLE(is_sorted);
PULL_ADAPTABLE(lower_bound);
PULL_ADAPTABLE(max_element);
PULL_ADAPTABLE(minmax_element);
PULL_ADAPTABLE(min_element);
PULL_ADAPTABLE(move);
PULL_ADAPTABLE(none_of);
PULL_ADAPTABLE(partition_copy);
PULL_ADAPTABLE(remove_copy);
PULL_ADAPTABLE(remove_copy_if);
PULL_ADAPTABLE(replace_copy);
PULL_ADAPTABLE(replace_copy_if);
PULL_ADAPTABLE(reverse_copy);
PULL_ADAPTABLE(rotate_copy);
PULL_ADAPTABLE(unique_copy);
PULL_ADAPTABLE(upper_bound);

using core::views::reverse;

using core::views::chain;
using core::views::append;
using core::views::prepend;

using core::views::chunk;
using core::views::slide;
using core::views::group;
using core::views::pairwise;

using core::to_vector;
using core::to_set;
using core::to_list;
using core::to_forward_list;
using core::to_deque;
using core::to_string;
using core::to_string_view;

using core::views::filter;
using core::views::take_if;
using core::views::drop_if;

using core::views::flat_map;
using core::views::flatten;

using core::views::enumerate;

using core::views::iterate;

using core::views::transform;
using core::views::map;
using core::views::replace_if;
using core::views::replace;

using core::views::slice;
using core::views::take;
using core::views::drop;
using core::views::take_back;
using core::views::drop_back;
using core::views::take_exactly;
using core::views::drop_exactly;
using core::views::take_back_exactly;
using core::views::drop_back_exactly;
using core::views::split_before;
using core::views::split_after;
using core::views::split;

using core::views::stride;

using core::views::take_while;
using core::views::drop_while;
using core::views::take_until;
using core::views::drop_until;
using core::views::take_back_while;
using core::views::drop_back_while;
using core::views::take_back_until;
using core::views::drop_back_until;
using core::views::trim_while;
using core::views::trim_until;
using core::views::split_while;
using core::views::split_until;

using core::views::zip;
using core::views::unzip;

#undef PULL_ADAPTABLE

} /* namespace sq */

#endif /* CPP_ESSENTIALS_SQ_SQ_HPP_ */
