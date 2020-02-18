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
#include <cpp_essentials/core/filter_map.hpp>
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

#define PULL_ADAPTABLE_VIEW(func) \
    static constexpr auto func = core::adaptable{ core::views::func }

using core::make_range;

using core::range;
using core::inclusive_range;
using core::infinite_range;
using core::ints;
using core::repeat;
using core::once;

PULL_ADAPTABLE(write);
PULL_ADAPTABLE(join);

template <template <class, class> class Map>
static constexpr auto group_by_as = core::adaptable{ core::group_by_as<Map> };

static constexpr auto group_by = group_by_as<std::unordered_map>;

PULL_ADAPTABLE(map_equal_range);
PULL_ADAPTABLE(map_at);
PULL_ADAPTABLE(map_try_get);
PULL_ADAPTABLE(map_get);

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

PULL_ADAPTABLE(at);
PULL_ADAPTABLE(at_or_throw);
PULL_ADAPTABLE(at_or);
PULL_ADAPTABLE(at_or_default);
PULL_ADAPTABLE(at_or_eval);
PULL_ADAPTABLE(at_or_none);

PULL_ADAPTABLE(size);
PULL_ADAPTABLE(empty);
PULL_ADAPTABLE(non_empty);
PULL_ADAPTABLE(copy_while);
PULL_ADAPTABLE(copy_until);
PULL_ADAPTABLE(overwrite);
PULL_ADAPTABLE(starts_with);
PULL_ADAPTABLE(ends_with);
PULL_ADAPTABLE(contains);
PULL_ADAPTABLE(starts_with_element);
PULL_ADAPTABLE(ends_with_element);
PULL_ADAPTABLE(contains_element);
PULL_ADAPTABLE(all_equal);
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

PULL_ADAPTABLE_VIEW(reverse);

PULL_ADAPTABLE_VIEW(chain);
PULL_ADAPTABLE_VIEW(append);
PULL_ADAPTABLE_VIEW(prepend);

PULL_ADAPTABLE_VIEW(chunk);
PULL_ADAPTABLE_VIEW(slide);
PULL_ADAPTABLE_VIEW(group);
PULL_ADAPTABLE_VIEW(pairwise);

template <class Container>
static constexpr auto to = core::adaptable{ core::to<Container> };

PULL_ADAPTABLE(to_vector);
PULL_ADAPTABLE(to_set);
PULL_ADAPTABLE(to_list);
PULL_ADAPTABLE(to_forward_list);
PULL_ADAPTABLE(to_deque);
PULL_ADAPTABLE(to_string);
PULL_ADAPTABLE(to_string_view);
PULL_ADAPTABLE(to_ref_vector);

PULL_ADAPTABLE_VIEW(filter);
PULL_ADAPTABLE_VIEW(take_if);
PULL_ADAPTABLE_VIEW(drop_if);

PULL_ADAPTABLE_VIEW(filter_map);
PULL_ADAPTABLE_VIEW(flatten);
PULL_ADAPTABLE_VIEW(flat_map);

PULL_ADAPTABLE_VIEW(enumerate);

PULL_ADAPTABLE_VIEW(iterate);

PULL_ADAPTABLE_VIEW(transform);
PULL_ADAPTABLE_VIEW(map);
PULL_ADAPTABLE_VIEW(replace_if);
PULL_ADAPTABLE_VIEW(replace);

PULL_ADAPTABLE_VIEW(slice);
PULL_ADAPTABLE_VIEW(take);
PULL_ADAPTABLE_VIEW(drop);
PULL_ADAPTABLE_VIEW(take_back);
PULL_ADAPTABLE_VIEW(drop_back);
PULL_ADAPTABLE_VIEW(take_exactly);
PULL_ADAPTABLE_VIEW(drop_exactly);
PULL_ADAPTABLE_VIEW(take_back_exactly);
PULL_ADAPTABLE_VIEW(drop_back_exactly);
PULL_ADAPTABLE_VIEW(split_before);
PULL_ADAPTABLE_VIEW(split_after);
PULL_ADAPTABLE_VIEW(split);

PULL_ADAPTABLE_VIEW(stride);

PULL_ADAPTABLE_VIEW(take_while);
PULL_ADAPTABLE_VIEW(drop_while);
PULL_ADAPTABLE_VIEW(take_until);
PULL_ADAPTABLE_VIEW(drop_until);
PULL_ADAPTABLE_VIEW(take_back_while);
PULL_ADAPTABLE_VIEW(drop_back_while);
PULL_ADAPTABLE_VIEW(take_back_until);
PULL_ADAPTABLE_VIEW(drop_back_until);
PULL_ADAPTABLE_VIEW(trim_while);
PULL_ADAPTABLE_VIEW(trim_until);
PULL_ADAPTABLE_VIEW(split_while);
PULL_ADAPTABLE_VIEW(split_until);

PULL_ADAPTABLE_VIEW(zip);
PULL_ADAPTABLE_VIEW(unzip);

#undef PULL_ADAPTABLE
#undef PULL_ADAPTABLE_VIEW

} /* namespace sq */

#endif /* CPP_ESSENTIALS_SQ_SQ_HPP_ */
