#ifndef CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_
#define CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_

#pragma once

#include <iterator>
#include <tuple>
#include <type_traits>

namespace cpp_essentials::core
{

namespace detail
{

template <class Func, class... Iters>
struct iterator_func_helper
{
    using tuple_type = std::tuple<Iters...>;
    using reference = std::tuple<typename std::iterator_traits<Iters>::reference...>;

#if _HAS_CXX17
    using type = std::invoke_result_t<Func, typename std::iterator_traits<Iters>::reference...>;
#else
    using type = std::result_of_t<Func(typename std::iterator_traits<Iters>::reference...)>;
#endif
};

} /* namespace detail */

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_DETAIL_ITERATOR_FUNC_HELPER_HPP_ */
