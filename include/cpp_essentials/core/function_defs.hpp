#ifndef CPP_ESSENTIALS_CORE_FUNCTION_DEFS_HPP_
#define CPP_ESSENTIALS_CORE_FUNCTION_DEFS_HPP_

#pragma once

#include <functional>

namespace cpp_essentials::core
{

template <class... Args>
using mut_action = std::function<void(Args&...)>;

template <class R, class... Args>
using function = std::function<R(const Args&...)>;

template <class... Args>
using action = function<void, Args...>;

template <class... Args>
using predicate = function<bool, Args...>;

template <class T>
using comparison = predicate<T, T>;

} /* namespace cpp_essentials::core */

#endif /* CPP_ESSENTIALS_CORE_FUNCTION_DEFS_HPP_ */
