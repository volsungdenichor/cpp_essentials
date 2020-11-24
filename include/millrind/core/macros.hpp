#pragma once

#define FORWARD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

#define LAMBDA_ARG(A ,...) [](auto&& A) -> decltype(__VA_ARGS__) { return (__VA_ARGS__); }

#define LAMBDA(...) LAMBDA_ARG(_, __VA_ARGS__)