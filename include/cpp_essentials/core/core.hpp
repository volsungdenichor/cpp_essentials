#ifndef CPP_ESSENTIALS_CORE_CORE_HPP_
#define CPP_ESSENTIALS_CORE_CORE_HPP_

#pragma once

#include <utility>

#define FORWARD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#endif /* CPP_ESSENTIALS_CORE_CORE_HPP_ */