#ifndef CPP_ESSENTIALS_CORE_SQ_HPP_
#define CPP_ESSENTIALS_CORE_SQ_HPP_

#pragma once

#include <cpp_essentials/core/adaptor.hpp>
#include <cpp_essentials/core/algorithm.hpp>
#include <cpp_essentials/core/algorithm_ext.hpp>
#include <cpp_essentials/core/backwards.hpp>
#include <cpp_essentials/core/chain.hpp>
#include <cpp_essentials/core/chunk.hpp>
#include <cpp_essentials/core/containers.hpp>
#include <cpp_essentials/core/elementwise.hpp>
#include <cpp_essentials/core/filter.hpp>
#include <cpp_essentials/core/flatten.hpp>
#include <cpp_essentials/core/index.hpp>
#include <cpp_essentials/core/iterate.hpp>
#include <cpp_essentials/core/join.hpp>
#include <cpp_essentials/core/map.hpp>
#include <cpp_essentials/core/numeric.hpp>
#include <cpp_essentials/core/slice.hpp>
#include <cpp_essentials/core/stride.hpp>
#include <cpp_essentials/core/trim.hpp>
#include <cpp_essentials/core/zip.hpp>

namespace cpp_essentials::core
{

namespace sq
{

static constexpr detail::front_t front = {};
static constexpr detail::front_or_t front_or = {};
static constexpr detail::front_or_default_t front_or_default = {};
static constexpr detail::front_or_eval_t front_or_eval = {};
static constexpr detail::front_or_none_t front_or_none = {};
static constexpr detail::size_t size = {};
static constexpr detail::empty_t empty = {};
static constexpr detail::copy_while_t copy_while = {};
static constexpr detail::copy_until_t copy_until = {};
static constexpr detail::starts_with_t starts_with = {};
static constexpr detail::ends_with_t ends_with = {};
static constexpr detail::contains_t contains = {};

} /* namespace sq */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_SQ_HPP_ */
